// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2007 Alistair Riddoch
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

#ifdef NDEBUG
#undef NDEBUG
#endif
#ifndef DEBUG
#define DEBUG
#endif

#include "EntityExerciser.h"

#include "rulesets/LocatedEntity.h"

#include "rulesets/AtlasProperties.h"
#include "rulesets/Script.h"

#include <cassert>

using Atlas::Message::MapType;
using Atlas::Message::ListType;

void runCoverageTest()
{
    LocatedEntityTest * le = new LocatedEntityTest("1", 1);

    le->setScript(new Script());
    // Installing a second one should delete the first.
    le->setScript(new Script());

    le->onContainered();
    le->onUpdated();

    EntityExerciser ee(*le);
    // Throw an op of every type at the entity
    ee.runOperations();

    // Subscribe the entity to every class of op
    std::set<std::string> opNames;
    ee.addAllOperations(opNames);

    std::set<std::string> attrNames;
    attrNames.insert("id");

    // Make sure we have all the default attributes
    assert(ee.checkAttributes(attrNames));

    attrNames.insert("test_int");
    attrNames.insert("test_float");
    attrNames.insert("test_list_string");
    attrNames.insert("test_list_int");
    attrNames.insert("test_list_float");
    attrNames.insert("test_map_string");
    attrNames.insert("test_map_int");
    attrNames.insert("test_map_float");

    // Make sure we don't have the test attributes yet
    assert(!ee.checkAttributes(attrNames));

    // Add the test attributes
    le->setAttr("test_int", 1);
    le->setAttr("test_float", 1.f);
    le->setAttr("test_list_string", "test_value");
    le->setAttr("test_list_int", ListType(1, 1));
    le->setAttr("test_list_float", ListType(1, 1.f));
    le->setAttr("test_map_string", ListType(1, "test_value"));
    MapType test_map;
    test_map["test_key"] = 1;
    le->setAttr("test_map_int", test_map);
    test_map["test_key"] = 1.f;
    le->setAttr("test_map_float", test_map);
    test_map["test_key"] = "test_value";
    le->setAttr("test_map_string", test_map);
    
    // Make sure we have the test attributes now
    assert(ee.checkAttributes(attrNames));

    MapType entityAsAtlas;

    le->merge(entityAsAtlas);

    // Throw an op of every type at the entity again now it is subscribed,
    // and full of data.
    ee.runOperations();

    delete le;
}

int main()
{
    // Test constructor
    {
        new LocatedEntityTest("1", 1);
    }

    // Test destructor
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);
        delete e;
    }

    // Test checkRef()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);

        assert(e->checkRef() == 0);
        delete e;
    }

    // Test incRef()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);

        e->incRef();
        assert(e->checkRef() == 1);
        delete e;
    }

    // Test decRef()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);

        e->incRef();
        e->decRef();
        assert(e->checkRef() == 0);
        delete e;
    }

    // Test decRef()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);

        e->decRef();
        // Enitity deleted - verified as not leaked
    }

    // Test setAttr()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);

        e->setAttr("foo", "bar");
        e->decRef();
    }
    
    // Test setAttr()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);

        e->setAttr("foo", "bar");
        e->setAttr("foo", 23);
        e->decRef();
    }

    // Test getAttr()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);
        Atlas::Message::Element val;

        bool ret = e->getAttr("foo", val);
        assert(ret == false);
        assert(val.isNone());
        e->decRef();
    }

    // Test getAttr()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);
        Atlas::Message::Element val;

        e->setAttr("foo", "bar");
        bool ret = e->getAttr("foo", val);
        assert(ret == true);
        assert(val.isString());
        e->decRef();
    }

    // Test hasAttr()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);
        Atlas::Message::Element val;

        bool ret = e->hasAttr("foo");
        assert(ret == false);
        e->decRef();
    }

    // Test hasAttr()
    {
        LocatedEntityTest * e = new LocatedEntityTest("1", 1);
        Atlas::Message::Element val;

        e->setAttr("foo", "bar");
        bool ret = e->hasAttr("foo");
        assert(ret == true);
        e->decRef();
    }

    runCoverageTest();

    return 0;
}

// stubs

Script noScript;

namespace Atlas { namespace Objects { namespace Operation {
int EAT_NO = -1;
int NOURISH_NO = -1;
int SETUP_NO = -1;
int TICK_NO = -1;
} } }

Router::Router(const std::string & id, long intId) : m_id(id),
                                                             m_intId(intId)
{
}

Router::~Router()
{
}

void Router::addToMessage(Atlas::Message::MapType & omap) const
{
}

void Router::addToEntity(const Atlas::Objects::Entity::RootEntity & ent) const
{
}

Script::Script()
{
}

/// \brief Script destructor
Script::~Script()
{
}

bool Script::operation(const std::string & opname,
                       const Atlas::Objects::Operation::RootOperation & op,
                       OpVector & res)
{
   return false;
}

void Script::hook(const std::string & function, LocatedEntity * entity)
{
}

Location::Location() : m_loc(0)
{
}

TypeNode::TypeNode() : m_parent(0)
{
}

IdProperty::IdProperty(const std::string & data) : PropertyBase(per_ephem),
                                                   m_data(data)
{
}

bool IdProperty::get(Atlas::Message::Element & e) const
{
    return true;
}

void IdProperty::set(const Atlas::Message::Element & e)
{
}

void IdProperty::add(const std::string & key,
                     Atlas::Message::MapType & ent) const
{
}

void IdProperty::add(const std::string & key,
                     const Atlas::Objects::Entity::RootEntity & ent) const
{
}

PropertyBase::PropertyBase(unsigned int flags) : m_flags(flags)
{
}

PropertyBase::~PropertyBase()
{
}

void PropertyBase::install(Entity *)
{
}

void PropertyBase::apply(Entity *)
{
}

void PropertyBase::add(const std::string & s,
                       Atlas::Message::MapType & ent) const
{
}

void PropertyBase::add(const std::string & s,
                       const Atlas::Objects::Entity::RootEntity & ent) const
{
}

SoftProperty::SoftProperty()
{
}

SoftProperty::SoftProperty(const Atlas::Message::Element & data) :
              PropertyBase(0), m_data(data)
{
}

bool SoftProperty::get(Atlas::Message::Element & val) const
{
    val = m_data;
    return true;
}

void SoftProperty::set(const Atlas::Message::Element & val)
{
}

ContainsProperty::ContainsProperty(LocatedEntitySet & data) :
      PropertyBase(per_ephem), m_data(data)
{
}

bool ContainsProperty::get(Atlas::Message::Element & e) const
{
    return true;
}

void ContainsProperty::set(const Atlas::Message::Element & e)
{
}

void ContainsProperty::add(const std::string & s,
                           const Atlas::Objects::Entity::RootEntity & ent) const
{
}
