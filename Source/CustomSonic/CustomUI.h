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
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBDeco;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBScroll;
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
	UISonicBody,
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

int CHudVarVisSel = 0;
int CHudVarScroll = 0;
int CHudVarTrueSel = 0;

int CHudVarShMaxScroll = 8;
int CHudVarBdMaxScroll = 3;
int CHudVarHeMaxScroll = 2;
int CHudVarHLMaxScroll = 4;
int CHudVarHRMaxScroll = 5;
int CHudVarSBMaxScroll = 0;


//Menu Functions
void CHudUICursorAnim()
{
	scBBIcon->SetMotion("ON_Anim");
	scBBIcon->SetMotionFrame(0.0f);
	scBBIcon->m_MotionDisableFlag = false;
	scBBIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	scBBIcon->m_MotionSpeed = 1.0f;
	scBBIcon->Update();
}

void CHudUISceneDestroy()
{
	if (scBBGui)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBGui);
	if (scBBIcon)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBIcon);
	if (scBBTextArea)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBTextArea);
	if (scBBLRMove)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBLRMove);
	if (scBBDeco)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBDeco);
	if (scBBScroll)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBScroll);
	if (scSWABG1)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWABG1);
	if (scSWATag)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWATag);
	if (scSWATagTxt)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWATagTxt);
	if (scSWAPress)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAPress);
	if (scSWAFooter)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAFooter);
	if (scSWAArrow)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAArrow);
	if (scSWAAlt)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAAlt);
	if (scSWAScroll)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAScroll);
	if (scSWAScrollBG)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAScrollBG);
	if (scSWASelect)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWASelect);
}

void KillScreen()
{
	if (obBBCustomUI)
	{
		obBBCustomUI->SendMessage(obBBCustomUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obBBCustomUI = nullptr;
	}
	if (obSWACustomUI)
	{
		obSWACustomUI->SendMessage(obSWACustomUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obSWACustomUI = nullptr;
	}
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

//INI file Handling
void WriteINI(FILE* iniFile)
{
	if (!iniFile)
		return;
	printf("WRITING INI");
	printf("\n");
	char buffer[512]{};
	snprintf(buffer, 512,
		"%s\n"
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
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n"
		"%s%d\n",
		"[Select]",
		"SelectShoes=", SelectShoesData,
		"SelectBody=", SelectBodyData,
		"SelectHead=", SelectHeadData,
		"SelectHandL=", SelectHandLData,
		"SelectHandR=", SelectHandRData,
		"SelectAltShoes=", s_ItemDataShoes[SelectShoesData].altselect,
		"SelectAltBody=", s_ItemDataBody[SelectBodyData].altselect,
		"SelectAltHead=", s_ItemDataHead[SelectHeadData].altselect,
		"SelectAltHandL=", s_ItemDataHandL[SelectHandLData].altselect,
		"SelectAltHandR=", s_ItemDataHandR[SelectHandRData].altselect,
		"SelectSnSonMat=", SelectSnSonMat,
		"SelectSsnSonMat=", SelectSsnSonMat,
		"SelectEyelid=", SelectEyelid,
		"SelectSsnHead=", SelectSsnHead,
		"SelectSsnForm=", SelectSsnForm,
		"SelectJumpBall=", SelectJumpBall);
	fputs(buffer, iniFile);
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
		fclose(pFile);
		reader = new INIReader(saveFilePath);
	}
	SelectShoesData = reader->GetInteger("Select", "SelectShoes", SelectShoesData);
	SelectBodyData = reader->GetInteger("Select", "SelectBody", SelectBodyData);
	SelectHeadData = reader->GetInteger("Select", "SelectHead", SelectHeadData);
	SelectHandLData = reader->GetInteger("Select", "SelectHandL", SelectHandLData);
	SelectHandRData = reader->GetInteger("Select", "SelectHandR", SelectHandRData);
	s_ItemDataShoes[SelectShoesData].altselect = reader->GetInteger("Select", "SelectAltShoes", s_ItemDataShoes[SelectShoesData].altselect);
	s_ItemDataBody[SelectBodyData].altselect = reader->GetInteger("Select", "SelectAltBody", s_ItemDataBody[SelectBodyData].altselect);
	s_ItemDataHead[SelectHeadData].altselect = reader->GetInteger("Select", "SelectAltHead", s_ItemDataHead[SelectHeadData].altselect);
	s_ItemDataHandL[SelectHandLData].altselect = reader->GetInteger("Select", "SelectAltHandL", s_ItemDataHandL[SelectHandLData].altselect);
	s_ItemDataHandR[SelectHandRData].altselect = reader->GetInteger("Select", "SelectAltHandR", s_ItemDataHandR[SelectHandRData].altselect);
	SelectSnSonMat = (SelectSnSonMatType)reader->GetInteger("Select", "SelectSnSonMat", SelectSnSonMat);
	SelectSsnSonMat = (SelectSsnSonMatType)reader->GetInteger("Select", "SelectSsnSonMat", SelectSsnSonMat);
	SelectEyelid = (SelectEyelidType)reader->GetInteger("Select", "SelectEyelid", SelectEyelid);
	SelectSsnHead = (SelectSsnHeadType)reader->GetInteger("Select", "SelectSsnHead", SelectSsnHead);
	SelectSsnForm = (SelectSsnFormType)reader->GetInteger("Select", "SelectSsnForm", SelectSsnForm);
	SelectJumpBall = (SelectJumpBallType)reader->GetInteger("Select", "SelectJumpBall", SelectJumpBall);
}

void ReadConfig()
{
	INIReader reader("CustomizeSetting.ini");
	ConfigDecoEnable = reader.GetBoolean("Mod", "ConfigDecoEnable", ConfigDecoEnable);
	ActivateButton = reader.GetInteger("Mod", "ActivateButton", ActivateButton);

	if (Common::IsModEnabled("Sonic Unleashed HUD"))
		IsUnleashedHUD = true;
}