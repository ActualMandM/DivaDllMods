// v1.01: 0x1402C37B1
// v1.02: 0x1402C3681
SIG_SCAN
(
	sigWindowsCheck,
	"\xE8\x00\x00\x00\x00\x84\xC0\x75\x1F\xB1\x01",
	"x????xxxxxx"
);

extern "C" __declspec(dllexport) void Init()
{
	WRITE_NOP(sigWindowsCheck(), 5);
}
