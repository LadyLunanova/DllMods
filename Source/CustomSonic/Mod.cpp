#include <Common.h>
#include "InstallSetup.h"
#include "InstallCustomUI.h"
#include "SonicPlayer.h"

void ReadConfig();
void ReadJson(std::string jsonFilePath, int cCat);
static std::string jsonFilePath;
static std::string jsonFilePathHead;
static std::string jsonFilePathBody;
static std::string jsonFilePathShoes;
static std::string jsonFilePathHandR;
static std::string jsonFilePathHandL;

EXPORT void PreInit()
{
	GetModLoaderAPI()->SendMessageToLoader(ML_MSG_REQ_LARGE_ADDRESS_AWARE, nullptr);
}
EXPORT void Init(ModInfo_t* modInfo)
{
	jsonFilePath = modInfo->CurrentMod->Path;
	jsonFilePath.erase(jsonFilePath.find_last_of("\\/") + 1);
	jsonFilePathHead = (jsonFilePath += "CustomizeHead.json");
	jsonFilePathBody = (jsonFilePath += "CustomizeBody.json");
	jsonFilePathShoes = (jsonFilePath += "CustomizeShoes.json");
	jsonFilePathHandR = (jsonFilePath += "CustomizeHandR.json");
	jsonFilePathHandL = (jsonFilePath += "CustomizeHandL.json");
	InstallSetup::applyPatches();
	InstallCustomUI::applyPatches(modInfo);
	InstallSonicPlayer::applyPatches();
	//MessageBoxA(nullptr, "Init Halt", "Window", 0);
	ReadConfig();
	ReadJson(jsonFilePathHead, 0);
	ReadJson(jsonFilePathBody, 1);
	ReadJson(jsonFilePathShoes, 2);
	ReadJson(jsonFilePathHandR, 3);
	ReadJson(jsonFilePathHandL, 4);
}
EXPORT void PostInit()
{
	InstallSetup::applyPostPatches();
}