// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2005 Alistair Riddoch
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

#include "TestWorld.h"

#include "rulesets/World.h"
#include "rulesets/Python_API.h"

#include "server/Ruleset.h"
#include "server/EntityBuilder.h"
#include "server/EntityFactory.h"

#include "common/Inheritance.h"

#include <Atlas/Objects/Anonymous.h>

#include <cassert>

using Atlas::Message::MapType;
using Atlas::Objects::Entity::Anonymous;
using Atlas::Objects::Root;

class ExposedRuleset : public Ruleset {
  public:
    ExposedRuleset(EntityBuilder * eb) : Ruleset(eb) { }

    void getRulesFromFiles(std::map<std::string, Root> & rules) {
        Ruleset::getRulesFromFiles(rules);
    }
    void populateEntityFactory(const std::string & class_name,
                               EntityKit * factory,
                               const MapType & class_desc) {
        Ruleset::populateEntityFactory(class_name, factory, class_desc);
    }
    int installEntityClass(const std::string & class_name,
                           const std::string & parent,
                           const Root & class_desc,
                           std::string & dependent,
                           std::string & reason) {
        return Ruleset::installEntityClass(class_name, parent, class_desc,
                                            dependent, reason);
    }
    int installOpDefinition(const std::string & op_def_name,
                            const std::string & parent,
                            const Root & op_def_desc,
                            std::string & dependent,
                            std::string & reason) {
        return Ruleset::installOpDefinition(op_def_name, parent, op_def_desc,
                                            dependent, reason);
    }

};

class ExposedEntityBuilder : public EntityBuilder {
  public:
    explicit ExposedEntityBuilder(BaseWorld & w) : EntityBuilder(w) {
        m_instance = this;
    }

    EntityKit * getNewFactory(const std::string & clss) {
        return EntityBuilder::getNewFactory(clss);
    }

    const FactoryDict & factoryDict() const { return m_entityFactories; }

};

int main(int argc, char ** argv)
{
    loadConfig(argc, argv);

    database_flag = false;

    init_python_api();

    int ret;

    {
        World e("1", 1);
        TestWorld test_world(e);
        Anonymous attributes;

        EntityBuilder::init(test_world);
        Ruleset::init();

        assert(Ruleset::instance() != 0);

        assert(EntityBuilder::instance() != 0);

        assert(EntityBuilder::instance()->newEntity("1", 1, "world", attributes) == 0);
        assert(EntityBuilder::instance()->newEntity("1", 1, "nonexistant", attributes) == 0);
        assert(EntityBuilder::instance()->newEntity("1", 1, "thing", attributes) != 0);

        Ruleset::del();
        assert(Ruleset::instance() == 0);
        EntityBuilder::del();
        assert(EntityBuilder::instance() == 0);
        Inheritance::clear();
    }

    {
        World e("1", 1);
        TestWorld test_world(e);
        Anonymous attributes;
        Atlas::Message::Element val;

        EntityBuilder::init(test_world);
        Ruleset::init();

        assert(Ruleset::instance() != 0);

        Entity * test_ent = EntityBuilder::instance()->newEntity("1", 1, "thing", attributes);
        assert(test_ent != 0);
        assert(!test_ent->getAttr("funky", val));
        assert(val.isNone());

        attributes->setAttr("funky", "true");

        test_ent = EntityBuilder::instance()->newEntity("1", 1, "thing", attributes);
        assert(test_ent != 0);
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        Ruleset::del();
        assert(Ruleset::instance() == 0);
        EntityBuilder::del();
        assert(EntityBuilder::instance() == 0);
        Inheritance::clear();
    }

    {
        // Create a test world.
        World e("1", 1);
        TestWorld test_world(e);
        Atlas::Message::Element val;

        // Instance of EntityBuilder with all protected methods exposed
        // for testing
        ExposedEntityBuilder * entity_factory = new ExposedEntityBuilder(test_world);
        // Instance of Ruleset with all protected methods exposed
        // for testing
        EntityBuilder * test_eb = EntityBuilder::instance();
        assert(test_eb == entity_factory);
        ExposedRuleset test_ruleset(test_eb);

        // Attributes for test entities being created
        Anonymous attributes;

        // Create an entity which is an instance of one of the core classes
        Entity * test_ent = test_eb->newEntity("1", 1, "thing", attributes);
        assert(test_ent != 0);
        // Check the created entity does not have the attribute values we
        // will be testing later
        assert(!test_ent->getAttr("funky", val));
        assert(val.isNone());

        // Set a test attribute
        attributes->setAttr("funky", "true");

        // Create another entity, and check that it has picked up the new
        // attribute value
        test_ent = test_eb->newEntity("1", 1, "thing", attributes);
        assert(test_ent != 0);
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Check that creating an entity of a type we know we have not yet
        // installed results in a null pointer.
        assert(test_eb->newEntity("1", 1, "custom_type", attributes) == 0);

        // Set up a type description for a new type, and install it.
        {
            Root custom_type_description;
            MapType attrs;
            MapType test_custom_type_attr;
            test_custom_type_attr["default"] = "test_value";
            test_custom_type_attr["visibility"] = "public";
            attrs["test_custom_type_attr"] = test_custom_type_attr;
            custom_type_description->setAttr("attributes", attrs);
            custom_type_description->setId("custom_type");
            custom_type_description->setParents(std::list<std::string>(1, "thing"));

            std::string dependent, reason;
            ret = test_ruleset.installEntityClass("custom_type", "thing", custom_type_description, dependent, reason);

            assert(ret == 0);
            assert(dependent.empty());
            assert(reason.empty());
        }

        // Check that the factory dictionary now contains a factory for
        // the custom type we just installed.
        EntityKit * custom_type_factory = test_eb->getClassFactory("custom_type");
        assert(custom_type_factory != 0);

        // Check the factory has the attributes we described on the custom
        // type.
        MapType::const_iterator J = custom_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        // Create an instance of our custom type, ensuring that it works.
        test_ent = test_eb->newEntity("1", 1, "custom_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute described when the
        // custom type was installed.
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        // Check that creating an entity of a type we know we have not yet
        // installed results in a null pointer.
        assert(test_eb->newEntity("1", 1, "custom_inherited_type", attributes) == 0);

        // Set up a type description for a second new type which inherits
        // from the first, and install it.
        {
            Root custom_inherited_type_description;
            MapType attrs;
            MapType test_custom_type_attr;
            test_custom_type_attr["default"] = "test_inherited_value";
            test_custom_type_attr["visibility"] = "public";
            attrs["test_custom_inherited_type_attr"] = test_custom_type_attr;
            custom_inherited_type_description->setAttr("attributes", attrs);
            custom_inherited_type_description->setId("custom_inherited_type");
            custom_inherited_type_description->setParents(std::list<std::string>(1, "custom_type"));

            std::string dependent, reason;
            ret = test_ruleset.installEntityClass("custom_inherited_type", "custom_type", custom_inherited_type_description, dependent, reason);

            assert(ret == 0);
            assert(dependent.empty());
            assert(reason.empty());
        }

        // Check that the factory dictionary does contain the factory for
        // the second newly installed type
        EntityKit * custom_inherited_type_factory = test_eb->getClassFactory("custom_inherited_type");
        assert(custom_inherited_type_factory != 0);

        // Check that the factory has inherited the attributes from the
        // first custom type
        J = custom_inherited_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        // Check that the factory has the attributes specified when installing
        // it
        J = custom_inherited_type_factory->m_attributes.find("test_custom_inherited_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_inherited_value");

        // Creat an instance of the second custom type, ensuring it works.
        test_ent = test_eb->newEntity("1", 1, "custom_inherited_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the instance of the second custom type has the attribute
        // described when the first custom type was installed.
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute described when the
        // second custom type was installed
        assert(test_ent->getAttr("test_custom_inherited_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_inherited_value");

        // FIXME TODO Modify a type, and ensure attribute propagate to inherited types.

        // Make sure than attempting to modify a non-existant type fails
        {
            Anonymous nonexistant_description;
            MapType attrs;
            MapType test_custom_type_attr;

            test_custom_type_attr["default"] = "no_value";
            test_custom_type_attr["visibility"] = "public";
            attrs["no_custom_type_attr"] = test_custom_type_attr;

            nonexistant_description->setId("nonexistant");
            nonexistant_description->setAttr("attributes", attrs);

            ret = test_ruleset.modifyRule("nonexistant", nonexistant_description);

            assert(ret != 0);
        }

        // Modify the second custom type removing its custom attribute
        {
            Anonymous new_custom_inherited_type_description;
            new_custom_inherited_type_description->setId("custom_inherited_type");
            new_custom_inherited_type_description->setAttr("attributes", MapType());

            // No parents
            ret = test_ruleset.modifyRule("custom_inherited_type",
                                          new_custom_inherited_type_description);
            assert(ret != 0);

            // empty parents
            new_custom_inherited_type_description->setParents(std::list<std::string>());

            ret = test_ruleset.modifyRule("custom_inherited_type",
                                          new_custom_inherited_type_description);
            assert(ret != 0);

            // wrong parents
            new_custom_inherited_type_description->setParents(std::list<std::string>(1, "wrong_parent"));

            ret = test_ruleset.modifyRule("custom_inherited_type",
                                          new_custom_inherited_type_description);
            assert(ret != 0);

            new_custom_inherited_type_description->setParents(std::list<std::string>(1, "custom_type"));

            ret = test_ruleset.modifyRule("custom_inherited_type",
                                          new_custom_inherited_type_description);

            assert(ret == 0);
        }

        // Check that the factory dictionary does contain the factory for
        // the second newly installed type
        custom_inherited_type_factory = test_eb->getClassFactory("custom_inherited_type");
        assert(custom_inherited_type_factory != 0);

        // Check that the factory has inherited the attributes from the
        // first custom type
        J = custom_inherited_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        // Check that the factory no longer has the attributes we removed
        J = custom_inherited_type_factory->m_attributes.find("test_custom_inherited_type_attr");
        assert(J == custom_inherited_type_factory->m_attributes.end());

        // Creat an instance of the second custom type, ensuring it works.
        test_ent = test_eb->newEntity("1", 1, "custom_inherited_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Make sure test nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant", val));
        // Make sure nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant_attribute", val));

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the instance of the second custom type has the attribute
        // described when the first custom type was installed.
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute described when the
        // second custom type was installed
        assert(!test_ent->getAttr("test_custom_inherited_type_attr", val));

        // Modify the first custom type removing its custom attribute
        {
            Anonymous new_custom_type_description;
            new_custom_type_description->setId("custom_type");
            new_custom_type_description->setAttr("attributes", MapType());
            new_custom_type_description->setParents(std::list<std::string>(1, "thing"));

            ret = test_ruleset.modifyRule("custom_type", new_custom_type_description);

            assert(ret == 0);
        }

        // Check that the factory dictionary now contains a factory for
        // the custom type we just installed.
        custom_type_factory = test_eb->getClassFactory("custom_type");
        assert(custom_type_factory != 0);

        // Check the factory has the attributes we described on the custom
        // type.
        J = custom_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J == custom_type_factory->m_attributes.end());

        // Create an instance of our custom type, ensuring that it works.
        test_ent = test_eb->newEntity("1", 1, "custom_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type no longer has the custom attribute
        assert(!test_ent->getAttr("test_custom_type_attr", val));

        // Check that the factory dictionary does contain the factory for
        // the second newly installed type
        custom_inherited_type_factory = test_eb->getClassFactory("custom_inherited_type");
        assert(custom_inherited_type_factory != 0);

        // Check that the factory no longer has inherited the attributes
        // from the first custom type which we removed
        J = custom_inherited_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J == custom_inherited_type_factory->m_attributes.end());

        // Check that the factory no longer has the attributes we removed
        J = custom_inherited_type_factory->m_attributes.find("test_custom_inherited_type_attr");
        assert(J == custom_inherited_type_factory->m_attributes.end());

        // Creat an instance of the second custom type, ensuring it works.
        test_ent = test_eb->newEntity("1", 1, "custom_inherited_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Make sure test nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant", val));
        // Make sure nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant_attribute", val));

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the instance of the second custom type has the attribute
        // described when the first custom type was installed.
        assert(!test_ent->getAttr("test_custom_type_attr", val));

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute described when the
        // second custom type was installed
        assert(!test_ent->getAttr("test_custom_inherited_type_attr", val));

        // Add more custom attributes to the first type
        {
            Anonymous new_custom_type_description;
            MapType attrs;
            MapType test_custom_type_attr;

            test_custom_type_attr["default"] = "test_value";
            test_custom_type_attr["visibility"] = "public";
            attrs["test_custom_type_attr"] = test_custom_type_attr;

            MapType new_custom_type_attr;

            new_custom_type_attr["default"] = "new_value";
            new_custom_type_attr["visibility"] = "public";
            attrs["new_custom_type_attr"] = new_custom_type_attr;

            new_custom_type_description->setId("custom_type");
            new_custom_type_description->setAttr("attributes", attrs);
            new_custom_type_description->setParents(std::list<std::string>(1, "thing"));

            ret = test_ruleset.modifyRule("custom_type", new_custom_type_description);

            assert(ret == 0);
            
        }

        // Check that the factory dictionary now contains a factory for
        // the custom type we just installed.
        custom_type_factory = test_eb->getClassFactory("custom_type");

        // Check the factory has the attributes we described on the custom
        // type.
        J = custom_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        J = custom_type_factory->m_attributes.find("new_custom_type_attr");
        assert(J != custom_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "new_value");

        // Create an instance of our custom type, ensuring that it works.
        test_ent = test_eb->newEntity("1", 1, "custom_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type now has the custom attributes
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        assert(test_ent->getAttr("new_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "new_value");

        // Check that the factory dictionary does contain the factory for
        // the second newly installed type
        custom_inherited_type_factory = test_eb->getClassFactory("custom_inherited_type");
        assert(custom_inherited_type_factory != 0);

        // Check that the factory now has inherited the attributes
        // from the first custom type
        J = custom_inherited_type_factory->m_attributes.find("test_custom_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "test_value");

        J = custom_inherited_type_factory->m_attributes.find("new_custom_type_attr");
        assert(J != custom_inherited_type_factory->m_attributes.end());
        assert(J->second.isString());
        assert(J->second.String() == "new_value");

        // Check that the factory no longer has the attributes we removed
        J = custom_inherited_type_factory->m_attributes.find("test_custom_inherited_type_attr");
        assert(J == custom_inherited_type_factory->m_attributes.end());

        // Creat an instance of the second custom type, ensuring it works.
        test_ent = test_eb->newEntity("1", 1, "custom_inherited_type", attributes);
        assert(test_ent != 0);

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Make sure test nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant", val));
        // Make sure nonexistant attribute isn't present
        assert(!test_ent->getAttr("nonexistant_attribute", val));

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type has the attribute we passed in when creating
        // the instance.
        assert(test_ent->getAttr("funky", val));
        assert(val.isString());
        assert(val.String() == "true");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the instance of the second custom type has the attribute
        // described when the first custom type was installed.
        assert(test_ent->getAttr("test_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "test_value");

        assert(test_ent->getAttr("new_custom_type_attr", val));
        assert(val.isString());
        assert(val.String() == "new_value");

        // Reset val.
        val = Atlas::Message::Element();
        assert(val.isNone());

        // Check the custom type no longer has the attribute described when the
        // second custom type was installed
        assert(!test_ent->getAttr("test_custom_inherited_type_attr", val));

    }
}
