// v1.01: 0x140580870
// v1.02: 0x140580634
SIG_SCAN
(
	sigSaturationBlock1,
	0x140580634,
	"\x8D\x42\xBD\x83\xF8\x01\x0F\x86\x00\x00\x00\x00\x81\xFA\xCF\x00\x00\x00",
	"xxxxxxxx????xxxxxx"
);

// v1.01: 0x1405819BC
// v1.02: 0x14058178C
SIG_SCAN
(
	sigSaturationBlock2,
	0x14058178C,
	"\x8D\x42\xBD\x83\xF8\x01\x76\x00\x81\xFA\xCF\x00\x00\x00",
	"xxxxxxx?xxxxxx"
);

// v1.01: 0x14064E1E0
// v1.02: 0x14064C980
SIG_SCAN
(
	sigUploadSettingCheck,
	0x14064C980,
	"\x48\x83\xEC\x28\xC6\x02\x01",
	"xxxxxxx"
);

HOOK(char, __fastcall, _LeaderboardUpload, sigUploadSettingCheck())
{
	printf("[Restore Cut Songs] Leaderboard upload detected, blocking.\n");
	return 0;
}

extern "C" __declspec(dllexport) void Init()
{
	WRITE_MEMORY((char*)sigSaturationBlock1() + 0xC, uint8_t, 0xEB, 0x0A);
	WRITE_MEMORY((char*)sigSaturationBlock2() + 0xC, uint8_t, 0xEB, 0x06);

	INSTALL_HOOK(_LeaderboardUpload);
}
