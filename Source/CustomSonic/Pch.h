#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <detours.h>

#include <BlueBlur.h>

#include <cstdint>
#include <cstdio>

#include <filesystem>
#include <mutex>

#include <INIReader.h>

#include <HE1ML/ModLoader.h>

#include <Helpers.h>

#include <Common.h>

#define EXPORT extern "C" __declspec(dllexport)
#define ASMHOOK __declspec(naked)