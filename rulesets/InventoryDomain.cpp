/*
 Copyright (C) 2014 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "InventoryDomain.h"

#include "common/const.h"

InventoryDomain::InventoryDomain(LocatedEntity& entity) :
        Domain(entity)
{
}

InventoryDomain::~InventoryDomain()
{
}

float InventoryDomain::constrainHeight(LocatedEntity * parent, const Point3D & pos, const std::string & mode)
{
    //Nothing can move
    return 0.0f;
}

void InventoryDomain::tick(double t)
{

}

void InventoryDomain::lookAtEntity(const LocatedEntity& observingEntity, const LocatedEntity& observedEntity, const Operation & originalLookOp, OpVector& res) const
{
    //Nothing can be seen
}

bool InventoryDomain::isEntityVisibleFor(const LocatedEntity& observingEntity, const LocatedEntity& observedEntity) const
{
    //If the observing entity is the same as the one the domain belongs to it can see everything.
    if (&observingEntity == &m_entity) {
        return true;
    }

    //Entities can only be seen by outside observers if they are outfitted or wielded.
    const OutfitProperty* outfitProperty = m_entity.getPropertyClass<OutfitProperty>("outfit");
    if (outfitProperty) {
        for (auto& entry : outfitProperty->data()) {
            auto outfittedEntity = entry.second.get();
            if (outfittedEntity && outfittedEntity == &observedEntity) {
                return true;
            }
        }
    }
    //If the entity isn't outfitted, perhaps it's wielded?
    const EntityProperty* rightHandWieldProperty = m_entity.getPropertyClass<EntityProperty>("right_hand_wield");
    if (rightHandWieldProperty) {
        auto entity = rightHandWieldProperty->data().get();
        if (entity && entity == &observedEntity) {
            return true;
        }
    }

    return false;
}

void InventoryDomain::processVisibilityForMovedEntity(const LocatedEntity& moved_entity, const Location& old_loc, OpVector & res)
{
//Nothing can be seen
}

void InventoryDomain::processDisappearanceOfEntity(const LocatedEntity& moved_entity, const Location& old_loc, OpVector & res)
{
//Nothing can be seen
}

float InventoryDomain::checkCollision(LocatedEntity& entity, CollisionData& collisionData)
{
//Nothing can move
    return consts::move_tick;
}
