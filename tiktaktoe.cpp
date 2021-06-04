/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "config.h" // just for this example app
#include "express/legacy/WebApp.h"
#include "express/middleware/StaticMiddleware.h"
#include "express/tls/WebApp.h"
#include "log/Logger.h"

#include <openssl/x509v3.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

using namespace express;

Router getRouter() {
  Router router;

  router.use(middleware::StaticMiddleware(SERVERROOT));

  return router;
}

int main(int argc, char **argv) {
  WebApp::init(argc, argv);

  legacy::WebApp6 legacyApp(getRouter());

  legacyApp.listen(8080, [](int err) -> void {
    if (err != 0) {
      PLOG(FATAL) << "listen on port 8080";
    } else {
      VLOG(0) << "snode.c listening on port 8080 for legacy connections";
    }
  });

  legacyApp.onConnect(
      [](const legacy::WebApp6::SocketAddress &localAddress,
         const legacy::WebApp6::SocketAddress &remoteAddress) -> void {
        VLOG(0) << "OnConnect:";

        VLOG(0) << "\tServer: " + localAddress.toString();
        VLOG(0) << "\tClient: " + remoteAddress.toString();
      });

  legacyApp.onDisconnect([](legacy::WebApp6::SocketConnection *socketConnection)
                             -> void {
    VLOG(0) << "OnDisconnect:";
    VLOG(0) << "\tServer: " + socketConnection->getLocalAddress().host() + "(" +
                   socketConnection->getLocalAddress().ip() + "):" +
                   std::to_string(socketConnection->getLocalAddress().port());
    VLOG(0) << "\tClient: " + socketConnection->getRemoteAddress().host() +
                   "(" + socketConnection->getRemoteAddress().ip() + "):" +
                   std::to_string(socketConnection->getRemoteAddress().port());
  });

  tls::WebApp6 tlsApp(getRouter(), {{"certChain", SERVERCERTF},
                                    {"keyPEM", SERVERKEYF},
                                    {"password", KEYFPASS}});
  tlsApp.onConnect(
      [](const tls::WebApp6::SocketAddress &localAddress,
         const tls::WebApp6::SocketAddress &remoteAddress) -> void {
        VLOG(0) << "OnConnect:";

        VLOG(0) << "\tServer: " + localAddress.toString();
        VLOG(0) << "\tClient: " + remoteAddress.toString();
      });

  tlsApp.onConnected([](tls::WebApp6::SocketConnection *socketConnection)
                         -> void {
    VLOG(0) << "OnConnected:";

    X509 *client_cert = SSL_get_peer_certificate(socketConnection->getSSL());

    if (client_cert != NULL) {
      int verifyErr = SSL_get_verify_result(socketConnection->getSSL());

      VLOG(0) << "\tClient certificate: " +
                     std::string(X509_verify_cert_error_string(verifyErr));

      char *str = X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0);
      VLOG(0) << "\t   Subject: " + std::string(str);
      OPENSSL_free(str);

      str = X509_NAME_oneline(X509_get_issuer_name(client_cert), 0, 0);
      VLOG(0) << "\t   Issuer: " + std::string(str);
      OPENSSL_free(str);

      // We could do all sorts of certificate verification stuff here before
      // deallocating the certificate.

      GENERAL_NAMES *subjectAltNames = static_cast<GENERAL_NAMES *>(
          X509_get_ext_d2i(client_cert, NID_subject_alt_name, NULL, NULL));

      int32_t altNameCount = sk_GENERAL_NAME_num(subjectAltNames);
      VLOG(0) << "\t   Subject alternative name count: " << altNameCount;
      for (int32_t i = 0; i < altNameCount; ++i) {
        GENERAL_NAME *generalName = sk_GENERAL_NAME_value(subjectAltNames, i);
        if (generalName->type == GEN_URI) {
          std::string subjectAltName = std::string(
              reinterpret_cast<const char *>(ASN1_STRING_get0_data(
                  generalName->d.uniformResourceIdentifier)),
              ASN1_STRING_length(generalName->d.uniformResourceIdentifier));
          VLOG(0) << "\t      SAN (URI): '" + subjectAltName;
        } else if (generalName->type == GEN_DNS) {
          std::string subjectAltName =
              std::string(reinterpret_cast<const char *>(
                              ASN1_STRING_get0_data(generalName->d.dNSName)),
                          ASN1_STRING_length(generalName->d.dNSName));
          VLOG(0) << "\t      SAN (DNS): '" + subjectAltName;
        } else {
          VLOG(0) << "\t      SAN (Type): '" +
                         std::to_string(generalName->type);
        }
      }
      sk_GENERAL_NAME_pop_free(subjectAltNames, GENERAL_NAME_free);

      X509_free(client_cert);
    } else {
      VLOG(0) << "\tClient certificate: no certificate";
    }
  });

  tlsApp.onDisconnect([](tls::WebApp6::SocketConnection *socketConnection)
                          -> void {
    VLOG(0) << "OnDisconnect:";
    VLOG(0) << "\tServer: " + socketConnection->getLocalAddress().host() + "(" +
                   socketConnection->getLocalAddress().ip() + "):" +
                   std::to_string(socketConnection->getLocalAddress().port());
    VLOG(0) << "\tClient: " + socketConnection->getRemoteAddress().host() +
                   "(" + socketConnection->getRemoteAddress().ip() + "):" +
                   std::to_string(socketConnection->getRemoteAddress().port());
  });

  tlsApp.listen(8088, [](int err) -> void {
    if (err != 0) {
      PLOG(FATAL) << "listen on port 8088";
    } else {
      VLOG(0) << "snode.c listening on port 8088 for SSL/TLS connections";
    }
  });

  return WebApp::start();
}
