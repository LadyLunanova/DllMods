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
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBGui;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBIcon;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBLRMove;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBTextArea;
Chao::CSD::RCPtr<Chao::CSD::CScene> scBBDeco;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWABG1;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWATag;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWATagTxt;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAFooter;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWASelect;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAPress;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAArrow;
Chao::CSD::RCPtr<Chao::CSD::CScene> scSWAAlt;
boost::shared_ptr<Sonic::CGameObjectCSD> obBBCustomUI;
boost::shared_ptr<Sonic::CGameObjectCSD> obSWACustomUI;
static SharedPtrTypeless menuSoundHandle;
enum MenuOptionType
{
	UIPartShoes = 0,
	UIPartBody = 1,
	UIPartHead = 2,
	UIPartHandL = 3,
	UIPartHandR = 4,
};
MenuOptionType MenuOption = MenuOptionType::UIPartShoes;
int ItemOption = 0;
bool prevblur = false;
bool IsInMenu = false;
bool IsInMenuChange = false;
bool IsInMenuChangeL = false;
bool IsInMenuChangeR = false;
bool IsInMenuExit = false;
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
/*
{
	Select = 0,
	LStick = 1,
	RStick = 2,
	Triggers = 3,
	Shoulders = 4,
	Dup = 5,
	Ddown = 6,
	Dleft = 7,
	Dright = 8,
};
ActivateButtonType ActivateButton = ActivateButtonType::Select; */

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
	if (scSWASelect)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWASelect);
}

void CreateFittingUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	if (!IsUnleashedHUD)
	{
		auto spCsdProject = wrapper.GetCsdProject("ui_fitting");
		prFittingScreenBB = spCsdProject->m_rcProject;
		obBBCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenBB, 0.5f, "HUD_Pause", true);
		Sonic::CGameDocument::GetInstance()->AddGameObject(obBBCustomUI, "main", This);
	}
	else
	{
		auto spCsdProject = wrapper.GetCsdProject("ui_fitting_bb");
		prFittingScreenBB = spCsdProject->m_rcProject;
		obBBCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenBB, 0.5f, "HUD_Pause", true);
		Sonic::CGameDocument::GetInstance()->AddGameObject(obBBCustomUI, "main", This);

		auto spCsdProjectSWA = wrapper.GetCsdProject("ui_fitting_swa");
		prFittingScreenSWA = spCsdProjectSWA->m_rcProject;
		obSWACustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenSWA, 0.4f, "HUD_Pause", true);
		Sonic::CGameDocument::GetInstance()->AddGameObject(obSWACustomUI, "main", This);
	}
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
	char buffer[256]{};
	snprintf(buffer, 256,
		"%s\n%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n",
		"[Select]",
		"SelectShoes=", SelectShoes,
		"SelectBody=", SelectBody,
		"SelectHead=", SelectHead,
		"SelectHandL=", SelectHandL,
		"SelectHandR=", SelectHandR);
	fputs(buffer, iniFile);
}

void ReadINI()
{
	INIReader* reader = new INIReader("CustomSelect.ini");
	if (reader->ParseError() != 0)
	{
		FILE* pFile = fopen("CustomSelect.ini", "wb");
		WriteINI(pFile);
		fclose(pFile);
		reader = new INIReader("CustomSelect.ini");
	}
	SelectShoes = (SelectShoeType)reader->GetInteger("Select", "SelectShoes", SelectShoes);
	SelectBody = (SelectBodyType)reader->GetInteger("Select", "SelectBody", SelectBody);
	SelectHead = (SelectHeadType)reader->GetInteger("Select", "SelectHead", SelectHead);
	SelectHandL = (SelectHandLType)reader->GetInteger("Select", "SelectHandL", SelectHandL);
	SelectHandR = (SelectHandRType)reader->GetInteger("Select", "SelectHandR", SelectHandR);
}

void ReadConfig()
{
	INIReader reader("CustomSonic.ini");
	ConfigDecoEnable = reader.GetBoolean("Mod", "ConfigDecoEnable", ConfigDecoEnable);
	ActivateButton = reader.GetInteger("Mod", "ActivateButton", ActivateButton);

	if (Common::IsModEnabled("Sonic Unleashed HUD"))
		IsUnleashedHUD = true;
}