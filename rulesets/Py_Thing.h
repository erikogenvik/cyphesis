// Cyphesis Online RPG Server and AI Engine
// Copyright (C) 2000 Alistair Riddoch
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

#ifndef RULESETS_PY_THING_H
#define RULESETS_PY_THING_H

#include <Python.h>

class Character;
class Entity;
class LocatedEntity;

/// \brief Wrapper for Entity in Python
/// \ingroup PythonWrappers
typedef struct {
    PyObject_HEAD
    /// \brief Storage for additional attributes
    PyObject * Entity_attr;  // Attributes dictionary
    /// \brief Entity object handled by this wrapper
    union {
        LocatedEntity * l;
        Entity * e;
        Character * c;
    } m_entity;
} PyEntity;

extern PyTypeObject PyLocatedEntity_Type;
extern PyTypeObject PyEntity_Type;
extern PyTypeObject PyCharacter_Type;

#define PyLocatedEntity_Check(_o) ((_o)->ob_type == &PyLocatedEntity_Type)
#define PyEntity_Check(_o) ((_o)->ob_type == &PyEntity_Type)
#define PyCharacter_Check(_o) ((_o)->ob_type == &PyCharacter_Type)

PyObject * wrapEntity(LocatedEntity * entity);
PyEntity * newPyLocatedEntity();
PyEntity * newPyEntity();
PyEntity * newPyCharacter();

#endif // RULESETS_PY_THING_H
