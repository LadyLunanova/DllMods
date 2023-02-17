#pragma once

#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <detours.h>

#include <BlueBlur.h>

#include <cstdint>
#include <cstdio>

#include <INIReader.h>

#include <Helpers.h>

#include <boost\algorithm\string\predicate.hpp>

#define EXPORT extern "C" __declspec(dllexport)
#define ASMHOOK __declspec(naked)