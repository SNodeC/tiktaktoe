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

#ifndef TIKTAKTOESUBPROTOCOLINTERFACE_H
#define TIKTAKTOESUBPROTOCOLINTERFACE_H

#include "web/websocket/SubProtocolFactory.h" // IWYU pragma: export
#include "web/websocket/server/SubProtocol.h" // IWYU pragma: export

// IWYU pragma: no_include <web/websocket/SubProtocolFactory.h>
// IWYU pragma: no_include <web/websocket/server/SubProtocol.h>

#include <string>

class TikTakToeSubProtocol;

class TikTakToeSubProtocolFactory : public web::websocket::SubProtocolFactory<TikTakToeSubProtocol> {
public:
    using web::websocket::SubProtocolFactory<TikTakToeSubProtocol>::SubProtocolFactory;

private:
    web::websocket::SubProtocolFactory<TikTakToeSubProtocol>::SubProtocol* create() override;
};

extern "C" void* tiktaktoeServerSubProtocolFactory();

#endif
