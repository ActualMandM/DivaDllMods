#pragma once

class FrameLimiter
{
	public:
		static void init();
		static void setCap(intmax_t maxFPS, bool enableLimiter);
};
