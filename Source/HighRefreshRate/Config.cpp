#include "Config.h"

bool Config::enableVSync;
uint32_t Config::framerateCap;

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
        MessageBoxA(nullptr, text, "High Refresh Rate", MB_OK | MB_ICONERROR);
    }

	enableVSync = config["vsync"].value_or(true);
	framerateCap = config["framerate"].value_or(0);

    return true;
}
