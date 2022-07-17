#include "Config.h"
#include "FrameLimiter.h"

// v1.01: 0x1414ACA64, 0x1414ACA68
// v1.02: 0x1414ABBB4, 0x1414ABBB8
bool* vsync;
uint32_t* framerateCap;

// v1.01: 0x1402C0CF6
// v1.02: 0x1402C0BC6
SIG_SCAN
(
	sigSingleThreadedFlags,
	"\x41\x83\xC9\x08\xC7\x45\x00\x00\x00\x00\x00",
	"xxxxxx?????"
);

// v1.01: 0x1402B7460
// v1.02: 0x1402B7330
SIG_SCAN
(
	sigSetFramerate,
	"\x88\x0D\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x3C\x00\x00\x00",
	"xx????xx????xxxx"
);

// v1.01: 0x1405F66C0
// v1.02: 0x1405F4D90
SIG_SCAN
(
	sigSetFramerateInGame,
	"\x40\x53\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xD9\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xFF\xC8",
	"xxxxx????xxx????xxxxxxx????xxxxxxx?x????x????xx"
);

HOOK(void, __fastcall, _SetFramerate, (char*)sigSetFramerate())
{
	*vsync = Config::enableVSync;

	if (Config::affectMenus)
	{
		*framerateCap = Config::framerateCap;
	}
	else
	{
		*framerateCap = 60;
	}
}

// v1.01: 0x1405F677E
// v1.02: 0x1405F4E4E
HOOK(void, __fastcall, _SetFramerateInGame, (char*)sigSetFramerateInGame() + 0xBE)
{
	*vsync = Config::enableVSync;
	*framerateCap = Config::framerateCap;
}

uint32_t prevFramerateCap = 60;

extern "C" __declspec(dllexport) void OnFrame()
{
	if (Config::newLimiter && prevFramerateCap != *framerateCap)
	{
		if (*framerateCap == 0)
		{
			FrameLimiter::setCap(60, false);
		}
		else
		{
			FrameLimiter::setCap(*framerateCap, true);
		}

		prevFramerateCap = *framerateCap;
	}
}

extern "C" __declspec(dllexport) void Init()
{
	Config::init();

	// Grab the vsync and framerate cap addresses.
	uint8_t* instrAddr = (uint8_t*)sigSetFramerate();
	vsync = (bool*)(instrAddr + readUnalignedU32(instrAddr + 0x2) + 0x6);
	instrAddr += 0x6;
	framerateCap = (uint32_t*)(instrAddr + readUnalignedU32(instrAddr + 0x2) + 0xA);

	printf("[High Refresh Rate] vsync: 0x%08x\n", vsync);
	printf("[High Refresh Rate] framerateCap: 0x%08x\n", framerateCap);

	// Apply patches and install hooks.
	if (Config::multiThreaded)
	{
		WRITE_MEMORY(sigSingleThreadedFlags(), uint8_t, 0x45, 0x31, 0xC9, 0x90);
	}

	INSTALL_HOOK(_SetFramerate);
	INSTALL_HOOK(_SetFramerateInGame);

	if (Config::newLimiter)
	{
		FrameLimiter::init();
	}
}
