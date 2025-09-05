#pragma once

#include "IVariationInstance.h"
#include "../Templates/TResourcePtr.h"
#include "../Templates/THashMap.h"
#include "../Templates/TDefaultHashMapPolicy.h"

class ZAnimationResource;

class ZAnimationDatabase : public IVariationInstance
{
public:
	class ZAnimDatabaseEntry
	{
	public:
		TResourcePtr<ZAnimationResource> m_pAnimResource;
	};

	THashMap<ZString, ZAnimationDatabase::ZAnimDatabaseEntry, TDefaultHashMapPolicy<ZString>> m_animationMap;
};
