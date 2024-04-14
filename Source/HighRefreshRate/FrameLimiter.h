#pragma once

class FrameLimiter
{
	public:
		static void Init();
		static void SetCap(intmax_t maxFPS, bool enableLimiter);
};
