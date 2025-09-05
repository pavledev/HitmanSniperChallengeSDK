#include "Glacier/Memory/ZMemoryManager.h"

#include "Global.h"
#include "Function.h"

IAllocator* ZMemoryManager::GetNormalAllocator() const
{
	return m_pNormalAllocator;
}

IAllocator* ZMemoryManager::GetAllocator(unsigned int nID)
{
	return Function::CallMethodAndReturn<IAllocator*, ZMemoryManager*, unsigned int>(BaseAddress + 0x2E6580, this, nID);
}
