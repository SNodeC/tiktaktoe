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

#include "TikTakToeGameModel.h"

#include <map>               // for operator==
#include <nlohmann/json.hpp> // for basic_json<>::object_t, basic_json<>::v...

// IWYU pragma: no_include <nlohmann/detail/json_pointer.hpp>

TikTakToeGameModel TikTakToeGameModel::gameModel;

void TikTakToeGameModel::playersMove(const std::string& player, int cell) {
    if (player == players[whosNext]) {
        board[cell] = whosNext == 0 ? 1 : -1;

        switch (checkState(board)) {
            case 1:
                state = "update";
                break;

            case 2:
                state = "tie";
                score[1]++;
                break;

            case 3:
                state = "win";
                if (whosNext == 0)
                    score[0]++;
                else
                    score[2]++;
                winner = player;
                break;
        }

        whosNext = whosNext == 0 ? 1 : 0;
    }
}

int TikTakToeGameModel::checkState(int board[]) {
    int count = 0;

    // Horizontal Win Check
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            count += board[i * 3 + j];
        }

        if (count == 3 || count == -3) {
            for (int j = 0; j < 3; j++) {
                board[i * 3 + j] *= 2;
            }

            return 3;
        } else {
            count = 0;
        }
    }

    // Vertical Win Check
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            count += board[i + j * 3];
        }

        if (count == 3 || count == -3) {
            for (int j = 0; j < 3; j++) {
                board[i + j * 3] *= 2;
            }

            return 3;
        } else {
            count = 0;
        }
    }

    //  Left Diagonal Win Check
    {
        for (int i = 0; i < 3; i++) {
            count += board[i * 4];
        }

        if (count == 3 || count == -3) {
            for (int i = 0; i < 3; i++) {
                board[i * 4] *= 2;
            }

            return 3;
        } else {
            count = 0;
        }
    }

    // Right Diagonal Win Check
    {
        for (int i = 0; i < 3; i++) {
            count += board[i * 2 + 2];
        }

        if (count == 3 || count == -3) {
            for (int i = 0; i < 3; i++) {
                board[i * 2 + 2] *= 2;
            }

            return 3;
        } else {
            count = 0;
        }
    }

    // Ongoing Check
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0)
            return 1;
    }

    // Return Tie
    return 2;
}

void TikTakToeGameModel::resetBoard() {
    for (int i = 0; i < 9; i++) {
        board[i] = 0;
    }

    state = "reset";
    winner = "";
    whosNext = 0;
}

nlohmann::json TikTakToeGameModel::updateClientState() {
    nlohmann::json message;

    message["board"] = board;
    message["score"] = score;
    message["state"] = state;
    message["winner"] = winner;
    message["leader"] = players[whosNext];

    return message;
}

TikTakToeGameModel& TikTakToeGameModel::getGameModel() {
    return gameModel;
}
