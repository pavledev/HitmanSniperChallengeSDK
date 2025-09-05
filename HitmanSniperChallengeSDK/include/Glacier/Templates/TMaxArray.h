#pragma once

#include "TMaxArrayBase.h"
#include "ZMaxArrayData.h"

template <typename TElement, int TCapacity>
class TMaxArray : public TMaxArrayBase<TElement, ZMaxArrayData<TElement, TCapacity>>
{
};
