#pragma once

#include "ZPhysicsBaseEntity.h"
#include "ICollisionShapeListener.h"
#include "IStaticPhysics.h"
#include "../Event/ZEvent.h"

class ZStaticPhysicsAspect : public ZPhysicsBaseEntity
{
public:
	PAD(0x18);
	IPhysicsObject* m_pPhysicsObject; // 0x24
};
