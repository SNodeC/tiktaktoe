/*
 * tiktaktoe - a game using SnodeC
 * Copyright (C) 2021 Volker Christian <me@vchrist.at>
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

#include "TikTakToeSubProtocolFactory.h"
#include "config.h"

#include <express/legacy/WebApp.h>
#include <express/tls/WebApp.h>
#include <log/Logger.h>
#include <net/SNodeC.h>

int main(int argc, char* argv[]) {
    net::SNodeC::init(argc, argv);

    plugin();

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
            PLOG(ERROR) << "Listen";
        } else {
            VLOG(0) << "snode.c listening on port 8080";
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
            PLOG(ERROR) << "Listen";
        } else {
            VLOG(0) << "snode.c listening on port 8088";
        }
    });

    return net::SNodeC::start();
}
