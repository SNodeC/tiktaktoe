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

#include "TikTakToeGameModel.h"

#include <map>               // for operator==
#include <nlohmann/json.hpp> // for basic_json<>::object_t, basic_json<>::v...

TikTakToeGameModel TikTakToeGameModel::gameModel;

void TikTakToeGameModel::playersMove(const std::string& player, int cellID) {
    if (player == players[whosNext]) {
        int cellValue = 0;

        if (player == "red") {
            cellValue = 1;
        } else if (player == "blue") {
            cellValue = -1;
        }

        board[cellID] = cellValue;
        
        checkIfGameCompleted();

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

void TikTakToeGameModel::checkIfGameCompleted() {
    //Check for a winner
    if(checkForWinner()){
        gameFinished = true;
        winner = players[whosNext];
    }        
    
    //Check for a draw
    if(checkIfBoardFull() && !gameFinished) {
        gameFinished = true;
        draw = true;
    }    
}

bool TikTakToeGameModel::checkForWinner() {
    //Vertical
    for(int i = 0; i < 3; i++) {
        if(board[i] != 0 && board[0 + i] == board [3 + i] && board[3 + i] == board[6 + i]){
            return true;
        }
    }
    
    //Horizontal
    for(int i = 0; i < 3; i++) {
        if(board[i * 3] != 0 && board[i * 3] == board [i * 3 + 1] && board[i * 3 + 1] == board[i * 3 + 2]){
            return true;
        }
    }
    
    //Diagonal left to right
    if(board[0] != 0 && board[0] == board[4] && board[4] == board[8]) {
        return true;
    }
    
    //Diagonal right to left
    if(board[2] != 0 && board[2] == board[4] && board[4] == board[6]) {
        return true;
    }
    
    return false;
}


bool TikTakToeGameModel::checkIfBoardFull() {
    for(int i = 0; i < 9; i++) {
        if(board[i] == 0) {
            return false;
        }
    }
    return true;
}


nlohmann::json TikTakToeGameModel::updateClientState() {
    nlohmann::json message;

    message["type"] = "update";
    message["whosTurn"] = players[whosNext];
    message["board"] = board;
    
    if(gameFinished) {
        if(draw) {
            message["gameOver"] = "Nobody won, it's a draw!";
        } else {
            message["gameOver"] = "Congratulations, player " + winner + " won!";
        }
    }

    return message;
}

TikTakToeGameModel& TikTakToeGameModel::getGameModel() {
    return gameModel;
}
