#pragma once

class FrameLimiter
{
	public:
		static void Init();
		static void SetTarget(uint32_t fps);
};
