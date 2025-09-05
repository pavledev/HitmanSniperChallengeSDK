#pragma once

#include "ZObjectRef.h"

class ZVariant;

class HitmanSniperChallengeSDK_API ZVariantRef : public ZObjectRef
{
public:
	ZVariantRef();
	ZVariantRef(const ZVariant& rhs);
	ZVariantRef(void* pObject, STypeID* const typeID);
};
