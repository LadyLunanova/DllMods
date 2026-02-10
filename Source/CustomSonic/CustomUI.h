#pragma once

// Get Stage ID
// Original code by Brianuuu: https://github.com/brianuuu
inline uint32_t GetMultiLevelAddress(uint32_t initAddress, std::vector<uint32_t> offsets)
{
	uint32_t address = *(uint32_t*)initAddress;
	for (uint32_t i = 0; i < offsets.size(); i++)
	{
		uint32_t const& offset = offsets[i];
		address += offset;

		if (i < offsets.size() - 1)
		{
			address = *(uint32_t*)address;
		}
	}
	return address;
}
inline uint32_t GetCurrentStageID()
{
	uint32_t stageIDAddress = GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x80, 0x0 });
	return *(uint32_t*)stageIDAddress;
}

//Menu setup
Chao::CSD::RCPtr<Chao::CSD::CProject> prFittingScreenBB;
Chao::CSD::RCPtr<Chao::CSD::CProject> prFittingScreenSWA;
Chao::CSD::RCPtr<Chao::CSD::CProject> prFittingScreenTXT;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBGui;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBIcon;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBLRMove;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBTextArea;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBBtmTextArea;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBBtmBtn;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBDeco;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBScroll;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBPrev;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBPrevBtmTxt;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBPrevBtmBtn;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWABG1;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWATag;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWATagTxt;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAFooter;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWASelect;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAPress;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAArrow;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAAlt;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAScroll;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAScrollBG;
Chao::CSD::RCPtr<Chao::CSD::CScene> scTXT;
boost::shared_ptr<Sonic::CGameObjectCSD> obBBCustomUI;
boost::shared_ptr<Sonic::CGameObjectCSD> obSWACustomUI;
static SharedPtrTypeless menuSoundHandle;

enum MenuOptionType
{
	UIPartShoes,
	UIPartBody,
	UIPartHead,
	UIPartHandL,
	UIPartHandR,
	UIMiscOption,
};
MenuOptionType CHudTabSel = MenuOptionType::UIPartShoes;
bool prevblur = false;
bool IsInMenu = false;
bool IsInMenuChange = false;
bool IsInMenuChangeL = false;
bool IsInMenuChangeR = false;
bool IsInMenuExit = false;
bool IsInScrollOpen = false;
bool ConfigDecoEnable = true;
bool* const ENABLE_BLUR = (bool*)0x1A43103;
struct MsgLookAtStruct
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	bool Condition;
	__declspec(align(16)) Hedgehog::Math::CVector TargetPosition[4];
};
bool IsLookAt = false;
bool IsUnleashedHUD = false;
int SWAOpenTimer = 15;
int MemoryOpenTimer = 1800;
int ActivateButton = 0;
float CHudVarScrollBarStarSpinFlt = 0.0f;

int PrevOpenTimer = 0;
int PrevAnim = 0;
bool PrevCatAnim = true;
float PrevRotation = 0.0f;
bool IsPreviewOpen = true;

static std::string saveFilePath;

int CHudVarVisSel = 0;
int CHudVarScroll = 0;
int CHudVarTrueSel = 0;

int CHudVarShMaxScroll = 8;
int CHudVarBdMaxScroll = 3;
int CHudVarHeMaxScroll = 2;
int CHudVarHLMaxScroll = 4;
int CHudVarHRMaxScroll = 5;
int CHudVarSBMaxScroll = 0;

template <typename T>
int ReadCustomItem(INIReader* reader, const std::string& section, const std::string& name, std::vector<T>& customItems)
{
	auto ini = reader->Get(section, name, "");

	auto it = std::find_if(customItems.begin(), customItems.end(), [&](const T& item)
	{
		return item.Name == ini;
	});

	if (it != customItems.end())
		return std::distance(customItems.begin(), it);

	return 0;
}

//INI file Handling
void WriteINI(FILE* iniFile)
{
	if (!iniFile)
		return;
	printf("WRITING INI");
	printf("\n");
	char buffer[1024]{};
	snprintf(buffer, sizeof(buffer),
		"%s\n"
		"%s%s\n"
		"%s%s\n"
		"%s%s\n"
		"%s%s\n"
		"%s%s\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%s\n",
		"[Select]",
		"SelectShoes=", s_ItemDataShoes[SelectShoesData].Name.c_str(),
		"SelectBody=", s_ItemDataBody[SelectBodyData].Name.c_str(),
		"SelectHead=", s_ItemDataHead[SelectHeadData].Name.c_str(),
		"SelectHandL=", s_ItemDataHandL[SelectHandLData].Name.c_str(),
		"SelectHandR=", s_ItemDataHandR[SelectHandRData].Name.c_str(),
		"SelectAltShoes=", s_ItemDataShoes[SelectShoesData].AltSelect,
		"SelectAltBody=", s_ItemDataBody[SelectBodyData].AltSelect,
		"SelectAltHead=", s_ItemDataHead[SelectHeadData].AltSelect,
		"SelectAltHandL=", s_ItemDataHandL[SelectHandLData].AltSelect,
		"SelectAltHandR=", s_ItemDataHandR[SelectHandRData].AltSelect,
		"SelectSnSonMat=", SelectSnSonMat,
		"SelectSsnSonMat=", SelectSsnSonMat,
		"SelectEyelid=", SelectEyelid,
		"SelectSsnHead=", SelectSsnHead,
		"SelectSsnForm=", SelectSsnForm,
		"SelectJumpBall=", SelectJumpBall,
		"IsPreviewOpen=", (IsPreviewOpen ? "true" : "false"));
	fputs(buffer, iniFile);
	fclose(iniFile);
}

void ReadINI(std::string saveFilePath)
{
	printf("READING INI");
	printf("\n");
	INIReader* reader = new INIReader(saveFilePath);
	if (reader->ParseError() != 0)
	{
		printf("INI PARSE FAIL");
		printf("\n");
		FILE* pFile = fopen(saveFilePath.c_str(), "wb");
		WriteINI(pFile);
		reader = new INIReader(saveFilePath);
	}
	
	SelectShoesData = ReadCustomItem<ItemDataShoes>(reader, "Select", "SelectShoes", s_ItemDataShoes);
	SelectBodyData = ReadCustomItem<ItemDataBody>(reader, "Select", "SelectBody", s_ItemDataBody);
	SelectHeadData = ReadCustomItem<ItemDataHead>(reader, "Select", "SelectHead", s_ItemDataHead);
	SelectHandLData = ReadCustomItem<ItemDataHandL>(reader, "Select", "SelectHandL", s_ItemDataHandL);
	SelectHandRData = ReadCustomItem<ItemDataHandR>(reader, "Select", "SelectHandR", s_ItemDataHandR);
	s_ItemDataShoes[SelectShoesData].AltSelect = reader->GetInteger("Select", "SelectAltShoes", s_ItemDataShoes[SelectShoesData].AltSelect);
	s_ItemDataBody[SelectBodyData].AltSelect = reader->GetInteger("Select", "SelectAltBody", s_ItemDataBody[SelectBodyData].AltSelect);
	s_ItemDataHead[SelectHeadData].AltSelect = reader->GetInteger("Select", "SelectAltHead", s_ItemDataHead[SelectHeadData].AltSelect);
	s_ItemDataHandL[SelectHandLData].AltSelect = reader->GetInteger("Select", "SelectAltHandL", s_ItemDataHandL[SelectHandLData].AltSelect);
	s_ItemDataHandR[SelectHandRData].AltSelect = reader->GetInteger("Select", "SelectAltHandR", s_ItemDataHandR[SelectHandRData].AltSelect);
	SelectSnSonMat = (SelectSnSonMatType)reader->GetInteger("Select", "SelectSnSonMat", SelectSnSonMat);
	SelectSsnSonMat = (SelectSsnSonMatType)reader->GetInteger("Select", "SelectSsnSonMat", SelectSsnSonMat);
	SelectEyelid = (SelectEyelidType)reader->GetInteger("Select", "SelectEyelid", SelectEyelid);
	SelectSsnHead = (SelectSsnHeadType)reader->GetInteger("Select", "SelectSsnHead", SelectSsnHead);
	SelectSsnForm = (SelectSsnFormType)reader->GetInteger("Select", "SelectSsnForm", SelectSsnForm);
	SelectJumpBall = (SelectJumpBallType)reader->GetInteger("Select", "SelectJumpBall", SelectJumpBall);
	IsPreviewOpen = reader->GetBoolean("Select", "IsPreviewOpen", IsPreviewOpen);
}

void ReadConfig()
{
	INIReader reader("CustomizeSetting.ini");
	ConfigDecoEnable = reader.GetBoolean("Mod", "ConfigDecoEnable", ConfigDecoEnable);
	ActivateButton = reader.GetInteger("Mod", "ActivateButton", ActivateButton);

	if (Common::IsModEnabled("Sonic Unleashed HUD"))
		IsUnleashedHUD = true;
}

//Menu SFX
void CHudUISFXOpen()
{
	if (IsUnleashedHUD)
		Common::PlaySoundStatic(menuSoundHandle, 1000019);
	else
		Common::PlaySoundStatic(menuSoundHandle, 1000002);
}

void CHudUISFXSelect(bool Accept)
{
	if (IsUnleashedHUD)
	{
		if (Accept)
			Common::PlaySoundStatic(menuSoundHandle, 1000023);
		else
			Common::PlaySoundStatic(menuSoundHandle, 1000031);
	}
	else
	{
		if (Accept)
			Common::PlaySoundStatic(menuSoundHandle, 1000005);
		else
			Common::PlaySoundStatic(menuSoundHandle, 1000007);
	}
}

void CHudUISFXMove()
{
	if (IsUnleashedHUD)
		Common::PlaySoundStatic(menuSoundHandle, 1000022);
	else
		Common::PlaySoundStatic(menuSoundHandle, 1000004);
}

void CHudUISFXAlt()
{
	if (IsUnleashedHUD)
		Common::PlaySoundStatic(menuSoundHandle, 1000028);
	else
		Common::PlaySoundStatic(menuSoundHandle, 1000005);
}

void CHudUISFXSwitch()
{
	if (IsUnleashedHUD)
		Common::PlaySoundStatic(menuSoundHandle, 1000029);
	else
		Common::PlaySoundStatic(menuSoundHandle, 1000006);
}

void CHudUISFXExit()
{
	if (IsUnleashedHUD)
		Common::PlaySoundStatic(menuSoundHandle, 1000024);
	else
		Common::PlaySoundStatic(menuSoundHandle, 1000003);
}
