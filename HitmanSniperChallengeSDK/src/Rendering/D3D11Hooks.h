#pragma once

#include <dxgi.h>
#include <optional>
#include <vector>

#include "Hook.h"

#define DECLARE_D3D11_HOOK(ReturnType, ThisType, FuncName, ...) \
	private: \
		static ReturnType __stdcall  Detour_ ## ThisType ## _ ## FuncName(ThisType* th, __VA_ARGS__); \
		typedef ReturnType (__stdcall  *ThisType ## _ ## FuncName ## _t)(ThisType* th, __VA_ARGS__); \
		static ThisType ## _ ## FuncName ## _t Original_ ## ThisType ## _ ## FuncName;

#define DEFINE_D3D11_HOOK(ThisType, FuncName) \
	D3D11Hooks::ThisType ## _ ## FuncName ## _t D3D11Hooks::Original_ ## ThisType ## _ ## FuncName = nullptr;

#define INSTALL_D3D11_HOOK(ThisType, FuncName) InstallHook(vTables->ThisType ## Vtbl, static_cast<int>(D3D11Hooks::Function::ThisType ## _ ## FuncName), Detour_ ## ThisType ## _ ## FuncName, reinterpret_cast<void**>(&Original_ ## ThisType ## _ ## FuncName));

class D3D11Hooks
{
public:
    enum class Function
    {
        IDXGISwapChain_Present = 8,
        IDXGISwapChain_ResizeBuffers = 13
    };

private:
    struct VTables
    {
        void* ID3D11DeviceVtbl;
        void* ID3D11DeviceContextVtbl;
        void* IDXGISwapChainVtbl;
    };

    struct InstalledHook
    {
        void** vTable;
        int index;
        void* originalAddr;
    };

public:
    ~D3D11Hooks();
    void Startup();
    void InstallHooks();
    void RemoveHooks();

private:
    std::optional<VTables> GetVTables();
    void InstallHook(void* vTable, int index, void* detour, void** original);
    void RemoveHook(const InstalledHook& hook);

    DECLARE_D3D11_HOOK(HRESULT, IDXGISwapChain, Present, UINT SyncInterval, UINT Flags);
    DECLARE_D3D11_HOOK(HRESULT, IDXGISwapChain, ResizeBuffers, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

    std::vector<InstalledHook> installedHooks;
};
