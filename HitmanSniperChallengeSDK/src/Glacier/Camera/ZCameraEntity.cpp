#include "Glacier/Camera/ZCameraEntity.h"

#include <Function.h>
#include <Global.h>

void ZCameraEntity::UpdateProjection()
{
    Function::CallMethod<ZCameraEntity*>(BaseAddress + 0xC6E90, this);
}

SMatrix ZCameraEntity::GetViewMatrix() const
{
	return GetObjectToWorldMatrix().Inverse();
}

void ZCameraEntity::SetFovYDeg(const float fovYDeg)
{
    if (m_fFovYDeg != fovYDeg)
    {
        constexpr float degToRad = 3.1415927f / 180.0f;
        const float fFovYRad = fovYDeg * degToRad;

        m_fFovYDeg = fovYDeg;
        m_fFovY = fFovYRad >= 0.0099999998 ? fFovYRad : 0.0099999998;

        UpdateProjection();
    }
}

void ZCameraEntity::SetNearZ(float fNearZ)
{
    if (m_fNearZ != fNearZ)
    {
        m_fNearZ = fNearZ;

        if (m_fNearZ <= 0.0099999998f)
        {
            m_fNearZ = 0.0099999998f;
        }

        UpdateProjection();
    }
}

void ZCameraEntity::SetFarZ(float fFarZ)
{
    if (m_fFarZ != fFarZ)
    {
        const float minFarZ = m_fNearZ + 0.0099999998f;

        m_fFarZ = fFarZ;

        if (m_fFarZ <= minFarZ)
        {
            m_fFarZ = minFarZ;
        }

        UpdateProjection();
    }
}

TEntityRef<IRenderPostfilterControllerEntity>& ZCameraEntity::GetRenderPostfilterControllerEntity()
{
	return m_rPostfilter;
}
