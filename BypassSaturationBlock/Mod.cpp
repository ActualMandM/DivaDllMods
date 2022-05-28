extern "C" __declspec(dllexport) void Init()
{
	// TODO: Make this rely on sigscanning instead.
	WRITE_MEMORY(0x14058584C, uint8_t, 0xEB, 0x0A);
	WRITE_MEMORY(0x140586994, uint8_t, 0xEB, 0x06);
}
