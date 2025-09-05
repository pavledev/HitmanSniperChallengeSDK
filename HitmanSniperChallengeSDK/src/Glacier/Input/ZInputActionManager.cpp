#include <Glacier/Input/ZInputActionManager.h>

#include <Function.h>
#include <Global.h>

void ZInputActionManager::SetEnabled(const bool enabled)
{
	this->m_bEnabled = enabled;
}

bool ZInputActionManager::AddBindings(const char* binds)
{
	return Function::CallMethodAndReturn<bool, ZInputActionManager*, const char*>(BaseAddress + 0x23CFD0, this, binds);
}
