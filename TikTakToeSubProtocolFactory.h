/*
 * TikTakToe - a demo game using the snode.c framework
 * Copyright (C) 2020, 2021, 2022, 2023 Volker Christian <me@vchrist.at>
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

#include "TikTakToeSubProtocol.h"

#include <web/websocket/SubProtocolFactory.h>

class TikTakToeSubProtocolFactory : public web::websocket::SubProtocolFactory<TikTakToeSubProtocol> {
public:
    using web::websocket::SubProtocolFactory<TikTakToeSubProtocol>::SubProtocolFactory;

private:
    TikTakToeSubProtocol* create(web::websocket::SubProtocolContext* subProtocolContext) override;
};

extern "C" TikTakToeSubProtocolFactory* tiktaktoeServerSubProtocolFactory();

#endif
