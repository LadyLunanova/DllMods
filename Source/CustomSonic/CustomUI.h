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
Chao::CSD::RCPtr<Chao::CSD::CProject> prFittingScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> scChara;
Chao::CSD::RCPtr<Chao::CSD::CScene> scIcon;
Chao::CSD::RCPtr<Chao::CSD::CScene> scLRMove;
Chao::CSD::RCPtr<Chao::CSD::CScene> scTextArea;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDeco;
boost::shared_ptr<Sonic::CGameObjectCSD> obCustomUI;
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

//Menu Functions
void PlayCursorAnim()
{
	scIcon->SetMotion("ON_Anim");
	scIcon->SetMotionFrame(0.0f);
	scIcon->m_MotionDisableFlag = false;
	scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	scIcon->m_MotionSpeed = 1.0f;
	scIcon->Update();
}

void CreateFittingUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
	auto spCsdProject = wrapper.GetCsdProject("ui_fitting");
	prFittingScreen = spCsdProject->m_rcProject;
	obCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreen, 0.5f, "HUD_Pause", true);
	Sonic::CGameDocument::GetInstance()->AddGameObject(obCustomUI, "main", This);
}

void KillScreen()
{
	if (obCustomUI)
	{
		obCustomUI->SendMessage(obCustomUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obCustomUI = nullptr;
	}
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