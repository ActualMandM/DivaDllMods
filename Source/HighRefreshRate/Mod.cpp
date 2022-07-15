#include "Config.h"
#include "FrameLimiter.h"

// v1.00: 0x1414B2A74, 0x1414B2A78
// v1.01: 0x1414ACA64, 0x1414ACA68
bool* vsync;
uint32_t* framerateCap;

// v1.00: 0x1402C0746
// v1.01: 0x1402C0CF6
SIG_SCAN
(
	sigSingleThreadedFlags,
	"\x41\x83\xC9\x08\xC7\x45\x00\x00\x00\x00\x00\x89\x75\xD0\x44\x89\x75\xD4\xC7\x45\x00\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00\x89\x4D\xE8\x48\xC7\x45\x00\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\x48\x89\x5D\x00\x83\xF7\x01\x89\x7D\x08\x89\x4D\x0C\xC7\x45\x00\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x44\x24\x00\x48\x8D\x45\x38\x48\x89\x44\x24\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x44\x24\x00\x4C\x89\x7C\x24\x00\x48\x8D\x45\xD0\x48\x89\x44\x24\x00\xC7\x44\x24\x00\x00\x00\x00\x00\xC7\x44\x24\x00\x00\x00\x00\x00\x48\x8D\x45\x20\x48\x89\x44\x24\x00\x45\x33\xC0\x8D\x51\x01\xFF\x15\x00\x00\x00\x00\x41\xC6\x87\x00\x00\x00\x00\x00\x85\xC0\x74\x0D\x41\xC6\x87\x00\x00\x00\x00\x00\xE9\x00\x00\x00\x00",
	"xxxxxx?????xxxxxxxxx?????xx?????xxx?????xxxxxx?????xx?????xx?????xxxxxxxxxxxxxxx?????xxx????xxxx?xxxxxxxx?xxx????xxxx?xxxx?xxxxxxxx?xxx?????xxx?????xxxxxxxx?xxxxxxxx????xxx?????xxxxxxx?????x????"
);

// v1.01: 0x1402B7460
// v1.02: 0x1402B7330
SIG_SCAN
(
	sigSetFramerate,
	"\x88\x0D\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x3C\x00\x00\x00",
	"xx????xx????xxxx"
);

// v1.00: 0x1405FBA80
// v1.01: 0x1405F66C0
SIG_SCAN
(
	sigSetFramerateInGame,
	"\x40\x53\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xD9\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xFF\xC8\x8B\x0B\x85\xC9\x79\x04\x33\xC9\xEB\x05\x3B\xC1\x0F\x4C\xC8\x89\x0B\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x90\x4C\x8D\x44\x24\x00\x8B\x13\xE8\x00\x00\x00\x00\x84\xC0\x74\x0E\x8B\x54\x24\x6C\x8B\x4C\x24\x68\xE8\x00\x00\x00\x00\x90\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x48\x8B\x8C\x24\x00\x00\x00\x00\x48\x33\xCC\xE8\x00\x00\x00\x00\x48\x81\xC4\x00\x00\x00\x00\x5B\xC3",
	"xxxxx????xxx????xxxxxxx????xxxxxxx?x????x????xxxxxxxxxxxxxxxxxxxxxxx?x????xxxxx?xxx????xxxxxxxxxxxxx????xxxxx?x????xxxx????xxxx????xxx????xx"
);

// v1.00: 0x1402B6EF0
// v1.01: 0x1402B7460
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

// v1.00: 0x1405FBB3E
// v1.01: 0x1405F677E
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
