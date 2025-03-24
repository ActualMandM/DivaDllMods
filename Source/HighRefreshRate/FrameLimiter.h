#pragma once

#include "Config.h"
#include <chrono>
#include <thread>
#include <d3d11.h>
#include <winuser.h>

class FrameLimiter
{
	public:
		static void Init();
		static void SetTarget(int32_t fps);
};
