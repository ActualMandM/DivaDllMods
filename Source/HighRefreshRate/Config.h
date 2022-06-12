#pragma once

class Config
{
	public:
		static bool enableVSync;
		static uint32_t framerateCap;
		static bool affectMenus;
		static bool multiThreaded;
	
		static bool init();
};
