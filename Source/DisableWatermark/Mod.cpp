#include "Config.h"

bool* pvMode;

SIG_SCAN(sigPVMark, 0x140C8B6D0, "pv_mark", "xxxxxxx");

SIG_SCAN(sigCopyright, 0x140CBCF10, "rom/copyright.farc", "xxxxxxxxxxxxxxxxxx");

SIG_SCAN
(
	sigLyrics,
	0x140245360,
	"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x00\x00\x00\x00\x80\xBB\x00\x00\x00\x00\x00",
	"xxxxxxxxxx????xx?????"
);

SIG_SCAN
(
	sigScreenshot,
	0x140601A00,
	"\x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x33\xFF\x89\x79\x08\x48\x89\x79\x10\x48\x89\x79\x18\x48\x83\xC1\x20",
	"xxxx?xxxx?xxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxx"
);

SIG_SCAN
(
	sigGameModeNX,
	0x1406F2AE0,
	"\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x70\x48\x8B\xD9\x48\x89\x4C\x24\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x41\xBC\x00\x00\x00\x00\x48\x8D\x55\xD8\x45\x8B\xC4\x4C\x8B\xE8\x66\x0F\x1F\x44\x00\x00",
	"xxxx?xxxxxxxxxxx?xxx????xxx????xxxxxxxxxxxxxx?xxx????x????xx????xxxxxxxxxxxxxxx?"
)

SIG_SCAN
(
	sigGameModePS4,
	0x140206810,
	"\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x54\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x70\x4C\x8B\xD9\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x41\xBC\x00\x00\x00\x00\x48\x8D\x55\xD8\x45\x8B\xC4\x4C\x8B\xF8\x0F\x1F\x44\x00\x00",
	"xxxx?xxxx?xxxxxxxxxxxx?xxx????xxx????xxxxxxxxxxxxx????x????xx????xxxxxxxxxxxxxx?"
)

SIG_SCAN
(
	sigGetPVMode,
	0x14040B2B0,
	"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x33\xED\x48\xC7\x41\x00\x00\x00\x00\x00\x48\x8B\xF9\x48\x89\x29\x48\x89\x69\x08\x48\x8D\x99\x00\x00\x00\x00\x40\x88\x69\x10\x8D\x75\x04\x48\x89\x69\x14\x66\x89\x69\x1C\x89\x69\x28\xC7\x41\x00\x00\x00\x00\x00",
	"xxxx?xxxx?xxxx?xxxxxxxxxx?????xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxx?????"
)

SIG_SCAN
(
	sigPhotoMode,
	0x14065C7E0,
	"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x26",
	"xxxxx????xxxxx"
)

HOOK(void, __fastcall, _SetGameModeNX, sigGameModeNX(), __int64 a1)
{
	original_SetGameModeNX(a1);

	WRITE_MEMORY((char*)sigLyrics() + 0x38, bool, *pvMode);
}

HOOK(void, __fastcall, _SetGameModePS4, sigGameModePS4(), __int64 a1)
{
	original_SetGameModePS4(a1);

	WRITE_MEMORY((char*)sigLyrics() + 0x38, bool, *pvMode);
}

extern "C" __declspec(dllexport) void Init()
{
	if (!sigValid)
	{
		versionWarning(TEXT(MOD_NAME));
		return;
	}

	Config::init();

	if (Config::pvMark)
	{
		WRITE_MEMORY(sigPVMark(), uint8_t, 0x00);
	}

	if (Config::copyrightMark == 1) // Redirect to disablewm.farc
	{
		WRITE_MEMORY(sigCopyright(), const char, "rom/disablewm.farc");
	}
	else if (Config::copyrightMark == 2) // Block game from overriding Steam screenshot
	{
		WRITE_MEMORY(sigScreenshot(), uint8_t, 0xC3);
	}

	if (Config::hideLyrics)
	{
		WRITE_MEMORY((char*)sigPhotoMode() + 0x2A, bool, true);

		uint8_t* instrAddr = (uint8_t*)sigGetPVMode() - 0x10;
		pvMode = (bool*)(instrAddr + readUnalignedU32(instrAddr + 0x3) + 0x20);
		printf("[%s] pvMode: 0x%llx\n", MOD_NAME, pvMode);
		INSTALL_HOOK(_SetGameModeNX);
		INSTALL_HOOK(_SetGameModePS4);
	}
}
