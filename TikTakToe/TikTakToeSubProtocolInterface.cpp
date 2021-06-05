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

#include "TikTakToeSubProtocolInterface.h"

#include "TikTakToeGameModel.h"
#include "TikTakToeSubProtocol.h"

#define NAME "tiktaktoe"

web::ws::SubProtocol* TikTakToeSubProtocolInterface::create([[maybe_unused]] web::http::server::Request& req,
                                                            [[maybe_unused]] web::http::server::Response& res) {
    return new TikTakToeSubProtocol(NAME, TikTakToeGameModel::getGameModel());
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
