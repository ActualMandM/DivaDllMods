// TODO: Make this rely on sigscanning instead.
HOOK(void, __fastcall, _SetFramerate, 0x1405FB7C0)
{
	original_SetFramerate();

	// TODO: Get the monitor's refresh rate instead, I don't think setting it to 0 is safe for the long-term.

	// TODO: Get this address from the function somehow.
	*(uint32_t*)0x1414B2A78 = 0;
}

extern "C" __declspec(dllexport) void Init()
{
	INSTALL_HOOK(_SetFramerate);
}
