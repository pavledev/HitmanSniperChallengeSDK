#include <Glacier/UI/ZHUDManager.h>

bool ZHUDManager::IsPauseMenuActive()
{
	return m_bPauseMenuActive;
}

IScaleformPlayer* ZHUDManager::GetHUD() const
{
	return m_mcHUD;
}
