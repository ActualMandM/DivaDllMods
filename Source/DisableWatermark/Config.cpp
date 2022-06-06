#include "Config.h"

bool Config::pvMark;
uint8_t Config::copyrightMark;

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
        MessageBoxA(nullptr, text, "Disable Watermarks", MB_OK | MB_ICONERROR);
    }

	pvMark = config["pv"].value_or(true);
	copyrightMark = config["copyright"].value_or(1);

    return true;
}
