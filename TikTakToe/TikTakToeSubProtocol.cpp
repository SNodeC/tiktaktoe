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

#include "TikTakToeSubProtocol.h"

#include "TikTakToeGameModel.h"
#include "config.h"

#include <log/Logger.h>
#include <nlohmann/json.hpp>
#include <web/http/server/Request.h>
#include <web/http/server/Response.h>

TikTakToeSubProtocol::TikTakToeSubProtocol(const std::string& name, TikTakToeGameModel& gameModel)
    : web::websocket::server::SubProtocol(name)
    , gameModel(gameModel)
    , timer(net::timer::Timer::intervalTimer(
          [this]([[maybe_unused]] const void* arg, [[maybe_unused]] const std::function<void()>& stop) -> void {
              this->sendPing();
              this->flyingPings++;
              if (this->flyingPings >= MAX_FLYING_PINGS) {
                  this->sendClose();
              }
          },
          {10, 0},
          nullptr)) {
}

TikTakToeSubProtocol::~TikTakToeSubProtocol() {
    timer.cancel();
}

void TikTakToeSubProtocol::onConnected() {
    // VLOG(0) << "TikTakToe on connected:";

    // VLOG(0) << "\tServer: " + getLocalAddressAsString();
    // VLOG(0) << "\tClient: " + getRemoteAddressAsString();

    // VLOG(0) << "\tNumPlayers: " << gameModel.numPlayers;

    if (gameModel.numPlayers < 2) {
        nlohmann::json json;

        json["board"] = gameModel.board;
        json["score"] = gameModel.score;
        json["state"] = gameModel.state;
        json["winner"] = gameModel.winner;
        json["leader"] = gameModel.players[gameModel.whosNext];
        json["player"] = gameModel.players[gameModel.numPlayers++];

        sendMessage(json.dump());
        activePlayer = true;

        VLOG(0) << "JSON: " << json.dump();
    } else {
        sendClose();

        VLOG(0) << "sendClose";
    }
}

void TikTakToeSubProtocol::onMessageStart([[maybe_unused]] int opCode) {
    // VLOG(0) << "TikTakToe on Message Start - OpCode: " << opCode;
}

void TikTakToeSubProtocol::onMessageData(const char* junk, size_t junkLen) {
    data += std::string(junk, junkLen);
}

void TikTakToeSubProtocol::onMessageEnd() {
    // VLOG(0) << "TikTakToe on Data: " << data;

    nlohmann::json action = nlohmann::json::parse(data);

    VLOG(0) << "Action dump: " << action.dump();

    if (action["type"] == "move") {
        gameModel.playersMove(action["player"], action["cell"]);
        nlohmann::json message = gameModel.updateClientState();

        sendBroadcast(message.dump());
        VLOG(0) << "SendMessage Dump: " << message.dump();
    } else if (action["type"] == "reset") {
        gameModel.resetBoard();
        nlohmann::json message = gameModel.updateClientState();

        sendBroadcast(message.dump());
        VLOG(0) << "SendMessage Dump: " << message.dump();
    }

    data.clear();
}

void TikTakToeSubProtocol::onMessageError(uint16_t errnum) {
    VLOG(0) << "TikTakToe on Message error: " << errnum;
}

void TikTakToeSubProtocol::onPongReceived() {
    // VLOG(0) << "TikTakToe on Pong received";
    flyingPings = 0;
}

void TikTakToeSubProtocol::onDisconnected() {
    // VLOG(0) << "TikTakToe on disconnected:";

    if (activePlayer) {
        gameModel.numPlayers--;

        if (gameModel.numPlayers == 0) {
            gameModel.resetBoard();
        }
    }

    // VLOG(0) << "\tServer: " + getLocalAddressAsString();
    // VLOG(0) << "\tClient: " + getRemoteAddressAsString();
}
