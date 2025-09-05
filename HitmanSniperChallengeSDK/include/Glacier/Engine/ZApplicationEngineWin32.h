#pragma once

#include <Windows.h>

#include "ZEngineAppCommon.h"
#include "ZWin32ApplicationBase.h"

class HitmanSniperChallengeSDK_API ZApplicationEngineWin32 : public ZWin32ApplicationBase
{
public:
	static ZApplicationEngineWin32* GetInstance();
	static void SetInstance(ZApplicationEngineWin32** instance);
	ZEngineAppCommon& GetEngineAppCommon();
	HWND GetHWND();
	HICON GetDefaultCursor();
	void SetShowingCursor(const bool showingCursor);

	static ZCameraEntity* GetActiveCamera();

private:
	PAD(0x4C);
	ZEngineAppCommon m_common; //0x50
	PAD(0x74);
	HWND m_hWnd; //0x144
	PAD(0x1);
	bool m_bShowingCursor; //0x149
	PAD(0x6);
	HICON m_hDefaultCursor; //0x150

	inline static ZApplicationEngineWin32** m_pInstance = nullptr;
};
