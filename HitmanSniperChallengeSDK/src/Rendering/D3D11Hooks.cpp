#include "D3D11Hooks.h"

#include <d3d11.h>
#include <dxgi.h>
#include <filesystem>

#include "Utility/D3DUtils.h"
#include "Logger.h"
#include "MinHook.h"
#include "SDK.h"

DEFINE_D3D11_HOOK(IDXGISwapChain, Present);
DEFINE_D3D11_HOOK(IDXGISwapChain, ResizeBuffers);

D3D11Hooks::~D3D11Hooks()
{
	RemoveHooks();
}

void D3D11Hooks::Startup()
{
	InstallHooks();
}

void D3D11Hooks::InstallHooks()
{
	const auto vTables = GetVTables();

	if (!vTables)
	{
		Logger::GetInstance().Log(Logger::Level::Error, "Could not get D3D vtables. Custom rendering will not work.");

		return;
	}

	INSTALL_D3D11_HOOK(IDXGISwapChain, Present);
	INSTALL_D3D11_HOOK(IDXGISwapChain, ResizeBuffers);

	Logger::GetInstance().Log(Logger::Level::Info, "Installed D3D hooks.");
}

void D3D11Hooks::RemoveHooks()
{
	for (auto& hook : installedHooks)
	{
		RemoveHook(hook);
	}

	installedHooks.clear();
}

struct ScopedWindowClass
{
	ScopedWindowClass() : classEx({}) {}

	~ScopedWindowClass()
	{
		UnregisterClassA(classEx.lpszClassName, classEx.hInstance);
	}

	WNDCLASSEX* operator->()
	{
		return &classEx;
	}

	operator WNDCLASSEX()
	{
		return classEx;
	}

	WNDCLASSEX classEx;
};

struct ScopedWindow
{
	ScopedWindow(HWND window) : window(window) {}

	~ScopedWindow()
	{
		if (window != nullptr)
		{
			DestroyWindow(window);
		}
	}

	operator HWND()
	{
		return window;
	}

	operator bool()
	{
		return window != nullptr;
	}

	HWND window;
};

std::optional<D3D11Hooks::VTables> D3D11Hooks::GetVTables()
{
	Logger::GetInstance().Log(Logger::Level::Info, "[D3D11Hooks] Locating D3D11 vtable addresses.");

	ScopedWindowClass windowClass;
	windowClass->cbSize = sizeof(WNDCLASSEX);
	windowClass->style = CS_HREDRAW | CS_VREDRAW;
	windowClass->lpfnWndProc = DefWindowProcA;
	windowClass->hInstance = GetModuleHandleA(nullptr);
	windowClass->lpszClassName = "AnvilSDK";

	RegisterClassExA(&windowClass.classEx);

	Logger::GetInstance().Log(Logger::Level::Info, "[D3D11Hooks] Creating fake window for swap chain.");

	ScopedWindow s_Window = CreateWindowA(windowClass->lpszClassName, "AnvilSDK_D3D", WS_OVERLAPPEDWINDOW, 0, 0, 256, 256, nullptr, nullptr, windowClass->hInstance, nullptr);

	if (!s_Window)
	{
		return std::nullopt;
	}

	DXGI_RATIONAL refreshRateRational{};
	refreshRateRational.Numerator = 60;
	refreshRateRational.Denominator = 1;

	DXGI_MODE_DESC bufferDesc{};
	bufferDesc.Width = 256;
	bufferDesc.Height = 256;
	bufferDesc.RefreshRate = refreshRateRational;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SAMPLE_DESC sampleDesc{};
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = s_Window;
	swapChainDesc.Windowed = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ScopedD3DRef<ID3D11Device> device;
	ScopedD3DRef<ID3D11DeviceContext> context;
	ScopedD3DRef<IDXGISwapChain> swapChain;

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain.ref,
		&device.ref,
		nullptr,
		&context.ref)))
	{
		return std::nullopt;
	}

	VTables vTables{};
	vTables.ID3D11DeviceVtbl = device.VTable();
	vTables.ID3D11DeviceContextVtbl = context.VTable();
	vTables.IDXGISwapChainVtbl = swapChain.VTable();

	Logger::GetInstance().Log(Logger::Level::Info, "[D3D11Hooks] Located all D3D11 vtable addresses.");

	return vTables;
}

void D3D11Hooks::InstallHook(void* vTable, int index, void* detour, void** original)
{
	auto vTableEntries = static_cast<void**>(vTable);
	auto originalAddr = vTableEntries[index];

	auto result = MH_CreateHook(originalAddr, detour, original);

	if (result != MH_OK)
	{
		Logger::GetInstance().Log(Logger::Level::Error, "Could not create D3D11 vtable hook. Error code: {}.", MH_StatusToString(result));

		return;
	}

	result = MH_EnableHook(originalAddr);

	if (result != MH_OK)
	{
		Logger::GetInstance().Log(Logger::Level::Error, "Could install detour for D3D11 vtable hook. Error code: {}.", MH_StatusToString(result));

		return;
	}

	InstalledHook hook;
	hook.vTable = vTableEntries;
	hook.index = index;
	hook.originalAddr = originalAddr;

	installedHooks.push_back(hook);

	Logger::GetInstance().Log(Logger::Level::Info, "Successfully installed detour for D3D11 vtable hook.");
}

void D3D11Hooks::RemoveHook(const InstalledHook& hook)
{
	MH_DisableHook(hook.originalAddr);
	MH_RemoveHook(hook.originalAddr);
}

HRESULT __stdcall D3D11Hooks::Detour_IDXGISwapChain_Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	SDK::GetInstance().OnPresent();

	return Original_IDXGISwapChain_Present(pSwapChain, SyncInterval, Flags);
}

HRESULT __stdcall D3D11Hooks::Detour_IDXGISwapChain_ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	SDK::GetInstance().OnReset();

	return Original_IDXGISwapChain_ResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}
