#include "TikTakToeSubProtocol.h"

#include "log/Logger.h"
#include "web/http/server/Request.h"
#include "web/http/server/Response.h"

#include <nlohmann/json.hpp>

#define MAX_FLYING_PINGS 3

TikTakToeSubProtocol::TikTakToeSubProtocol(const std::string& name, TikTakToeGameModel& gameModel)
    : web::ws::server::SubProtocol(name)
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

void TikTakToeSubProtocol::onProtocolConnected() {
    VLOG(0) << "On TikTakToe connected:";

    VLOG(0) << "\tServer: " + getLocalAddressAsString();
    VLOG(0) << "\tClient: " + getRemoteAddressAsString();

    VLOG(0) << "\tNumPlayers: " << gameModel.numPlayers;

    if (gameModel.numPlayers < 2) {
        nlohmann::json json;

        json["type"] = "setup";
        json["playerData"]["whosTurn"] = gameModel.players[gameModel.whosNext];
        json["playerData"]["playerID"] = gameModel.players[gameModel.numPlayers++];
        json["playerData"]["board"] = gameModel.board;

        VLOG(0) << "Json: " << json.dump();

        sendMessage(json.dump());

        playing = true;
    } else {
        sendClose();
    }
}

void TikTakToeSubProtocol::onMessageStart(int opCode) {
    VLOG(0) << "TikTakToe Message Start - OpCode: " << opCode;
}

void TikTakToeSubProtocol::onMessageData(const char* junk, std::size_t junkLen) {
    data += std::string(junk, junkLen);
}

void TikTakToeSubProtocol::onMessageEnd() {
    VLOG(0) << "TikTakToe Data: " << data;

    nlohmann::json action = nlohmann::json::parse(data);

    VLOG(0) << "Action dump: " << action.dump();

    if (action["type"] == "move") {
        gameModel.playersMove(action["playerID"], action["cellID"]);
        nlohmann::json message = gameModel.updateClientState();

        /* // also possible
                forEachClient([&message](SubProtocol* client) {
                    client->sendMessage(message.dump());
                });
        */

        sendBroadcast(message.dump());
        VLOG(0) << "SendMessage Dump: " << message.dump();
    }

    data.clear();
}

void TikTakToeSubProtocol::onMessageError(uint16_t errnum) {
    VLOG(0) << "TikTakToe Message error: " << errnum;
}

void TikTakToeSubProtocol::onPongReceived() {
    VLOG(0) << "TikTakToe Pong received";
    flyingPings = 0;
}
void TikTakToeSubProtocol::onProtocolDisconnected() {
    VLOG(0) << "On TikTakToe disconnected:";

    if (playing) {
        gameModel.numPlayers--;

        if (gameModel.numPlayers == 0) {
            gameModel.resetBoard();
        }
    }

    VLOG(0) << "\tServer: " + getLocalAddressAsString();
    VLOG(0) << "\tClient: " + getRemoteAddressAsString();
}
