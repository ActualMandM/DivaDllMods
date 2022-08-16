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
	printf("[Leaderboard Blocker] Leaderboard upload detected, blocking.\n");
	return 0;
}

extern "C" __declspec(dllexport) void Init()
{
	INSTALL_HOOK(_LeaderboardUpload);
}
