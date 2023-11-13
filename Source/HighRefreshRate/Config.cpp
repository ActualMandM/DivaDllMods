#include "Config.h"

bool Config::enableVSync;
uint32_t Config::framerateCap;
bool Config::affectMenus;
bool Config::multiThreaded;
bool Config::newLimiter;
bool Config::oldFullscreen;

bool Config::init()
{
    toml::table config;

    try
    {
        config = toml::parse_file("config.toml");
    }
    catch (std::exception& exception)
    {
        char text[1024];
        sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
        MessageBoxA(nullptr, text, MOD_NAME, MB_OK | MB_ICONERROR);
    }

	enableVSync = config["vsync"].value_or(true);
	framerateCap = config["framerate"].value_or(0);
	affectMenus = config["menu"].value_or(false);
	multiThreaded = config["mt"].value_or(true);
	newLimiter = config["newlimiter"].value_or(true);
	oldFullscreen = config["oldfullscreen"].value_or(false);

    return true;
}
