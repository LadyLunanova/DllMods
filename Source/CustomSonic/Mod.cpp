#include <Common.h>
#include "InstallSetup.h"
#include "InstallCustomUI.h"
#include "SonicPlayer.h"

void ReadConfig();
void ReadJson(std::string jsonFilePath);
static std::string jsonFilePath;
static std::string jsonFilePathHead;

EXPORT void PreInit()
{
	GetModLoaderAPI()->SendMessageToLoader(ML_MSG_REQ_LARGE_ADDRESS_AWARE, nullptr);
}
EXPORT void Init(ModInfo_t* modInfo)
{
	jsonFilePath = modInfo->CurrentMod->Path;
	jsonFilePath.erase(jsonFilePath.find_last_of("\\/") + 1);
	jsonFilePathHead = (jsonFilePath += "CustomizeHead.json");
	InstallSetup::applyPatches();
	InstallCustomUI::applyPatches(modInfo);
	InstallSonicPlayer::applyPatches();
	//MessageBoxA(nullptr, "Init Halt", "Window", 0);
	ReadConfig();
	ReadJson(jsonFilePathHead);
}
EXPORT void PostInit()
{
	InstallSetup::applyPostPatches();
}