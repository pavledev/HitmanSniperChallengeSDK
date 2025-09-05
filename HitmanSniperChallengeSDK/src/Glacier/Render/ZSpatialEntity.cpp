#include "Glacier/Render/ZSpatialEntity.h"
#include "Function.h"
#include "Global.h"

SMatrix ZSpatialEntity::GetObjectToWorldMatrix() const
{
	return Function::CallRVOMethodAndReturn<SMatrix, const ZSpatialEntity*>(BaseAddress + 0x5E3A0, this);
}

void ZSpatialEntity::SetObjectToWorldMatrix(const SMatrix& mObjectToWorld)
{
	Function::CallMethod<ZSpatialEntity*, const SMatrix&>(BaseAddress + 0xBF60, this, mObjectToWorld);
}

float4 ZSpatialEntity::GetWorldPosition() const
{
	return GetObjectToWorldMatrix().Pos;
}

void ZSpatialEntity::SetWorldPosition(const float4& vWorldPosition)
{
	Function::CallMethod<ZSpatialEntity*, const float4&>(BaseAddress + 0x117720, this, vWorldPosition);
}

SMatrix ZSpatialEntity::GetViewMatrix() const
{
	return GetObjectToWorldMatrix().Inverse();
}

ZSpatialEntity* ZSpatialEntity::GetTransformParent() const
{
	return m_pTransformParent;
}

ZSpatialEntity* ZSpatialEntity::GetTransformChildren() const
{
	return m_pTransformChildren;
}
