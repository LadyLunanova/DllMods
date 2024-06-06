#include <Common.h>
#include "InstallSetup.h"
#include "InstallCustomUI.h"
#include "SonicPlayer.h"

void ReadConfig();
void ReadJson(std::string jsonFilePath, int cCat);
//static std::string jsonFilePath;
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
	jsonFilePathHead = modInfo->CurrentMod->Path;
	jsonFilePathHead.erase(jsonFilePathHead.find_last_of("\\/") + 1);
	jsonFilePathHead += "CustomizeHead.json";

	jsonFilePathBody = modInfo->CurrentMod->Path;
	jsonFilePathBody.erase(jsonFilePathBody.find_last_of("\\/") + 1);
	jsonFilePathBody += "CustomizeBody.json";

	jsonFilePathShoes = modInfo->CurrentMod->Path;
	jsonFilePathShoes.erase(jsonFilePathShoes.find_last_of("\\/") + 1);
	jsonFilePathShoes += "CustomizeShoes.json";

	jsonFilePathHandR = modInfo->CurrentMod->Path;
	jsonFilePathHandR.erase(jsonFilePathHandR.find_last_of("\\/") + 1);
	jsonFilePathHandR += "CustomizeHandR.json";

	jsonFilePathHandL = modInfo->CurrentMod->Path;
	jsonFilePathHandL.erase(jsonFilePathHandL.find_last_of("\\/") + 1);
	jsonFilePathHandL += "CustomizeHandL.json";
	ReadJson(jsonFilePathHead, 0);
	ReadJson(jsonFilePathBody, 1);
	ReadJson(jsonFilePathShoes, 2);
	ReadJson(jsonFilePathHandR, 3);
	ReadJson(jsonFilePathHandL, 4);
	ReadConfig();
	InstallSetup::applyPatches();
	InstallCustomUI::applyPatches(modInfo);
	InstallSonicPlayer::applyPatches();
	//MessageBoxA(nullptr, "Init Halt", "Window", 0);
}
EXPORT void PostInit()
{
	InstallSetup::applyPostPatches();
}