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

#ifndef TIKTAKTOEGAMEMODEL_H
#define TIKTAKTOEGAMEMODEL_H

#include <nlohmann/json_fwd.hpp>
#include <string>

class TikTakToeGameModel {
protected:
  TikTakToeGameModel() = default;

public:
  void playersMove(const std::string &player, int cell);
  void resetBoard();
  int checkState(int board[]);

  nlohmann::json updateClientState();

  static TikTakToeGameModel &getGameModel();

protected:
  std::string players[2] = {"player_1", "player_2"};
  std::string winner = "";
  std::string state = "setup";

  int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int score[3] = {0, 0, 0};
  int whosNext = 0;
  int numPlayers = 0;

  friend class TikTakToeSubProtocol;

  static TikTakToeGameModel gameModel;
};

#endif
