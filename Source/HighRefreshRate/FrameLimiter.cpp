// https://github.com/michael-fadely/sadx-frame-limit/blob/11a231c76d053027bab29fe1df042b0b660878ab/sadx-frame-limit/mod.cpp

#include "FrameLimiter.h"

using namespace std::chrono;

using FrameRatio = duration<double, std::ratio<1, 60>>;

static bool enable_frame_limit = true;
static auto frame_start = system_clock::now();
static auto frame_ratio = FrameRatio(1);

static duration<double, std::milli> present_time = {};
static milliseconds frame_portion_ms = duration_cast<milliseconds>(frame_ratio) - milliseconds(1);

// v1.00: 0x1402B6960
// v1.01: 0x1402B6ED0
SIG_SCAN
(
	sigFrameLimiter,
	"\x40\x53\x48\x83\xEC\x50\x80\x3D\x00\x00\x00\x00\x00\x0F\x29\x74\x24\x00\xF3\x0F\x10\x35\x00\x00\x00\x00\x75\x56\x83\x3D\x00\x00\x00\x00\x00\x7E\x4D\x48\x8D\x54\x24\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x66\x0F\x6E\x05\x00\x00\x00\x00\x0F\x28\xCE\x0F\x5B\xC0\x0F\x57\xD2\xF3\x48\x0F\x2A\x10\xF3\x0F\x5E\xC8\xF3\x0F\x5C\xCA\xF3\x0F\x2C\xC1\x2D\x00\x00\x00\x00\x85\xC0\x7E\x11\x48\x98\x48\x8D\x4C\x24\x00\x48\x89\x44\x24\x00\xE8\x00\x00\x00\x00\x48\x8D\x54\x24\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x5C\x24\x00\x48\x8D\x4C\x24\x00\x0F\x57\xC0\x33\xD2\xF3\x48\x0F\x2A\xC3\xF3\x0F\x5E\xF0\xF3\x0F\x11\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x80\x3D\x00\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x44\x24\x00\x74\x67\x44\x8B\x05\x00\x00\x00\x00\x0F\x57\xC9\x48\x8B\x05\x00\x00\x00\x00\x41\xFF\xC0\x48\x03\xC3\x41\x8B\xC8\x48\x69\xD1\x00\x00\x00\x00\x0F\x57\xC0\xF3\x48\x0F\x2A\xC0\x44\x89\x05\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\xF3\x48\x0F\x2A\xCA\xF3\x0F\x5E\xC8\xF3\x0F\x11\x0D\x00\x00\x00\x00\x41\x83\xF8\x3C\x72\x19\x48\x99\x41\xD1\xE8\x48\x2B\xC2\x44\x89\x05\x00\x00\x00\x00\x48\xD1\xF8\x48\x89\x05\x00\x00\x00\x00\x48\x8D\x54\x24\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x0F\x28\x74\x24\x00\x48\x8B\x08\x48\x8B\x05\x00\x00\x00\x00\x48\x3B\xC1\x48\x89\x0D\x00\x00\x00\x00\x48\x0F\x4C\xC1\x48\x89\x05\x00\x00\x00\x00\x48\x83\xC4\x50\x5B\xC3",
	"xxxxxxxx?????xxxx?xxxx????xxxx?????xxxxxx?xxx????x????xxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxx?xxxx?x????xxxx?xxx????x????xxxx?xxxx?xxxxxxxxxxxxxxxxxx????x????xx?????xxx????xxxx?xxxxx????xxxxxx????xxxxxxxxxxxx????xxxxxxxxxxx????xxx????xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxx????xxxxxx????xxxx?xxxx?x????xxxx?xxxxxx????xxxxxx????xxxxxxx????xxxxxx"
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
	frame_ratio = FrameRatio(60.0f / (float)maxFPS);
	frame_portion_ms = duration_cast<milliseconds>(frame_ratio) - milliseconds(1);
	enable_frame_limit = enableLimiter;
}

void FrameLimiter::init()
{
	INSTALL_HOOK(_FrameLimiter);
	INSTALL_HOOK(D3D11CreateDeviceAndSwapChain);
}
