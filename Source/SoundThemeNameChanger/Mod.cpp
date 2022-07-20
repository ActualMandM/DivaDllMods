// v1.01: 0x140CC8148
// v1.02: 0x140CC7298
SIG_SCAN(sigArcade, 0x140CC7298, "DIVA Arcade\x00", "xxxxxxxxxxxx");

// v1.01: 0x140CC8118
// v1.02: 0x140CC72A8
SIG_SCAN(sigArcadeVerB, 0x140CC72A8, "DIVA Arcade Ver.B", "xxxxxxxxxxxxxxxxx");

// v1.01: 0x140CC8130
// v1.02: 0x140CC71B0
SIG_SCAN(sigFutureTone, 0x140CC71AF, "\x00\Future Tone", "xxxxxxxxxxxx");

// v1.01: 0x140C88960
// v1.02: 0x140C879C0
SIG_SCAN(sigArcadeUI, 0x140C879C0, "result_c_right_first", "xxxxxxxxxxxxxxxxxxxx");

// v1.01: 0x140C832D0
// v1.02: 0x140C82330
SIG_SCAN(sigArcadeVerBUI, 0x140C82330, "result_c_right_verb", "xxxxxxxxxxxxxxxxxxx");

// v1.01: 0x140C88978
// v1.02: 0x140C879D8
SIG_SCAN(sigFutureToneUI, 0x140C879D8, "result_c_right\x00", "xxxxxxxxxxxxxxx");

extern "C" __declspec(dllexport) void Init()
{
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
