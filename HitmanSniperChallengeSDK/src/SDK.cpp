#include <filesystem>

#include <MinHook.h>

#include <Glacier/Module/ZHitman5Module.h>
#include <Glacier/Engine/ZApplicationEngineWin32.h>
#include <Glacier/Engine/ZIniFile.h>
#include <Glacier/ZGraphicsSettingsManager.h>

#include "SDK.h"
#include "Global.h"
#include "Function.h"
#include "Logger.h"
#include "Hooks.h"
#include "ModInterface.h"
#include "Registry/ResourceIDRegistry.h"
#include "Registry/EnumRegistry.h"
#include "Registry/PropertyRegistry.h"
#include "Rendering/D3D11Hooks.h"

uintptr_t BaseAddress;
ZRenderManager* RenderManager;
ZLevelManager* LevelManager;
ZGraphicsSettingsManager* GraphicsSettingsManager;
ZMemoryManager* MemoryManager;
ZGameTimeManager* GameTimeManager;
ZInputDeviceManagerWindows* InputDeviceManager;
ZInputActionManager* InputActionManager;
ZHitman5Module* Hitman5Module;
ZGameLoopManager* GameLoopManager;
ZGameWideUI* GameWideUI;
ZHUDManager* HUDManager;
ZScaleformManager* ScaleformManager;
ZInputAction* HM5InputControl;
ZCollisionManager* CollisionManager;
ZTypeRegistry** TypeRegistry;
ZContentKitManager* ContentKitManager;
ZResourceManager* ResourceManager;
ZActorManager* ActorManager;
LocalResourceIDsResolver** LocalResourceIDsResolverSingleton;
ZCheckPointManager* CheckPointManager;
ZHM5ActionManager* HM5ActionManager;
ZEntityManager* EntityManager;
ZMorphemeMediator* MorphemeMediator;
bool IsEngineInitialized = true;
void* ZTemplateEntityFactoryVFTbl;
void* ZTemplateEntityBlueprintFactoryVFTbl;
void* ZAspectEntityFactoryVFTbl;
void* ZAspectEntityBlueprintFactoryVFTbl;

WNDPROC originalWndProc;

SDK::SDK()
{
    InitializeSingletons();

    if (MH_Initialize() != MH_OK)
    {
        Logger::GetInstance().Log(Logger::Level::Error, "Failed to initialize MinHook!");
    }

    directXRenderer = std::make_shared<DirectXRenderer>();
    imGuiRenderer = std::make_shared<ImGuiRenderer>();
    d3d11Hooks = std::make_shared<D3D11Hooks>();

    modManager = std::make_shared<ModManager>();

    mainMenu = std::make_shared<MainMenu>();
    modSelector = std::make_shared<ModSelector>();

    ResourceIDRegistry& resourceIDRegistry = ResourceIDRegistry::GetInstance();
    EnumRegistry& enumRegistry = EnumRegistry::GetInstance();
    PropertyRegistry& propertyRegistry = PropertyRegistry::GetInstance();
    std::thread thread = std::thread(&ResourceIDRegistry::Load, &resourceIDRegistry);
    std::thread thread2 = std::thread(&EnumRegistry::Load, &enumRegistry);
    std::thread thread3 = std::thread(&PropertyRegistry::Load, &propertyRegistry);

    thread.detach();
    thread2.detach();
    thread3.detach();
}

SDK& SDK::GetInstance()
{
    static SDK instance;

    return instance;
}

void SDK::Setup()
{
    d3d11Hooks->Startup();

    modManager->LoadAllMods();

    /*modManager->LockRead();

    for (const auto& mod : modManager->GetLoadedMods())
    {
        mod.second.modInterface->SetupUI();
        mod.second.modInterface->Initialize();
    }

    modManager->UnlockRead();*/

    if (IsEngineInitialized)
    {
        OnEngineInitialized();
    }
}

void SDK::Cleanup()
{
    directXRenderer->Cleanup();
    imGuiRenderer->Cleanup();

    if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
    {
        Logger::GetInstance().Log(Logger::Level::Error, "Failed to disable hooks.");
    }

    if (MH_Uninitialize() != MH_OK)
    {
        Logger::GetInstance().Log(Logger::Level::Error, "Failed to uninitialize MinHook.");
    }
}

ZMemoryManager* SDK::GetMemoryManager()
{
    return Function::CallAndReturn<ZMemoryManager*>(BaseAddress + 0x2E6550);
}

void SDK::InitializeSingletons()
{
    BaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
    RenderManager = reinterpret_cast<ZRenderManager*>(BaseAddress + 0x1227BE0);
    LevelManager = reinterpret_cast<ZLevelManager*>(BaseAddress + 0xE02120);
    GraphicsSettingsManager = reinterpret_cast<ZGraphicsSettingsManager*>(BaseAddress + 0xD4D3F0);
    MemoryManager = GetMemoryManager();
    GameTimeManager = reinterpret_cast<ZGameTimeManager*>(BaseAddress + 0x12231A0);
    InputDeviceManager = reinterpret_cast<ZInputDeviceManagerWindows*>(BaseAddress + 0x1224440);
    InputActionManager = reinterpret_cast<ZInputActionManager*>(BaseAddress + 0x1224E00);
    Hitman5Module = reinterpret_cast<ZHitman5Module*>(BaseAddress + 0xE028F8);
    GameLoopManager = reinterpret_cast<ZGameLoopManager*>(BaseAddress + 0x12230A0);
    GameWideUI = reinterpret_cast<ZGameWideUI*>(BaseAddress + 0xE02370);
    HUDManager = reinterpret_cast<ZHUDManager*>(BaseAddress + 0xD4DF40);
    ScaleformManager = reinterpret_cast<ZScaleformManager*>(BaseAddress + 0x3248980);
    HM5InputControl = reinterpret_cast<ZInputAction*>(BaseAddress + 0xD3545C);
    CollisionManager = reinterpret_cast<ZCollisionManager*>(BaseAddress + 0x3247DF0);
    TypeRegistry = reinterpret_cast<ZTypeRegistry**>(BaseAddress + 0xD2F30C);
    ContentKitManager = reinterpret_cast<ZContentKitManager*>(BaseAddress + 0xD4D4F0);
    ResourceManager = reinterpret_cast<ZResourceManager*>(BaseAddress + 0x12237D0);
    ActorManager = reinterpret_cast<ZActorManager*>(BaseAddress + 0xDDC7E0);
    LocalResourceIDsResolverSingleton = reinterpret_cast<LocalResourceIDsResolver**>(BaseAddress + 0x1223C4C);
    CheckPointManager = reinterpret_cast<ZCheckPointManager*>(BaseAddress + 0xE022B0);
    HM5ActionManager = reinterpret_cast<ZHM5ActionManager*>(BaseAddress + 0xD4EB90);
    EntityManager = reinterpret_cast<ZEntityManager*>(BaseAddress + 0x1223E60);
    MorphemeMediator = reinterpret_cast<ZMorphemeMediator*>(BaseAddress + 0x12252F0);
    ZTemplateEntityFactoryVFTbl = reinterpret_cast<void*>(BaseAddress + 0xB2C7CC);
    ZTemplateEntityBlueprintFactoryVFTbl = reinterpret_cast<void*>(BaseAddress + 0xB2C608);
    ZAspectEntityFactoryVFTbl = reinterpret_cast<void*>(BaseAddress + 0xB2B86C);
    ZAspectEntityBlueprintFactoryVFTbl = reinterpret_cast<void*>(BaseAddress + 0xB2B7F4);

    ZApplicationEngineWin32::SetInstance(reinterpret_cast<ZApplicationEngineWin32**>(BaseAddress + 0xD2DB08));
}

void SDK::OnEngineInitialized()
{
    modManager->LockRead();

    for (auto& loadedMod : modManager->GetLoadedMods())
    {
        loadedMod.second.modInterface->OnEngineInitialized();
    }

    modManager->UnlockRead();
}

void SDK::OnModLoaded(const std::string& name, ModInterface* modInterface, const bool liveLoad)
{
    modInterface->SetupUI();
    modInterface->Initialize();

    if (liveLoad && IsEngineInitialized)
    {
        modInterface->OnEngineInitialized();
    }
}

void SDK::OnDrawUI(const bool hasFocus)
{
    mainMenu->Draw(hasFocus);
    modSelector->Draw(hasFocus);

    modManager->LockRead();

    for (auto& mod : modManager->GetLoadedMods())
    {
        mod.second.modInterface->OnDrawUI(hasFocus);
    }

    modManager->UnlockRead();
}

void SDK::OnDraw3D()
{
    modManager->LockRead();

    for (auto& mod : modManager->GetLoadedMods())
    {
        mod.second.modInterface->OnDraw3D();
    }

    modManager->UnlockRead();
}

void SDK::OnDrawMenu()
{
    modManager->LockRead();

    for (auto& mod : modManager->GetLoadedMods())
    {
        mod.second.modInterface->OnDrawMenu();
    }

    modManager->UnlockRead();
}

void SDK::OnPresent()
{
    directXRenderer->OnPresent();
    imGuiRenderer->OnPresent();
}

void SDK::OnReset()
{
    directXRenderer->OnReset();
    imGuiRenderer->OnReset();
}

long SDK::MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return imGuiRenderer->OnMainWindowProc(hwnd, uMsg, wParam, lParam);
}

ImGuiContext* SDK::GetImGuiContext()
{
    return imGuiRenderer->GetImGuiContext();
}

ImGuiMemAllocFunc SDK::GetImGuiMemAllocFunc()
{
    return imGuiRenderer->GetImGuiMemAllocFunc();
}

ImGuiMemFreeFunc SDK::GetImGuiMemFreeFunc()
{
    return imGuiRenderer->GetImGuiMemFreeFunc();
}

void* SDK::GetImGuiUserDataAllocator()
{
    return imGuiRenderer->GetImGuiUserDataAllocator();
}

ImFont* SDK::GetRegularFont()
{
    return imGuiRenderer->GetRegularFont();
}

ImFont* SDK::GetBoldFont()
{
    return imGuiRenderer->GetBoldFont();
}

std::shared_ptr<DirectXRenderer> SDK::GetDirectXRenderer() const
{
    return directXRenderer;
}

std::shared_ptr<ImGuiRenderer> SDK::GetImGuiRenderer() const
{
    return imGuiRenderer;
}

std::shared_ptr<ModManager> SDK::GetModManager() const
{
    return modManager;
}

std::shared_ptr<ModSelector> SDK::GetModSelector() const
{
    return modSelector;
}

uintptr_t SDK::SearchPattern(uintptr_t baseAddress, size_t scanSize, const uint8_t* pattern, const char* mask)
{
    const size_t patternSize = strlen(mask);

    if (patternSize <= 1)
    {
        return 0;
    }

    const uintptr_t searchEnd = baseAddress + scanSize - patternSize;

    for (uintptr_t searchAddr = baseAddress; searchAddr <= searchEnd; ++searchAddr)
    {
        const uint8_t* memoryPtr = reinterpret_cast<uint8_t*>(searchAddr);
        bool found = true;

        for (int i = 0; i < patternSize; ++i)
        {
            if (mask[i] == '?')
            {
                continue;
            }

            if (memoryPtr[i] != pattern[i])
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            return searchAddr;
        }
    }

    return 0;
}

bool SDK::PatchCodeInternal(const char* pattern, const char* mask, void* newCode, size_t codeSize, ptrdiff_t offset, void* originalCode)
{
    if (!pattern || !mask || !newCode || codeSize == 0)
    {
        std::cout << "Invalid parameters provided to PatchCode call.";
        return false;
    }

    const auto* pattern2 = reinterpret_cast<const uint8_t*>(pattern);
    const auto target = SearchPattern(
        moduleBase,
        sizeOfCode,
        pattern2,
        mask
    );

    if (target == 0)
    {
        std::cout << "Could not find pattern in call to PatchCode. Game might have been updated.";
        return false;
    }

    auto* targetPtr = reinterpret_cast<void*>(target + offset);

    if (originalCode != nullptr)
    {
        memcpy(originalCode, targetPtr, codeSize);
    }

    std::cout << std::format("Patching {} bytes of code at {} with new code from {}.", codeSize, targetPtr, newCode);

    DWORD oldProtect;

    VirtualProtect(targetPtr, codeSize, PAGE_EXECUTE_READWRITE, &oldProtect);

    memcpy(targetPtr, newCode, codeSize);

    VirtualProtect(targetPtr, codeSize, oldProtect, nullptr);

    return true;
}

bool SDK::PatchCode(const char* pattern, const char* mask, void* newCode, size_t codeSize, ptrdiff_t offset)
{
    return PatchCodeInternal(pattern, mask, newCode, codeSize, offset, nullptr);
}

bool SDK::PatchCodeStoreOriginal(const char* pattern, const char* mask, void* newCode, size_t codeSize, ptrdiff_t offset, void* originalCode)
{
    return PatchCodeInternal(pattern, mask, newCode, codeSize, offset, originalCode);
}

uint32_t SDK::GetSizeOfCode(HMODULE module)
{
    PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
    PIMAGE_NT_HEADERS ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(module) +
        dosHeader->e_lfanew);

    if (!ntHeader)
    {
        return 0;
    }

    return static_cast<uint32_t>(ntHeader->OptionalHeader.SizeOfCode);
}

uintptr_t SDK::GetBaseOfCode(HMODULE module)
{
    PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
    PIMAGE_NT_HEADERS ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(module) +
        dosHeader->e_lfanew);

    if (!ntHeader)
    {
        return 0;
    }

    return static_cast<uintptr_t>(ntHeader->OptionalHeader.BaseOfCode);
}

LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return SDK::GetInstance().MainWindowProc(hwnd, uMsg, wParam, lParam);
}
