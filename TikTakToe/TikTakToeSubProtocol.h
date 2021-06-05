#ifndef TIKTAKTOESUBPROTOCOL_H
#define TIKTAKTOESUBPROTOCOL_H

#include "TikTakToeGameModel.h"
#include "net/timer/IntervalTimer.h"
#include "web/ws/server/SubProtocol.h"

class TikTakToeSubProtocol : public web::ws::server::SubProtocol {
public:
    TikTakToeSubProtocol(const std::string& name, TikTakToeGameModel& game);

    ~TikTakToeSubProtocol() override;

    void onProtocolConnected() override;
    void onMessageStart(int opCode) override;
    void onMessageData(const char* junk, std::size_t junkLen) override;
    void onMessageEnd() override;
    void onMessageError(uint16_t errnum) override;
    void onPongReceived() override;
    void onProtocolDisconnected() override;

private:
    bool playing = false;

    std::string data;

    int flyingPings = 0;

    const std::string name;

    TikTakToeGameModel& gameModel;

    net::timer::Timer& timer;
};

#endif // TIKTAKTOESUBPROTOCOL_H
