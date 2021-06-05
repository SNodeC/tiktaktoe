#include "TikTakToeGameModel.h"

void TikTakToeGameModel::playersMove(const std::string& player, int cellID) {
    if (player == players[whosNext]) {
        int cellValue = 0;

        if (player == "red") {
            cellValue = 1;
        } else if (player == "blue") {
            cellValue = -1;
        }

        board[cellID] = cellValue;

        if (whosNext >= 1) {
            whosNext = 0;
        } else {
            whosNext += 1;
        }
    }
}

void TikTakToeGameModel::resetBoard() {
    whosNext = 0;
    for (int i = 0; i < 9; i++) {
        board[i] = 0;
    }
}

nlohmann::json TikTakToeGameModel::updateClientState() {
    nlohmann::json message;

    message["type"] = "update";
    message["whosTurn"] = players[whosNext];
    message["board"] = board;

    return message;
}
