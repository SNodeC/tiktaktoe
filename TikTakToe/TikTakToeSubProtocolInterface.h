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

#ifndef TIKTAKTOESUBPROTOCOLINTERFACE_H
#define TIKTAKTOESUBPROTOCOLINTERFACE_H

namespace web::ws {
    class SubProtocol;
}

#include <string> // for string
#include <web/ws/server/SubProtocolInterface.h>

class TikTakToeSubProtocolInterface : public web::ws::server::SubProtocolInterface {
private:
    void destroy() override;

    std::string name() override;
    ROLE role() override;

    web::ws::SubProtocol* create() override;
    void destroy(web::ws::SubProtocol* tikTakToeSubProtocol) override;
};

extern "C" {
    class TikTakToeSubProtocolInterface* plugin();
}

#endif // TIKTAKTOESUBPROTOCOLINTERFACE_H
