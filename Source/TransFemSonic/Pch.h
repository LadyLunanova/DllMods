#pragma once

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <detours.h>

#include <BlueBlur.h>
#include <LostCodeLoader.h>

#include <cstdint>
#include <cstdio>

#include <INIReader.h>

#include <Helpers.h>

#define EXPORT extern "C" __declspec(dllexport)
#define ASMHOOK __declspec(naked)