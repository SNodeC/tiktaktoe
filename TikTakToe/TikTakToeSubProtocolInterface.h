#ifndef TIKTAKTOESUBPROTOCOLINTERFACE_H
#define TIKTAKTOESUBPROTOCOLINTERFACE_H

#include "TikTakToeGameModel.h"
#include "web/ws/server/SubProtocolInterface.h"

class TikTakToeSubProtocolInterface : public web::ws::server::SubProtocolInterface {
private:
    web::ws::SubProtocol* create(web::http::server::Request& req, web::http::server::Response& res) override;
    std::string name() override;
    web::ws::SubProtocol::Role role() override;
    void destroy(web::ws::SubProtocol* tikTakToeSubProtocol) override;

    TikTakToeGameModel gameModel;
};

extern "C" {
    class web::ws::server::SubProtocolInterface* plugin();
}

#endif // TIKTAKTOESUBPROTOCOLINTERFACE_H
