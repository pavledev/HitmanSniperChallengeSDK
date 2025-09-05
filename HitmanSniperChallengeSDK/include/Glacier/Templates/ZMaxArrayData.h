#pragma once

#include "TAlignedType.h"

template <typename TElement, int TCapacity>
struct ZMaxArrayData
{
    unsigned int m_nSize;

    union
    {
        unsigned char m_data[TCapacity * sizeof(TElement)];
        TAlignedType<alignof(TElement)> alignDummy;
    };
};
