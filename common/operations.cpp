// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2000-2004 Alistair Riddoch

#include "Chop.h"
#include "Cut.h"
#include "Eat.h"
#include "Burn.h"
#include "Generic.h"
#include "Nourish.h"
#include "Setup.h"
#include "Tick.h"
#include "Unseen.h"
#include "Use.h"
#include "Wield.h"

namespace Atlas { namespace Objects { namespace Operation {

Chop::Chop() : Action("", "chop")
{
}

Chop::Chop(const char * id, const char * parent) : Action(id, parent)
{
}

Chop::~Chop()
{
}

Chop Chop::Class()
{
    Chop value("chop", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

Cut::Cut() : Action("", "cut")
{
}

Cut::Cut(const char * id, const char * parent) : Action(id, parent)
{
}

Cut::~Cut() { }

Cut Cut::Class()
{
    Cut value("cut", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

Eat::Eat() : Action("", "eat")
{
}

Eat::Eat(const char * id, const char * parent) : Action(id, parent)
{
}

Eat::~Eat()
{
}

Eat Eat::Class()
{
    Eat value("eat", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

Burn::Burn() : Action("", "burn")
{
}

Burn::Burn(const char * id, const char * parent) : Action(id, parent)
{
}

Burn::~Burn()
{
}

Burn Burn::Class()
{
    Burn value("burn", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

Generic::Generic(const std::string & p) : RootOperation("", p.c_str())
{
}

Generic::~Generic()
{
}

Generic Generic::Class(const std::string & p)
{
    Generic value(p);
    value.setObjtype(std::string("op_definition"));
    return value;
}

Nourish::Nourish() : Action("", "nourish")
{
}

Nourish::Nourish(const char * id, const char * parent) : Action(id, parent)
{
}

Nourish::~Nourish()
{
}

Nourish Nourish::Class()
{
    Nourish value("nourish", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

Setup::Setup() : RootOperation("", "setup")
{
}

Setup::Setup(const char * id, const char * parent) : RootOperation(id, parent)
{
}

Setup::~Setup()
{
}

Setup Setup::Class()
{
    Setup value("setup", "root_operation");
    value.setObjtype(std::string("op_definition"));
    return value;
}

Tick::Tick() : RootOperation("", "tick")
{
}

Tick::Tick(const char * id, const char * parent) : RootOperation(id, parent)
{
}

Tick::~Tick()
{
}

Tick Tick::Class()
{
    Tick value("tick", "root_operation");
    value.setObjtype(std::string("op_definition"));
    return value;
}

Unseen::Unseen() : Perception("", "unseen")
{
}

Unseen::Unseen(const char * id, const char * parent) : Perception(id, parent)
{
}

Unseen::~Unseen()
{
}

Unseen Unseen::Class()
{
    Unseen value("unseen", "perception");
    value.setObjtype(std::string("op_definition"));
    return value;
}

Use::Use() : Action("", "use")
{
}

Use::Use(const char * id, const char * parent) : Action(id, parent)
{
}

Use::~Use()
{
}

Use Use::Class()
{
    Use value("use", "action");
    value.setObjtype(std::string("op_definition"));
    return value;
}

Wield::Wield() : Set("", "wield")
{
}

Wield::Wield(const char * id, const char * parent) : Set(id, parent)
{
}

Wield::~Wield()
{
}

Wield Wield::Class()
{
    Wield value("wield", "set");
    value.setObjtype(std::string("op_definition"));
    return value;
}

} } }
