#pragma once

#include <Glacier/Resource/ZRuntimeResourceID.h>

#include "Common.h"

namespace MR
{
	class IAnimSource;
}

class ZAnimationResource
{
public:
	ZAnimationResource* m_pNext;
	ZAnimationResource* m_pPrevious;
	ZRuntimeResourceID m_ridResource;
	PAD(0x38);
	MR::IAnimSource* m_pAnimationSource;
};
