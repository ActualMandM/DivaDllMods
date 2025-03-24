#include "FrameLimiter.h"

static int32_t targetFPS = 60;
static int32_t currentHz = 60;

SIG_SCAN
(
	sigFrameLimiter,
	0x1402B6DC0,
	"\x40\x53\x48\x83\xEC\x50\x80\x3D\x00\x00\x00\x00\x00",
	"xxxxxxxx?????"
);

HOOK(void, __fastcall, _FrameLimiter, sigFrameLimiter())
{
	if (targetFPS > 0)
	{
		// TODO: Get permission to use the code, due to license conflict.
		// UnleashedRecomp is licensed under GPL, whilst DivaDllMods is MIT.
	}
}

extern "C"
{
	__declspec(dllexport) void OnFrame(IDXGISwapChain* swapChain)
	{
		if (!sigValid && Config::fps != 0)
		{
			return;
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChain->GetDesc(&swapChainDesc);

		HMONITOR monitor = MonitorFromWindow(swapChainDesc.OutputWindow, MONITOR_DEFAULTTONEAREST);
		MONITORINFOEX monitorInfo = {};
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfoA(monitor, &monitorInfo);

		DEVMODEA devMode = {};
		devMode.dmSize = sizeof(DEVMODEA);
		EnumDisplaySettingsA(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);

		currentHz = devMode.dmDisplayFrequency;
	}
}

void FrameLimiter::SetTarget(int32_t fps)
{
	targetFPS = fps == 0 ? currentHz : fps;
}

void FrameLimiter::Init()
{
	INSTALL_HOOK(_FrameLimiter);
}
