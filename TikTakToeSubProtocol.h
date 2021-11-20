/*
 * TikTakToe - a demo game using the snode.c framework
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
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

#include "web/websocket/server/SubProtocol.h" // IWYU pragma: export

#include <cstddef>
#include <stdint.h> // for uint16_t
#include <string>   // for string

namespace core::timer {
    class Timer;
}

class TikTakToeSubProtocol : public web::websocket::server::SubProtocol {
public:
    TikTakToeSubProtocol(TikTakToeGameModel& game);

    ~TikTakToeSubProtocol() override;

    void onConnected() override;
    void onMessageStart(int opCode) override;
    void onMessageData(const char* junk, std::size_t junkLen) override;
    void onMessageEnd() override;
    void onMessageError(uint16_t errnum) override;
    void onPongReceived() override;
    void onDisconnected() override;

private:
    bool activePlayer = false;

    std::string data;

    int flyingPings = 0;

    TikTakToeGameModel& gameModel;

    core::timer::Timer& timer;
};

#endif
