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

#include "TikTakToeSubProtocolFactory.h"

#include "TikTakToeGameModel.h"
#include "TikTakToeSubProtocol.h"

#include <web/http/server/SocketContextUpgradeFactorySelector.h>
#include <web/websocket/server/SocketContextUpgradeFactory.h>
#include <web/websocket/server/SubProtocolFactorySelector.h>

#define NAME "tiktaktoe"

TikTakToeSubProtocol* TikTakToeSubProtocolFactory::create() {
    return new TikTakToeSubProtocol(NAME, TikTakToeGameModel::getGameModel());
}

TikTakToeSubProtocolFactory::TikTakToeSubProtocolFactory() {
    web::http::server::SocketContextUpgradeFactorySelector::instance()->add(new web::websocket::server::SocketContextUpgradeFactory(
        reinterpret_cast<web::websocket::SubProtocolFactory<web::websocket::server::SubProtocol>*>(this)));
}

void TikTakToeSubProtocolFactory::destroy() {
    delete this;
}

std::string TikTakToeSubProtocolFactory::name() {
    return NAME;
}

void TikTakToeSubProtocolFactory::destroy(TikTakToeSubProtocol* tikTakToeSubProtocol) {
    delete tikTakToeSubProtocol;
}

extern "C" {
    TikTakToeSubProtocolFactory* plugin() {
        return new TikTakToeSubProtocolFactory();
    }
}
