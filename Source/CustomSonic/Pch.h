#pragma once

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <detours.h>

#include <BlueBlur.h>

#include <cstdint>
#include <cstdio>

<<<<<<< HEAD
#include <filesystem>
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
#include <mutex>

#include <INIReader.h>

#include <Helpers.h>

#include <Common.h>

#define EXPORT extern "C" __declspec(dllexport)
#define ASMHOOK __declspec(naked)