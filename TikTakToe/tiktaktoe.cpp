#include "TikTakToeSubProtocolInterface.h"
#include "config.h"
#include "express/legacy/WebApp.h"
#include "express/tls/WebApp.h"
#include "log/Logger.h"
#include "net/SNodeC.h"
#include "web/ws/server/SubProtocolSelector.h"

#include <cstddef>
#include <endian.h>
#include <iomanip>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    net::SNodeC::init(argc, argv);

    web::ws::server::SubProtocolSelector::instance().registerSubProtocol(plugin());

    express::legacy::WebApp legacyApp;

    legacyApp.get("/", [] APPLICATION(req, res) {
        if (req.url == "/") {
            req.url = "/index.html";
        }

        res.sendFile("/home/voc/projects/tiktaktoe/websocket-tic-tac-toe/public" + req.url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req.url;
            }
        });
    });

    legacyApp.get("/stylesheets", [] APPLICATION(req, res) {
        res.sendFile("/home/voc/projects/tiktaktoe/websocket-tic-tac-toe/public" + req.url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req.url;
            }
        });
    });

    legacyApp.get("/js", [] APPLICATION(req, res) {
        res.sendFile("/home/voc/projects/tiktaktoe/websocket-tic-tac-toe/public" + req.url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req.url;
            }
        });
    });

    legacyApp.get("/ws", [] APPLICATION(req, res) {
        if (httputils::ci_contains(req.header("connection"), "Upgrade")) {
            res.upgrade(req);
        } else {
            res.sendStatus(404);
        }
    });

    legacyApp.listen(8080, [](int err) -> void {
        if (err != 0) {
            perror("Listen");
        } else {
            std::cout << "snode.c listening on port 8080" << std::endl;
        }
    });

    express::tls::WebApp tlsApp({{"certChain", SERVERCERTF}, {"keyPEM", SERVERKEYF}, {"password", KEYFPASS}});

    tlsApp.get("/", [] APPLICATION(req, res) {
        if (req.url == "/") {
            req.url = "/index.html";
        }

        res.sendFile("/home/voc/projects/tiktaktoe/websocket-tic-tac-toe/public" + req.url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req.url;
            }
        });
    });

    tlsApp.get("/stylesheets", [] APPLICATION(req, res) {
        res.sendFile("/home/voc/projects/tiktaktoe/websocket-tic-tac-toe/public" + req.url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req.url;
            }
        });
    });

    tlsApp.get("/js", [] APPLICATION(req, res) {
        res.sendFile("/home/voc/projects/tiktaktoe/websocket-tic-tac-toe/public" + req.url, [&req](int ret) -> void {
            if (ret != 0) {
                PLOG(ERROR) << req.url;
            }
        });
    });

    tlsApp.get("/ws", [] APPLICATION(req, res) {
        if (httputils::ci_contains(req.header("connection"), "Upgrade")) {
            res.upgrade(req);
        } else {
            res.sendStatus(404);
        }
    });

    tlsApp.listen(8088, [](int err) -> void {
        if (err != 0) {
            perror("Listen");
        } else {
            std::cout << "snode.c listening on port 8088" << std::endl;
        }
    });

    return net::SNodeC::start();
}
