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

#ifndef RULESETS_AREA_PROPERTY_H
#define RULESETS_AREA_PROPERTY_H

#include "LineProperty.h"

typedef WFMath::Point<2> Corner;
typedef std::vector<WFMath::Point<2> > CornerList;

/// \brief Class to handle Entity terrain property
/// \ingroup PropertyClasses
class AreaProperty : public PropertyBase {
  protected:
    /// \brief Property data for the line that defines the outline of the area
    CornerList m_data;

    /// \breif Optional terrain layer property.
    int* m_layer;
  public:
    explicit AreaProperty();
    virtual ~AreaProperty();

    const CornerList & line() const { return m_data; }

    virtual bool get(Atlas::Message::Element & val) const;
    virtual void set(const Atlas::Message::Element & val);
    virtual void add(const std::string & key, Atlas::Message::MapType & map) const;
};

#endif // RULESETS_AREA_PROPERTY_H
