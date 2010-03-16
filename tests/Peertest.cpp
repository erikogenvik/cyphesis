// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2009 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

// $Id$

#include "server/Peer.h"

#include "server/CommPeer.h"
#include "server/CommServer.h"
#include "server/Lobby.h"
#include "server/ServerRouting.h"

#include <Atlas/Objects/RootOperation.h>
#include <Atlas/Objects/SmartPtr.h>

#include <cassert>

int main()
{
    {
        Peer * p = new Peer(*(CommClient*)0, *(ServerRouting*)0, "addr", "1");
        delete p;
    }

    {
        Peer * p = new Peer(*(CommClient*)0, *(ServerRouting*)0, "addr", "1");

        Atlas::Objects::Operation::RootOperation op;
        OpVector res;
        p->operation(op, res);

        delete p;
    }
    return 0;
}
