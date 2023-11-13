#pragma once

#define WIN32_LEAN_AND_MEAN

#define MOD_NAME "Disable Watermark"

// Detours
#include <Windows.h>
#include <detours.h>

// Standard library
#include <cstdint>
#include <cstdio>
#include <string>

// Dependencies
#include <Helpers.h>
#include <Signature.h>
#include <toml.hpp>
