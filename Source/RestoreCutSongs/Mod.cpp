// v1.02: 0x140580634
SIG_SCAN
(
	sigSaturationBlock1,
	0x140580674,
	"\x8D\x42\xBD\x83\xF8\x01\x0F\x86\x00\x00\x00\x00\x81\xFA\xCF\x00\x00\x00",
	"xxxxxxxx????xxxxxx"
);

// v1.02: 0x14058178C
SIG_SCAN
(
	sigSaturationBlock2,
	0x1405817CC,
	"\x8D\x42\xBD\x83\xF8\x01\x76\x00\x81\xFA\xCF\x00\x00\x00",
	"xxxxxxx?xxxxxx"
);

extern "C" __declspec(dllexport) void Init()
{
	if (!sigValid)
	{
		versionWarning(TEXT("Restore Cut Songs"));
		return;
	}

	WRITE_MEMORY((char*)sigSaturationBlock1() + 0xC, uint8_t, 0xEB, 0x0A);
	WRITE_MEMORY((char*)sigSaturationBlock2() + 0xC, uint8_t, 0xEB, 0x06);
}
