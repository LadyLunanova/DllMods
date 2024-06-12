#include "InstallSetup.h"

//Setup
void OnFrameHandler();
bool IsGensRT = false;

//OnFrame
EXPORT void OnFrame()
{
	//Model Visibility//
	OnFrameHandler();
}

void InstallSetup::applyPatches()
{
	WRITE_MEMORY(0x01271FD1, char*, "ef_null"); // Disable original game's jumpball creation
	WRITE_MEMORY(0x01271DC1, char*, "ef_null"); // Disable original game's super jumpball creation
}
void InstallSetup::applyPostPatches()
{
	if (GetModuleHandle(TEXT("GenerationsRaytracing.X86.dll")) != nullptr)
		IsGensRT = true;
}