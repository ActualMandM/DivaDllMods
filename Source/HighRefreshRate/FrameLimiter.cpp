#include "FrameLimiter.h"

static uint32_t targetFPS = 60;

SIG_SCAN
(
	sigFrameLimiter,
	0x1402B6DC0,
	"\x40\x53\x48\x83\xEC\x50\x80\x3D\x00\x00\x00\x00\x00",
	"xxxxxxxx?????"
);

HOOK(void, __fastcall, _FrameLimiter, sigFrameLimiter())
{
	if (targetFPS > 0)
	{
		// TODO: Get permission to use the code, due to license conflict.
		// UnleashedRecomp is licensed under GPL, whilst DivaDllMods is MIT.
	}
}

void FrameLimiter::SetTarget(uint32_t fps)
{
	targetFPS = fps;
}

void FrameLimiter::Init()
{
	INSTALL_HOOK(_FrameLimiter);
}
