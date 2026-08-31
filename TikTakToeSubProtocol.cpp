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

#include "TikTakToeSubProtocol.h"

#include "TikTakToeGameModel.h"
#include "config.h"

#include <cmath>
#include <cstring>
#include <SemanticLog.h>
#include <map>
#include <nlohmann/json.hpp>

// IWYU pragma: no_include <nlohmann/json_fwd.hpp>

TikTakToeSubProtocol::TikTakToeSubProtocol(web::websocket::SubProtocolContext* subProtocolContext,
                                           const std::string& name,
                                           TikTakToeGameModel& gameModel)
    : web::websocket::server::SubProtocol(subProtocolContext, name, PING_INTERVAL, MAX_FLYING_PINGS)
    , gameModel(gameModel) {
}

void TikTakToeSubProtocol::onConnected() {
    snode::semantic::appLog().info() << "TikTakToe connected:";

    if (gameModel.numPlayers < 2) {
        nlohmann::json json;

        json["board"] = gameModel.board;
        json["score"] = gameModel.score;
        json["state"] = gameModel.state;
        json["winner"] = gameModel.winner;
        json["leader"] = gameModel.players[gameModel.whosNext];
        json["player"] = gameModel.players[gameModel.numPlayers++];

        const std::string message = json.dump();
        sendMessage(message);
        activePlayer = true;

        auto log = snode::semantic::appLog();
        if (log.enabled(logger::LogLevel::Trace)) {
            log.trace() << "JSON: " << message;
        }
    } else {
        sendClose();

        snode::semantic::appLog().debug() << "sendClose";
    }
}

void TikTakToeSubProtocol::onMessageStart([[maybe_unused]] int opCode) {
}

void TikTakToeSubProtocol::onMessageData(const char* junk, std::size_t junkLen) {
    data += std::string(junk, junkLen);
}

void TikTakToeSubProtocol::onMessageEnd() {
    nlohmann::json action = nlohmann::json::parse(data);

    auto log = snode::semantic::appLog();
    if (log.enabled(logger::LogLevel::Trace)) {
        log.trace() << "Action dump: " << action.dump();
    }

    if (action["type"] == "move") {
        gameModel.playersMove(action["player"], action["cell"]);
        nlohmann::json message = gameModel.updateClientState();

        const std::string payload = message.dump();
        sendBroadcast(payload);
        if (log.enabled(logger::LogLevel::Trace)) {
            log.trace() << "SendMessage Dump: " << payload;
        }
    } else if (action["type"] == "reset") {
        gameModel.resetBoard();
        nlohmann::json message = gameModel.updateClientState();

        const std::string payload = message.dump();
        sendBroadcast(payload);
        if (log.enabled(logger::LogLevel::Trace)) {
            log.trace() << "SendMessage Dump: " << payload;
        }
    }

    data.clear();
}

void TikTakToeSubProtocol::onMessageError(uint16_t errnum) {
    snode::semantic::appLog().warn() << "TikTakToe: Message error: " << errnum;
}

void TikTakToeSubProtocol::onDisconnected() {
    snode::semantic::appLog().info() << "TikTakToe: disconnected:";

    if (activePlayer) {
        gameModel.numPlayers--;

        if (gameModel.numPlayers == 0) {
            gameModel.resetBoard();
        }
    }
}

bool TikTakToeSubProtocol::onSignal(int signum) {
    snode::semantic::appLog().info() << "TikTakToe: exit:";

    snode::semantic::appLog().info() << "SubProtocol 'TikTakTop' exit doe to '" << strsignal(signum) << "' (SIG" << sigabbrev_np(signum) << " = " << signum << ")";

    return true;
}
