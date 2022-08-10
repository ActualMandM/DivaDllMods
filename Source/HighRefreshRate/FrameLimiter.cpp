// https://github.com/michael-fadely/sadx-frame-limit/blob/11a231c76d053027bab29fe1df042b0b660878ab/sadx-frame-limit/mod.cpp

#include "FrameLimiter.h"

using namespace std::chrono;

using FrameRatio = duration<double, std::ratio<1, 60>>;

static bool enable_frame_limit = true;
static auto frame_start = system_clock::now();
static auto frame_ratio = FrameRatio(1);

static duration<double, std::milli> present_time = {};
static milliseconds frame_portion_ms = duration_cast<milliseconds>(frame_ratio) - milliseconds(1);

// v1.01: 0x1402B6ED0
// v1.02: 0x1402B6DB0
SIG_SCAN
(
	sigFrameLimiter,
	0x1402B6DB0,
	"\x40\x53\x48\x83\xEC\x50\x80\x3D\x00\x00\x00\x00\x00",
	"xxxxxxxx?????"
);

HOOK(void, __fastcall, _FrameLimiter, sigFrameLimiter())
{
	if (enable_frame_limit && present_time < frame_ratio)
	{
		auto now = system_clock::now();
		const milliseconds delta = duration_cast<milliseconds>(now - frame_start);

		if (delta < frame_ratio)
		{
			// sleep for a portion of the frame time to free up cpu time
			std::this_thread::sleep_for(frame_portion_ms - delta);

			while ((now = system_clock::now()) - frame_start < frame_ratio)
			{
				// spin for the remainder of the time
			}
		}
	}

	frame_start = system_clock::now();
}

VTABLE_HOOK(HRESULT, WINAPI, IDXGISwapChain, Present, UINT SyncInterval, UINT Flags)
{
	// This is done to avoid vsync issues.
	const auto start = system_clock::now();
	HRESULT result = originalIDXGISwapChainPresent(This, SyncInterval, Flags);
	present_time = system_clock::now() - start;
	return result;
}

HOOK(HRESULT, WINAPI, D3D11CreateDeviceAndSwapChain, PROC_ADDRESS("d3d11.dll", "D3D11CreateDeviceAndSwapChain"),
	IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	const D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
	IDXGISwapChain** ppSwapChain,
	ID3D11Device** ppDevice,
	D3D_FEATURE_LEVEL* pFeatureLevel,
	ID3D11DeviceContext** ppImmediateContext)
{
	const HRESULT result = originalD3D11CreateDeviceAndSwapChain(
		pAdapter,
		DriverType,
		Software,
		Flags,
		pFeatureLevels,
		FeatureLevels,
		SDKVersion,
		pSwapChainDesc,
		ppSwapChain,
		ppDevice,
		pFeatureLevel,
		ppImmediateContext);

	if (SUCCEEDED(result) && ppSwapChain && *ppSwapChain)
	{
		INSTALL_VTABLE_HOOK(IDXGISwapChain, *ppSwapChain, Present, 8);
	}

	return result;
}

void FrameLimiter::setCap(intmax_t maxFPS, bool enableLimiter)
{
	frame_ratio = FrameRatio(60.0 / (double)maxFPS);
	frame_portion_ms = duration_cast<milliseconds>(frame_ratio) - milliseconds(1);
	enable_frame_limit = enableLimiter;
}

void FrameLimiter::init()
{
	INSTALL_HOOK(D3D11CreateDeviceAndSwapChain);
	INSTALL_HOOK(_FrameLimiter);
}
