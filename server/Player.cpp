// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2000,2001 Alistair Riddoch

#include "Player.h"

#include <Atlas/Objects/Operation/Create.h>

#include <common/types.h>

std::set<std::string> Player::playableTypes;

Player::Player(Connection * conn, const std::string& username,
               const std::string& passwd) : Account(conn, username, passwd)
{
}

Player::~Player() { }

const char * Player::getType() const
{
    return "player";
}

void Player::addToObject(Fragment::MapType & omap) const
{
    Account::addToObject(omap);
    Fragment::ListType typeList;
    std::set<std::string>::const_iterator I = Player::playableTypes.begin();
    for(;I != Player::playableTypes.end(); ++I) {
        typeList.push_back(Fragment(*I));
    }
    omap["character_types"] = typeList;
}

OpVector Player::characterError(const Create& op,const Fragment::MapType& ent) const
{
    Fragment::MapType::const_iterator I = ent.find("name");
    if ((I == ent.end()) || !I->second.IsString()) {
        return error(op, "Entity to be created has no name");
    }

#if defined(__GNUC__) && __GNUC__ < 3
    if (!I->second.AsString().substr(0,5).compare("admin")) {
        return error(op, "Entity to be created cannot start with admin");
    }
#else
    if (!I->second.AsString().compare(0,5,"admin")) {
        return error(op, "Entity to be created cannot start with admin");
    }
#endif

    I = ent.find("parents");
    if ((I == ent.end()) || !I->second.IsList()) {
        return error(op, "You cannot create a character with no type.");
    }
    const Fragment::ListType & parents = I->second.AsList();
    if (parents.empty() || !parents.front().IsString()) {
        return error(op, "You cannot create a character with non-string type.");
    }
    const std::string& type = parents.front().AsString(); 
    if (Player::playableTypes.find(type) == Player::playableTypes.end()) {
        return error(op, "You cannot create a character of this type.");
    }
    return OpVector();
}
