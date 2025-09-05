#include <Glacier/Input/ZInputAction.h>

#include <Function.h>
#include <Global.h>

ZInputAction::ZInputAction()
{
	m_szName = "";
	m_pkMap = nullptr;
	m_iSeq = -1;
}

ZInputAction::ZInputAction(const char* name)
{
	m_szName = name;
	m_pkMap = nullptr;
	m_iSeq = -1;
}

float ZInputAction::Analog()
{
	return Function::CallMethodAndReturn<float, ZInputAction*>(BaseAddress + 0x2A62B0, this);
}

bool ZInputAction::Digital()
{
	return Function::CallMethodAndReturn<bool, ZInputAction*>(BaseAddress + 0x21C910, this);
}

const char* ZInputAction::GetName() const
{
	return m_szName;
}

char ZInputAction::SetEnabled(bool bEnabled)
{
	return Function::CallMethodAndReturn<char, ZInputAction*>(BaseAddress + 0x120E10, this, bEnabled);
}
