// v1.00: 0x14058584C
// v1.01: 0x14058087C
SIG_SCAN
(
	sigSaturationBlock1,
	"\x8D\x42\xBD\x83\xF8\x01\x0F\x86\x00\x00\x00\x00\x81\xFA\xCF\x00\x00\x00",
	"xxxxxxxx????xxxxxx"
);

// v1.00: 0x140586994
// v1.01: 0x1405819C4
SIG_SCAN
(
	sigSaturationBlock2,
	"\x8D\x42\xBD\x83\xF8\x01\x76\x00\x81\xFA\xCF\x00\x00\x00",
	"xxxxxxx?xxxxxx"
);

// v1.00: 0x140653680
// v1.01: 0x14064E1E0
SIG_SCAN
(
	sigUploadSettingCheck,
	"\x48\x83\xEC\x28\xC6\x02\x01\x4C\x8B\xCA\x41\xC6\x00\x01\x4C\x8B\xD1\xE8\x00\x00\x00\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00\x80\x78\x01\x00\x74\x23\x49\x8B\xCA\xE8\x00\x00\x00\x00\x84\xC0\x74\x17\x49\x8B\x82\x00\x00\x00\x00\x83\x38\xFF\x75\x04\x41\xC6\x01\x00\xB0\x01\x48\x83\xC4\x28\xC3\x41\xC6\x01\x00\x32\xC0\x41\xC6\x00\x00\x48\x83\xC4\x28\xC3",
	"xxxxxxxxxxxxxxxxxx????xxxx????xxxxxxxxxx????xxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
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
