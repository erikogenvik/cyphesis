// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2000,2001 Alistair Riddoch

#ifndef RULESETS_ENTITY_FACTORY_H
#define RULESETS_ENTITY_FACTORY_H

#include <string>
#include <map>

class Thing;
class FactoryBase;

typedef std::map<std::string, FactoryBase *> fdict_t;

class EntityFactory {
    EntityFactory();
    static EntityFactory * m_instance;

    fdict_t factories;
  public:
    static EntityFactory * instance() {
        if (m_instance == NULL) {
            m_instance = new EntityFactory();
        }
        return m_instance;
    }
    static EntityFactory * del() {
        if (m_instance != NULL) {
            delete m_instance;
        }
    }
    Thing * newThing(const std::string &, const Atlas::Message::Object &, const edict_t &);
    void flushFactories();
};

#endif // ENTITY_FACTORY_H
