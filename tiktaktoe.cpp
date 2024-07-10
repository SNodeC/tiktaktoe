/*
 * TikTakToe - a demo game using the snode.c framework
 * Copyright (C) 2020, 2021, 2022, 2023 Volker Christian <me@vchrist.at>
 * Copyright (C) 2021 Ertug Obalar, Jens Patzelt and Milad Tousi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
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

#ifdef LINK_SUBPROTOCOL_STATIC
#include "TikTakToeSubProtocolFactory.h"
#include "web/websocket/server/SubProtocolFactorySelector.h"
#endif

#if defined(LINK_WEBSOCKET_STATIC) || defined(LINK_SUBPROTOCOL_STATIC)
#include "web/websocket/server/SocketContextUpgradeFactory.h"
#endif

#include "config.h"
#include "core/SNodeC.h"
#include "express/legacy/in/WebApp.h"
#include "express/tls/in/WebApp.h"
#include "web/http/http_utils.h" // for ci_contains

#include <log/Logger.h>

int main(int argc, char* argv[]) {
#if defined(LINK_WEBSOCKET_STATIC) || defined(LINK_SUBPROTOCOL_STATIC)
    web::websocket::server::SocketContextUpgradeFactory::link();
#endif

#ifdef LINK_SUBPROTOCOL_STATIC
    web::websocket::server::SubProtocolFactorySelector::link("tiktaktoe", tiktaktoeServerSubProtocolFactory);
#endif

    core::SNodeC::init(argc, argv);

    const express::legacy::in::WebApp legacyApp("legacy");

    legacyApp.get("/", [] APPLICATION(req, res) {
        if (req->url == "/") {
            req->url = "/index.html";
        }

        res->sendFile(CMAKE_SOURCE_DIR "public" + req->url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req->url;
            }
        });
    });

    legacyApp.get("/css", [] APPLICATION(req, res) {
        res->sendFile(CMAKE_SOURCE_DIR "public" + req->url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req->url;
            }
        });
    });

    legacyApp.get("/js", [] APPLICATION(req, res) {
        res->sendFile(CMAKE_SOURCE_DIR "public" + req->url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req->url;
            }
        });
    });

    legacyApp.get("/sfx", [] APPLICATION(req, res) {
        res->sendFile(CMAKE_SOURCE_DIR "public" + req->url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req->url;
            }
        });
    });

    legacyApp.get("/ws", [] APPLICATION(req, res) {
        if (req->get("sec-websocket-protocol").find("tiktaktoe") != std::string::npos) {
            res->upgrade(req, [req, res](bool success) -> void {
                if (success) {
                    VLOG(1) << "Successful upgrade to '" << req->get("upgrade") << "'";
                } else {
                    VLOG(1) << "Can not upgrade to '" << req->get("upgrade") << "'";
                }
                res->end();
            });
        } else {
            res->sendStatus(404);
        }
    });

    legacyApp.listen([instanceName = legacyApp.getConfig().getInstanceName()](
                         const net::in::SocketAddress& socketAddress,
                         const core::socket::State& state) -> void { // Listen on all bluetooth interfaces on channel 16{
        switch (state) {
            case core::socket::State::OK:
                VLOG(1) << instanceName << ": listening on '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                VLOG(1) << instanceName << ": disabled";
                break;
            case core::socket::State::ERROR:
                LOG(ERROR) << instanceName << ": " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                LOG(FATAL) << instanceName << ": " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    const express::tls::in::WebApp tlsApp("tls");

    tlsApp.get("/", [] APPLICATION(req, res) {
        if (req->url == "/") {
            req->url = "/index.html";
        }

        res->sendFile(CMAKE_SOURCE_DIR "public" + req->url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req->url;
            }
        });
    });

    tlsApp.get("/css", [] APPLICATION(req, res) {
        res->sendFile(CMAKE_SOURCE_DIR "public" + req->url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req->url;
            }
        });
    });

    tlsApp.get("/js", [] APPLICATION(req, res) {
        res->sendFile(CMAKE_SOURCE_DIR "public" + req->url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req->url;
            }
        });
    });

    tlsApp.get("/sfx", [] APPLICATION(req, res) {
        res->sendFile(CMAKE_SOURCE_DIR "public" + req->url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req->url;
            }
        });
    });

    tlsApp.get("/ws", [] APPLICATION(req, res) {
        if (req->get("sec-websocket-protocol").find("tiktaktoe") != std::string::npos) {
            res->upgrade(req, [req, res](bool success) -> void {
                if (success) {
                    VLOG(1) << "Successful upgrade to '" << req->get("upgrade") << "'";
                } else {
                    VLOG(1) << "Can not upgrade to '" << req->get("upgrade") << "'";
                }
                res->end();
            });
        } else {
            res->sendStatus(404);
        }
    });

    tlsApp.listen([instanceName = legacyApp.getConfig().getInstanceName()](
                      const net::in::SocketAddress& socketAddress,
                      const core::socket::State& state) -> void { // Listen on all bluetooth interfaces on channel 16{
        switch (state) {
            case core::socket::State::OK:
                VLOG(1) << instanceName << ": listening on '" << socketAddress.toString() << "'";
                break;
            case core::socket::State::DISABLED:
                VLOG(1) << instanceName << ": disabled";
                break;
            case core::socket::State::ERROR:
                LOG(ERROR) << instanceName << ": " << socketAddress.toString() << ": " << state.what();
                break;
            case core::socket::State::FATAL:
                LOG(FATAL) << instanceName << ": " << socketAddress.toString() << ": " << state.what();
                break;
        }
    });

    return core::SNodeC::start();
}
