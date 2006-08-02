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

#error This file has been removed from the build
#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef COMMON_CUSTOM_OP_IMPL_H
#define COMMON_CUSTOM_OP_IMPL_H

#include "common/CustomOp.h"

#include <list>

template<class ParentOpData, class ParentProxy>
CustomOpData<ParentOpData, ParentProxy>::~CustomOpData()
{
}
// <ParentOpData, ParentProxy>

template<class ParentOpData, class ParentProxy>
CustomOpData<ParentOpData, ParentProxy> * CustomOpData<ParentOpData, ParentProxy>::copy() const
{
    CustomOpData<ParentOpData, ParentProxy> * copied = CustomOpData<ParentOpData, ParentProxy>::alloc();
    *copied = *this;
    copied->m_refCount = 0;
    return copied;
}

template<class ParentOpData, class ParentProxy>
bool CustomOpData<ParentOpData, ParentProxy>::instanceOf(int classNo) const
{
    if(class_no == classNo) return true;
    return ParentOpData::instanceOf(classNo);
}

// template<class ParentOpData, class ParentProxy>
// const char * const *CustomOpData<ParentOpData, ParentProxy>::class_name = 0;
template<class ParentOpData, class ParentProxy>
int CustomOpData<ParentOpData, ParentProxy>::class_no;

//freelist related methods specific to this class
template<class ParentOpData, class ParentProxy>
CustomOpData<ParentOpData, ParentProxy> *CustomOpData<ParentOpData, ParentProxy>::defaults_CustomOpData = 0;

template<class ParentOpData, class ParentProxy>
CustomOpData<ParentOpData, ParentProxy> *CustomOpData<ParentOpData, ParentProxy>::begin_CustomOpData = 0;

template<class ParentOpData, class ParentProxy>
CustomOpData<ParentOpData, ParentProxy> *CustomOpData<ParentOpData, ParentProxy>::alloc()
{
    if(begin_CustomOpData) {
        CustomOpData *res = begin_CustomOpData;
        assert( res->m_refCount == 0 );
        res->m_attrFlags = 0;
        res->m_attributes.clear();
        begin_CustomOpData = (CustomOpData *)begin_CustomOpData->m_next;
        return res;
    }
    return new CustomOpData<ParentOpData, ParentProxy>(CustomOpData<ParentOpData, ParentProxy>::getDefaultObjectInstance());
}

template<class ParentOpData, class ParentProxy>
void CustomOpData<ParentOpData, ParentProxy>::free()
{
    this->m_next = begin_CustomOpData;
    begin_CustomOpData = this;
}


template<class ParentOpData, class ParentProxy>
CustomOpData<ParentOpData, ParentProxy> *CustomOpData<ParentOpData, ParentProxy>::getDefaultObjectInstance()
{
    if (defaults_CustomOpData == 0) {
        defaults_CustomOpData = new CustomOpData;
        defaults_CustomOpData->attr_objtype = "op";
        defaults_CustomOpData->attr_serialno = 0;
        defaults_CustomOpData->attr_refno = 0;
        defaults_CustomOpData->attr_seconds = 0.0;
        defaults_CustomOpData->attr_future_seconds = 0.0;
        defaults_CustomOpData->attr_stamp = 0.0;
        defaults_CustomOpData->attr_parents = std::list<std::string>(1, ParentProxy::name());
    }
    return defaults_CustomOpData;
}

template<class ParentOpData, class ParentProxy>
CustomOpData<ParentOpData, ParentProxy> *CustomOpData<ParentOpData, ParentProxy>::getDefaultObject()
{
    return CustomOpData<ParentOpData, ParentProxy>::getDefaultObjectInstance();
}

#endif // COMMON_CUSTOM_OP_IMPL_H
