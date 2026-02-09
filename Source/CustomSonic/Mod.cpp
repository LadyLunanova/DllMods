#include <Common.h>
#include "InstallSetup.h"
#include "InstallCustomUI.h"
#include "SonicPlayer.h"
//#include <filesystem>

void ReadConfig();
void ReadJson(std::string jsonFilePath, int cCat);
static std::string jsonFilePathHead;
static std::string jsonFilePathBody;
static std::string jsonFilePathShoes;
static std::string jsonFilePathHandR;
static std::string jsonFilePathHandL;

static std::string jsonFilePathAddHead;
static std::string jsonFilePathAddBody;
static std::string jsonFilePathAddShoes;
static std::string jsonFilePathAddHandR;
static std::string jsonFilePathAddHandL;

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

	//MessageBoxA(nullptr, "Init Halt", "Window", 0);

	for (auto& mod : *modInfo->ModList)
	{
		jsonFilePathAddHead = mod->Path;
		jsonFilePathAddHead.erase(jsonFilePathAddHead.find_last_of("\\/") + 1);
		jsonFilePathAddHead += "AddCustomizeHead.json";
		ReadJson(jsonFilePathAddHead, 0);
		jsonFilePathAddHead.clear();
	}
	for (auto& mod : *modInfo->ModList)
	{
		jsonFilePathAddBody = mod->Path;
		jsonFilePathAddBody.erase(jsonFilePathAddBody.find_last_of("\\/") + 1);
		jsonFilePathAddBody += "AddCustomizeBody.json";
		ReadJson(jsonFilePathAddBody, 1);
		jsonFilePathAddBody.clear();
	}
	for (auto& mod : *modInfo->ModList)
	{
		jsonFilePathAddShoes = mod->Path;
		jsonFilePathAddShoes.erase(jsonFilePathAddShoes.find_last_of("\\/") + 1);
		jsonFilePathAddShoes += "AddCustomizeShoes.json";
		ReadJson(jsonFilePathAddShoes, 2);
		jsonFilePathAddShoes.clear();
	}


	ReadConfig();
	InstallSetup::applyPatches();
	InstallCustomUI::applyPatches(modInfo);
	InstallSonicPlayer::applyPatches();
}
EXPORT void PostInit()
{
	InstallSetup::applyPostPatches();
}