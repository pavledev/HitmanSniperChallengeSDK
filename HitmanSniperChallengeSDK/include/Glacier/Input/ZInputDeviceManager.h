#pragma once

#include "IInputDeviceManager.h"
#include "ZInputDevice.h"

#include <Common.h>

class ZInputDeviceManager : public IInputDeviceManager
{
private:
	ZInputDevice* m_devices[32];
};
