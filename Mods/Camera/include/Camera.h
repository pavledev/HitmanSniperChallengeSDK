#pragma once

#include <ModInterface.h>

class Camera : public ModInterface
{
public:
	void OnDrawMenu() override;
	void OnDrawUI(const bool hasFocus) override;

private:
	bool isOpen;

	bool areLUTAndVignetteEffectsEnabled = true;
};

DECLARE_MOD(Camera)
