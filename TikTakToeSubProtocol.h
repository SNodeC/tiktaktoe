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

#ifndef TIKTAKTOESUBPROTOCOL_H
#define TIKTAKTOESUBPROTOCOL_H

class TikTakToeGameModel;

#include <web/websocket/server/SubProtocol.h>

namespace web::websocket {
    class SubProtocolContext;
}

#include <cstddef>
#include <stdint.h>
#include <string>

class TikTakToeSubProtocol : public web::websocket::server::SubProtocol {
private:
    using Super = web::websocket::server::SubProtocol;

public:
    TikTakToeSubProtocol(web::websocket::SubProtocolContext* subProtocolContext, const std::string& name, TikTakToeGameModel& game);

    void onConnected() override;
    void onMessageStart(int opCode) override;
    void onMessageData(const char* junk, std::size_t junkLen) override;
    void onMessageEnd() override;
    void onMessageError(uint16_t errnum) override;
    void onDisconnected() override;
    [[nodiscard]] bool onSignal(int signum) override;

private:
    bool activePlayer = false;

    std::string data;

    TikTakToeGameModel& gameModel;
};

#endif
