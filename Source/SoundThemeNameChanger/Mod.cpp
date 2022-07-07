// v1.00: 0x140CCE240
// v1.01: 0x140CC8148
SIG_SCAN(sigArcade, "DIVA Arcade\x00", "xxxxxxxxxxxx");

// v1.00: 0x140CCE250
// v1.01: 0x140CC8118
SIG_SCAN(sigArcadeVerB, "DIVA Arcade Ver.B", "xxxxxxxxxxxxxxxxx");

// v1.00: 0x140CCE268
// v1.01: 0x140CC8130
//SIG_SCAN(sigFutureTone, "Future Tone", "xxxxxxxxxxx");

// v1.00: 0x140C8E898
// v1.01: 0x140C88960
SIG_SCAN(sigArcadeUI, "result_c_right_first", "xxxxxxxxxxxxxxxxxxxx");

// v1.00: 0x140C89290
// v1.01: 0x140C832D0
SIG_SCAN(sigArcadeVerBUI, "result_c_right_verb", "xxxxxxxxxxxxxxxxxxx");

// v1.00: 0x140C8E858
// v1.01: 0x140C88978
SIG_SCAN(sigFutureToneUI, "result_c_right\x00", "xxxxxxxxxxxxxxx");

extern "C" __declspec(dllexport) void Init()
{
	// Replace sound theme strings.
	WRITE_MEMORY(sigArcade(), const char, "F/Arcade");
	WRITE_MEMORY(sigArcadeVerB(), const char, "F2nd/AC Ver.B");
	WRITE_MEMORY((char*)sigArcadeVerB() + 0x18, const char, "DIVA X/FT");

	// Prevent menu sprite from showing.
	#if true
		WRITE_MEMORY(sigArcadeUI(), uint8_t, 0x00);
		WRITE_MEMORY(sigArcadeVerBUI(), uint8_t, 0x00);
		WRITE_MEMORY(sigFutureToneUI(), uint8_t, 0x00);
	#endif
}
