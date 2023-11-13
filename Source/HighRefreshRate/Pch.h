#pragma once

#define WIN32_LEAN_AND_MEAN

#define MOD_NAME "High Frame Rate"

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

// Frame Limiter
#include <chrono>
#include <d3d11.h>
#include <thread>
