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

#ifndef TIKTAKTOEGAMEMODEL_H
#define TIKTAKTOEGAMEMODEL_H

#include <nlohmann/json.hpp>
#include <string>

class TikTakToeGameModel {
public:
    void playersMove(const std::string& player, int cellID);
    void resetBoard();
    nlohmann::json updateClientState();

protected:
    std::string players[2] = {"red", "blue"};
    int whosNext = 0;
    int numPlayers = 0;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    friend class TikTakToeSubProtocol;
};

#endif // TIKTAKTOEGAMEMODEL_H
