#pragma once

#include "../Entity/ZEntityImpl.h"
#include "../Templates/TEntityRef.h"
#include "../Math/SMatrix.h"

#include <Common.h>

class ZBoundedEntity;
struct float4;

class HitmanSniperChallengeSDK_API ZSpatialEntity : public ZEntityImpl
{
public:
	enum ERoomBehaviour
	{
		ROOM_STATIC = 0,
		ROOM_DYNAMIC = 1
	};

	virtual void Unk16() const = 0;
	virtual void Unk17() const = 0;
	virtual void Unk18() const = 0;
	virtual ZSpatialEntity* GetScene() const = 0;
	virtual void CalculateBounds(float4& vMin_, float4& vMax_, const unsigned int nIncludeFlags, const unsigned int nExcludeFlags) = 0;

	SMatrix GetObjectToWorldMatrix() const;
	void SetObjectToWorldMatrix(const SMatrix& mObjectToWorld);
	float4 GetWorldPosition() const;
	void SetWorldPosition(const float4& vWorldPosition);
	SMatrix GetViewMatrix() const;
	ZSpatialEntity* GetTransformParent() const;
	ZSpatialEntity* GetTransformChildren() const;

private:
	ZSpatialEntity* m_pTransformParent;
	ZSpatialEntity* m_pTransformChildren;
	ZSpatialEntity* m_pTransformNext;
	ZSpatialEntity* m_pTransformPrev;
	PAD(0x58);
};
