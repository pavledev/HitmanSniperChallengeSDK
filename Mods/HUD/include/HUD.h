#pragma once

#include <Glacier/UI/EHUDItem.h>

#include <ModInterface.h>

class HUD : public ModInterface
{
public:
	void OnDrawMenu() override;

private:
	inline static int* uiDisableHUD;
	bool isHUDVisible;
};

DECLARE_MOD(HUD)
