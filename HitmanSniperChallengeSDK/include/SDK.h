#pragma once

#include "Rendering/Renderers/DirectXRenderer.h"
#include "Rendering/Renderers/ImGuiRenderer.h"
#include "ModManager.h"
#include "UI/MainMenu.h"
#include "UI/ModSelector.h"

class ZMemoryManager;
class ZHitman5Module;
class ZEngineAppCommon;
class ZIniFile;
class ZMouseWindows;
class ZKeyboardWindows;

class D3D11Hooks;

extern LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class SDK
{
public:
	HitmanSniperChallengeSDK_API static SDK& GetInstance();
	void Setup();
	void Cleanup();

	static ZMemoryManager* GetMemoryManager();
	static void InitializeSingletons();

	void OnEngineInitialized();
	void OnModLoaded(const std::string& name, ModInterface* modInterface, const bool liveLoad);
	void OnDrawUI(const bool hasFocus);
	void OnDraw3D();
	void OnDrawMenu();

	void OnPresent();
	void OnReset();

	long MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HitmanSniperChallengeSDK_API ImGuiContext* GetImGuiContext();
	HitmanSniperChallengeSDK_API ImGuiMemAllocFunc GetImGuiMemAllocFunc();
	HitmanSniperChallengeSDK_API ImGuiMemFreeFunc GetImGuiMemFreeFunc();
	HitmanSniperChallengeSDK_API void* GetImGuiUserDataAllocator();
	HitmanSniperChallengeSDK_API ImFont* GetRegularFont();
	HitmanSniperChallengeSDK_API ImFont* GetBoldFont();

	HitmanSniperChallengeSDK_API std::shared_ptr<DirectXRenderer> GetDirectXRenderer() const;
	HitmanSniperChallengeSDK_API std::shared_ptr<ImGuiRenderer> GetImGuiRenderer() const;

	std::shared_ptr<ModManager> GetModManager() const;

	std::shared_ptr<ModSelector> GetModSelector() const;

	uintptr_t SearchPattern(uintptr_t p_BaseAddress, size_t p_ScanSize, const uint8_t* p_Pattern, const char* p_Mask);
	bool PatchCodeInternal(const char* p_Pattern, const char* p_Mask, void* p_NewCode, size_t p_CodeSize, ptrdiff_t p_Offset, void* p_OriginalCode);
	bool PatchCode(const char* pattern, const char* mask, void* newCode, size_t codeSize, ptrdiff_t offset);
	bool PatchCodeStoreOriginal(const char* p_Pattern, const char* p_Mask, void* p_NewCode, size_t p_CodeSize, ptrdiff_t p_Offset, void* p_OriginalCode);
	uint32_t GetSizeOfCode(HMODULE p_Module);
	uintptr_t GetBaseOfCode(HMODULE p_Module);

private:
	SDK();
	SDK(const SDK& other) = delete;
	SDK& operator=(const SDK& other) = delete;

	std::shared_ptr<DirectXRenderer> directXRenderer;
	std::shared_ptr<ImGuiRenderer> imGuiRenderer;
	std::shared_ptr<D3D11Hooks> d3d11Hooks;

	std::shared_ptr<ModManager> modManager;

	std::shared_ptr<MainMenu> mainMenu;
	std::shared_ptr<ModSelector> modSelector;

	uintptr_t moduleBase;
	uint32_t sizeOfCode;
};
