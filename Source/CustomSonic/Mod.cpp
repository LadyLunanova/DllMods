#include <Common.h>
#include "InstallSetup.h"
#include "InstallCustomUI.h"
#include "SonicPlayer.h"

void ReadConfig();

EXPORT void PreInit()
{
	GetModLoaderAPI()->SendMessageToLoader(ML_MSG_REQ_LARGE_ADDRESS_AWARE, nullptr);
}
EXPORT void Init(ModInfo_t* modInfo)
{
	InstallSetup::applyPatches();
	InstallCustomUI::applyPatches(modInfo);
	InstallSonicVFX::applyPatches();
	ReadConfig();
}
EXPORT void PostInit()
{
	InstallSetup::applyPostPatches();
}