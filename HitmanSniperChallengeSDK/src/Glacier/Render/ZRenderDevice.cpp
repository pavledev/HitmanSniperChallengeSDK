#include "Glacier/Render/ZRenderDevice.h"

ID3D11Device* ZRenderDevice::GetDirect3DDevice() const
{
	return m_pDirect3DDevice;
}

ID3D11DeviceContext* ZRenderDevice::GetImmediateContext() const
{
	return m_pDeviceContextImmediate;
}
