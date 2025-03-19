#include "Config.h"
#include "FrameLimiter.h"

bool* vsync;
uint32_t* targetFPS;

SIG_SCAN
(
	sigSingleThreadedFlags,
	0x1402C0BE6,
	"\x41\x83\xC9\x08\xC7\x45\x00\x00\x00\x00\x00",
	"xxxxxx?????"
);

SIG_SCAN
(
	sigSetFramerate,
	0x1402B7340,
	"\x88\x0D\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x3C\x00\x00\x00",
	"xx????xx????xxxx"
);

SIG_SCAN
(
	sigSetFramerateInGame,
	0x1405F4DD0,
	"\x40\x53\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xD9\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xFF\xC8",
	"xxxxx????xxx????xxxxxxx????xxxxxxx?x????x????xx"
);

static void SetFramerate(uint32_t fps)
{
	*vsync = Config::vsync;
	*targetFPS = fps;
	FrameLimiter::SetTarget(*targetFPS);
}

HOOK(void, __fastcall, _SetFramerate, sigSetFramerate())
{
	SetFramerate(60);
}

HOOK(void, __fastcall, _SetFramerateInGame, (char*)sigSetFramerateInGame() + 0xBE)
{
	SetFramerate(Config::fps);
}

extern "C"
{
	__declspec(dllexport) void Init()
	{
		if (!sigValid)
		{
			versionWarning(TEXT(MOD_NAME));
			return;
		}

		Config::Init();

		// Grab the VSync and target FPS addresses.
		{
			uint8_t* instrAddr = (uint8_t*)sigSetFramerate();
			vsync = (bool*)(instrAddr + readUnalignedU32(instrAddr + 0x2) + 0x6);
			instrAddr += 0x6;
			targetFPS = (uint32_t*)(instrAddr + readUnalignedU32(instrAddr + 0x2) + 0xA);
		}

		printf("[%s] vsync: 0x%llx\n", MOD_NAME, vsync);
		printf("[%s] targetFPS: 0x%llx\n", MOD_NAME, targetFPS);

		// Disable D3D11's singlethreaded flag.
		WRITE_MEMORY(sigSingleThreadedFlags(), uint8_t, 0x45, 0x31, 0xC9, 0x90);

		// Disable 60hz limit and forced VSync when in fullscreen.
		WRITE_MEMORY((char*)sigSingleThreadedFlags() + 0x15, uint8_t, 0);
		WRITE_MEMORY((char*)sigSingleThreadedFlags() + 0x1C, uint8_t, 0);

		INSTALL_HOOK(_SetFramerate);
		INSTALL_HOOK(_SetFramerateInGame);

		FrameLimiter::Init();
	}
}
