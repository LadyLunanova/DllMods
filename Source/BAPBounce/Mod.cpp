#include "Mod.h"

bool checkCode = false;

EXPORT void OnFrame()
{
	if (checkCode)
		return;
	checkCode = true;
	constexpr int key = 0xFFB6F908;
	bool isNotModified = *(int*)0x012552E4 == key;

	if (!isNotModified)
	{
		MessageBoxA(nullptr, "The Unleashed Style Stomp code is not compatible with Bounce Attack+, Disable one before rebooting", "Code Incompatibility", MB_ICONERROR);
		exit(-1);
	}
}

EXPORT void Init()
{
	//MessageBoxA(nullptr, "Hook Now", "Window", MB_OK);
	InstallSonicPlayer::applyPatches();
}

