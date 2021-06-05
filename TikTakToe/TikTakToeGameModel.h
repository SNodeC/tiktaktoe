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
