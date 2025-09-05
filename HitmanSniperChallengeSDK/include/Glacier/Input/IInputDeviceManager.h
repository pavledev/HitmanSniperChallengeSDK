#pragma once

#include "../IComponentInterface.h"
#include "IInputDevice.h"

class ZString;

class IInputDeviceManager : public IComponentInterface
{
public:
	virtual ~IInputDeviceManager() = 0;
	virtual void Unk5() = 0;
	virtual void Unk6() = 0;
	virtual void Unk7() = 0;
	virtual void Unk8() = 0;
	virtual void Unk9() = 0;
	virtual void Unk10() = 0;
	virtual void Unk11() = 0;
	virtual void Unk12() = 0;
	virtual void Unk13() = 0;
	virtual void Unk14() = 0;
	virtual void Unk15() = 0;
	virtual void Unk16() = 0;
	virtual void Unk17() = 0;
	virtual void Unk18() = 0;
	virtual void Unk19() = 0;
	virtual void Unk20() = 0;
	virtual void Unk21() = 0;
	virtual IInputDevice* GetNthDevicePtr(IInputDevice::EDeviceType deviceType, int param2) = 0;
};
