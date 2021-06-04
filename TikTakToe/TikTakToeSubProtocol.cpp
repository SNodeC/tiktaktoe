#include "TikTakToeSubProtocol.h"

#include "log/Logger.h"
#include "web/http/server/Request.h"
#include "web/http/server/Response.h"

#include <nlohmann/json.hpp>

#define MAX_FLYING_PINGS 3

#define NAME "tiktaktoe"

class TikTakToeSubProtocolInterface : public web::ws::server::SubProtocolInterface {
    web::ws::SubProtocol* create([[maybe_unused]] web::http::server::Request& req,
                                 [[maybe_unused]] web::http::server::Response& res) override {
        return new TikTakToeSubProtocol();
    }

    std::string name() override {
        return NAME;
    }

    web::ws::SubProtocol::Role role() override {
        return web::ws::SubProtocol::Role::SERVER;
    }

    void destroy(web::ws::SubProtocol* tikTakToeSubProtocol) override {
        delete tikTakToeSubProtocol;
    }
};

extern "C" {
    class web::ws::server::SubProtocolInterface* plugin() {
        return new TikTakToeSubProtocolInterface();
    }
}

struct GameState {
    std::string players[2] = {"red", "blue"};
    int whosNext = 0;
    int numPlayers = 0;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
};

static GameState gameState;

void playersMove(const std::string& player, int cellID) {
    if (player == gameState.players[gameState.whosNext]) {
        int cellValue = 0;

        if (player == "red") {
            cellValue = 1;
        } else if (player == "blue") {
            cellValue = -1;
        }

        gameState.board[cellID] = cellValue;

        if (gameState.whosNext >= 1) {
            gameState.whosNext = 0;
        } else {
            gameState.whosNext += 1;
        }
    }
}

void resetBoard() {
    gameState.whosNext = 0;
    for (int i = 0; i < 9; i++) {
        gameState.board[i] = 0;
    }
}

nlohmann::json updateClientState() {
    nlohmann::json message;

    message["type"] = "update";
    message["whosTurn"] = gameState.players[gameState.whosNext];
    message["board"] = gameState.board;

    return message;
}

TikTakToeSubProtocol::TikTakToeSubProtocol()
    : web::ws::server::SubProtocol(NAME)
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

    VLOG(0) << "\tNumPlayers: " << gameState.numPlayers;

    if (gameState.numPlayers < 2) {
        nlohmann::json json;

        json["type"] = "setup";
        json["playerData"]["whosTurn"] = gameState.players[gameState.whosNext];
        json["playerData"]["playerID"] = gameState.players[gameState.numPlayers++];
        json["playerData"]["board"] = gameState.board;

        VLOG(0) << "Json: " << json.dump();

        sendMessage(json.dump());
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
        playersMove(action["playerID"], action["cellID"]);
        nlohmann::json message = updateClientState();
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

    gameState.numPlayers--;

    if (gameState.numPlayers == 0) {
        resetBoard();
    }

    VLOG(0) << "\tServer: " + getLocalAddressAsString();
    VLOG(0) << "\tClient: " + getRemoteAddressAsString();
}
