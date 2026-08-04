#include <Common.h>
#include "CustomSonicAPI.h"
#include "CustomSonicItems.h"
#include "InstallSetup.h"
#include "InstallCustomUI.h"
#include "SonicPlayer.h"

void ReadConfig();
void IsBounceModEnabled(bool enabled);
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
	for (auto& mod : *modInfo->ModList)
	{
		jsonFilePathAddHandR = mod->Path;
		jsonFilePathAddHandR.erase(jsonFilePathAddHandR.find_last_of("\\/") + 1);
		jsonFilePathAddHandR += "AddCustomizeHandR.json";
		ReadJson(jsonFilePathAddHandR, 3);
		jsonFilePathAddHandR.clear();
	}
	for (auto& mod : *modInfo->ModList)
	{
		jsonFilePathAddHandL = mod->Path;
		jsonFilePathAddHandL.erase(jsonFilePathAddHandL.find_last_of("\\/") + 1);
		jsonFilePathAddHandL += "AddCustomizeHandL.json";
		ReadJson(jsonFilePathAddHandL, 4);
		jsonFilePathAddHandL.clear();
	}

	ReadConfig();
	InstallSetup::applyPatches();
	InstallCustomUI::applyPatches(modInfo);
	InstallSonicPlayer::applyPatches();

	if (GetModuleHandle(L"BAPBounce.dll"))
		IsBounceModEnabled(true);
	else
		IsBounceModEnabled(false);
}

EXPORT void PostInit()
{
	InstallSetup::applyPostPatches();
}

EXPORT void ProcessMessage(size_t id, void* data)
{
	switch (id)
	{
		case CustomSonicAPI::MsgGetCustomSonicSelection::ID:
		{
			auto pMsg = (CustomSonicAPI::MsgGetCustomSonicSelection*)data;

			pMsg->HeadName = s_ItemDataHead[SelectHeadData].Name.c_str();
			pMsg->BodyName = s_ItemDataBody[SelectBodyData].Name.c_str();
			pMsg->ShoesName = s_ItemDataShoes[SelectShoesData].Name.c_str();
			pMsg->HandRName = s_ItemDataHandR[SelectHandRData].Name.c_str();
			pMsg->HandLName = s_ItemDataHandL[SelectHandLData].Name.c_str();
			
			pMsg->SelectSnMaterial = SelectSnMaterial;
			pMsg->SelectEyelid = SelectEyelid;
			pMsg->SelectSsnHead = SelectSsnHead;
			pMsg->SelectSsnMaterial = SelectSsnMaterial;
			pMsg->SelectJumpBall = SelectJumpBall;
			pMsg->SelectBounceBall = SelectBounceBall;
			
			break;
		}
	}
}