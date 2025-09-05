#pragma once

#include "NetworkDef.h"

class ZNetworkResource
{
public:
	PAD(0x4);
	MR::NetworkDef* m_pNetworkDef;
	ZRuntimeResourceID m_ridResource;
	TArray<MR::IAnimSource*> m_animationLibrary;
};
