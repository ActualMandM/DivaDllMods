SIG_SCAN(sigArcade, 0x140CC7140, "DIVA Arcade\x00", "xxxxxxxxxxxx");
SIG_SCAN(sigArcadeVerB, 0x140CC7150, "DIVA Arcade Ver.B", "xxxxxxxxxxxxxxxxx");
SIG_SCAN(sigFutureTone, 0x140CC7117, "\x00\Future Tone", "xxxxxxxxxxxx");
SIG_SCAN(sigArcadeUI, 0x140C879D0, "result_c_right_first", "xxxxxxxxxxxxxxxxxxxx");
SIG_SCAN(sigArcadeVerBUI, 0x140C82340, "result_c_right_verb", "xxxxxxxxxxxxxxxxxxx");
SIG_SCAN(sigFutureToneUI, 0x140C879E8, "result_c_right\x00", "xxxxxxxxxxxxxxx");

extern "C" __declspec(dllexport) void Init()
{
	if (!sigValid)
	{
		versionWarning(TEXT("Sound Theme Name Changer"));
		return;
	}

	// Replace sound theme strings.
	WRITE_MEMORY(sigArcade(), const char, "F/Arcade");
	WRITE_MEMORY(sigArcadeVerB(), const char, "F2nd/AC Ver.B");
	WRITE_MEMORY((char*)sigFutureTone() + 0x01, const char, "DIVA X/FT");

	// Prevent menu sprite from showing.
	#if true
		WRITE_MEMORY(sigArcadeUI(), uint8_t, 0x00);
		WRITE_MEMORY(sigArcadeVerBUI(), uint8_t, 0x00);
		WRITE_MEMORY(sigFutureToneUI(), uint8_t, 0x00);
	#endif
}
