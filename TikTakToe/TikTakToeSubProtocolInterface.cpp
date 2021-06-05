#include "TikTakToeSubProtocolInterface.h"

#include "TikTakToeSubProtocol.h"

#define NAME "tiktaktoe"

web::ws::SubProtocol* TikTakToeSubProtocolInterface::create([[maybe_unused]] web::http::server::Request& req,
                                                            [[maybe_unused]] web::http::server::Response& res) {
    return new TikTakToeSubProtocol(NAME, gameModel);
}

std::string TikTakToeSubProtocolInterface::name() {
    return NAME;
}

web::ws::SubProtocol::Role TikTakToeSubProtocolInterface::role() {
    return web::ws::SubProtocol::Role::SERVER;
}

void TikTakToeSubProtocolInterface::destroy(web::ws::SubProtocol* tikTakToeSubProtocol) {
    delete tikTakToeSubProtocol;
}

extern "C" {
    class web::ws::server::SubProtocolInterface* plugin() {
        return new TikTakToeSubProtocolInterface();
    }
}
