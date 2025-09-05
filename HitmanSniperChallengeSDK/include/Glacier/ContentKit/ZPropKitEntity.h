#pragma once

#include "ZContentKitEntity.h"
#include "../Resource/ZRuntimeResourceID.h"

class HitmanSniperChallengeSDK_API ZPropKitEntity : public ZContentKitEntity
{
public:
	ZRuntimeResourceID GetLargeIconRID() const;

private:
	ZRuntimeResourceID m_sHiResNotebookImage;
	bool m_bIsNewPickup;
};
