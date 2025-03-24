#include "Config.h"

bool Config::vsync;
int32_t Config::fps;

bool Config::Init()
{
    toml::table config;

    try
    {
        config = toml::parse_file("config_hfr.toml");
    }
    catch (std::exception& exception)
    {
        char text[1024];
        sprintf_s(text, "Failed to parse config_hfr.toml:\n%s", exception.what());
        MessageBoxA(nullptr, text, MOD_NAME, MB_OK | MB_ICONERROR);
    }

	vsync = config["vsync"].value_or(true);
	fps = config["fps"].value_or(0);

    return true;
}
