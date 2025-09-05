#include <Global.h>
#include <Function.h>

bool SetPropertyValue(ZEntityType** pEntity, unsigned int nPropertyID, const ZVariantRef& value, bool bInvokeChangeHandlers)
{
	return Function::CallAndReturn<bool, ZEntityType**, unsigned int, const ZVariantRef&, bool>(BaseAddress + 0xA86B0, pEntity, nPropertyID, value, bInvokeChangeHandlers);
}

void SignalInputPin(ZEntityType** pEntity, unsigned int nPinID, const ZVariantRef& data)
{
	return Function::Call<ZEntityType**, unsigned int, const ZVariantRef&>(BaseAddress + 0x27AD10, pEntity, nPinID, data);
}

void SignalOutputPin(ZEntityType** pEntity, unsigned int nPinID, const ZVariantRef& data)
{
	return Function::Call<ZEntityType**, unsigned int, const ZVariantRef&>(BaseAddress + 0x2869F0, pEntity, nPinID, data);
}

bool GetApplicationOptionBool(const ZString& sName, bool bDefault)
{
	return Function::CallAndReturn<bool, const ZString&, bool>(BaseAddress + 0x984C0, sName, bDefault);
}
