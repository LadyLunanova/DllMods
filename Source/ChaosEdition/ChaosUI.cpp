#include <Common.h>
#include "ChaosUI.h"

//Menu setup
Chao::CSD::RCPtr<Chao::CSD::CProject> prTxtBGScreen;
Chao::CSD::RCPtr<Chao::CSD::CProject> prTextScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> scTxtBGArea;
Chao::CSD::RCPtr<Chao::CSD::CScene> scTextArea;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugOpt01;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugOpt02;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugOpt03;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugOpt04;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugOpt05;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugOptDisp;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugOptScrollDisp;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugOptSelDisp;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugSelector;
boost::shared_ptr<Sonic::CGameObjectCSD> obTxtBGUI;
boost::shared_ptr<Sonic::CGameObjectCSD> obTextUI;
static SharedPtrTypeless menuSoundHandle;
bool UIExists = false;
bool DebugOpen = false;
int DebugSelect = 0;
int DebugListStart = 0;
int DebugListEnd = 5;
int DebugCount = 9;
int DebugScrollCount = 0;
int DebugOption01 = 0;
int DebugOption02 = 0;
int DebugOption03 = 0;
int DebugOption04 = 0;
int DebugOption05 = 0;
int DebugOptionSelect = 0;

void ChActSlowMaxSpd();
void ChActFlashbang();
void ChActSmallSonic();
void ChActBigSonic();
void ChActHideSonic();
void ChActInfMaxSpd();
void ChActSonicFrozen();

void SetTextCast(const char* Text)
{
	scTextArea->GetNode("Text_sdw")->SetText(Text);
	scTextArea->GetNode("Text_yellow")->SetText(Text);
	scTextArea->GetNode("Text_blue")->SetText(Text);
}
void SetBGWidth(float Width)
{
	scTxtBGArea->SetScale(Width, 0.7);
}

void SetDebugDispText(int Select, Chao::CSD::RCPtr<Chao::CSD::CScene>& Scene)
{
	const char* textInt = "0";

	switch (Select)
	{
	case -5:
		textInt = "-5";
		break;
	case -4:
		textInt = "-4";
		break;
	case -3:
		textInt = "-3";
		break;
	case -2:
		textInt = "-2";
		break;
	case -1:
		textInt = "-1";
		break;
	case 0:
		textInt = "00";
		break;
	case 1:
		textInt = "01";
		break;
	case 2:
		textInt = "02";
		break;
	case 3:
		textInt = "03";
		break;
	case 4:
		textInt = "04";
		break;
	case 5:
		textInt = "05";
		break;
	case 6:
		textInt = "06";
		break;
	case 7:
		textInt = "07";
		break;
	case 8:
		textInt = "08";
		break;
	case 9:
		textInt = "09";
		break;
	case 10:
		textInt = "10";
		break;
	case 11:
		textInt = "11";
		break;
	case 12:
		textInt = "12";
		break;
	default:
		textInt = "NULL";
		break;
	}

	Scene->GetNode("Text_sdw")->SetText(textInt);
	Scene->GetNode("Text_yellow")->SetText(textInt);
	Scene->GetNode("Text_blue")->SetText(textInt);
}
void SetDebugText(const char* Text, Chao::CSD::RCPtr<Chao::CSD::CScene>& Scene)
{
	Scene->GetNode("Text_sdw")->SetText(Text);
	Scene->GetNode("Text_yellow")->SetText(Text);
	Scene->GetNode("Text_blue")->SetText(Text);
}

void SetDebugOptions(int Item, Chao::CSD::RCPtr<Chao::CSD::CScene>& Scene)
{
	const char* textInt = "0";

	switch (Item)
	{
	case 0:
		textInt = "Slow Max Speed";
		break;
	case 1:
		textInt = "Flashbang";
		break;
	case 2:
		textInt = "Tiny Sonic";
		break;
	case 3:
		textInt = "Huge Sonic";
		break;
	case 4:
		textInt = "Hidden Sonic";
		break;
	case 5:
		textInt = "Uncapped Speed";
		break;
	case 6:
		textInt = "Frozen Sonic";
		break;
	case 7:
		textInt = "Cloud";
		break;
	case 8:
		textInt = "Barret";
		break;
	case 9:
		textInt = "Tifa";
		break;
	default:
		textInt = "OUT OF RANGE";
		break;
	}

	Scene->GetNode("Text_sdw")->SetText(textInt);
	Scene->GetNode("Text_yellow")->SetText(textInt);
	Scene->GetNode("Text_blue")->SetText(textInt);
}

void CreateChaosUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	if (obTxtBGUI == nullptr)
	{
		auto spBGCsdProject = wrapper.GetCsdProject("ui_chaostext_txtbg");
		prTxtBGScreen = spBGCsdProject->m_rcProject;
		obTxtBGUI = boost::make_shared<Sonic::CGameObjectCSD>(prTxtBGScreen, 0.5f, "HUD_Pause", true);
		Sonic::CGameDocument::GetInstance()->AddGameObject(obTxtBGUI, "main", This);
	}

	if (obTextUI == nullptr)
	{
		auto spTxtCsdProject = wrapper.GetCsdProject("ui_chaostext_txt");
		prTextScreen = spTxtCsdProject->m_rcProject;
		obTextUI = boost::make_shared<Sonic::CGameObjectCSD>(prTextScreen, 0.5f, "HUD_Pause", true);
		Sonic::CGameDocument::GetInstance()->AddGameObject(obTextUI, "main", This);
	}
}
void KillScreen()
{
	if (scTxtBGArea)
		Chao::CSD::CProject::DestroyScene(prTxtBGScreen.Get(), scTxtBGArea);

	if (scTextArea)
		Chao::CSD::CProject::DestroyScene(prTextScreen.Get(), scTextArea);

	if (obTxtBGUI)
	{
		obTxtBGUI->SendMessage(obTxtBGUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obTxtBGUI = nullptr;
	}

	if (obTextUI)
	{
		obTextUI->SendMessage(obTextUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obTextUI = nullptr;
	}
	UIExists = false;
}

void CHudChaosUpdate(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	auto inputState = Sonic::CInputState::GetInstance();
	auto inputPtr = &inputState->m_PadStates[inputState->m_CurrentPadStateIndex];
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	bool PressedX = input.IsTapped(Sonic::eKeyState_X);
	bool PressedB = input.IsTapped(Sonic::eKeyState_B);
	bool PressedA = input.IsTapped(Sonic::eKeyState_A);
	bool PressedSL = input.IsTapped(Sonic::eKeyState_Select);
	bool PressedST = input.IsTapped(Sonic::eKeyState_Start);
	bool PressedLB = input.IsTapped(Sonic::eKeyState_LeftBumper);
	bool PressedRB = input.IsTapped(Sonic::eKeyState_RightBumper);
	bool PressedLT = input.IsTapped(Sonic::eKeyState_LeftTrigger);
	bool PressedRT = input.IsTapped(Sonic::eKeyState_RightTrigger);
	bool PressedUp = input.IsTapped(Sonic::eKeyState_DpadUp);
	bool PressedDown = input.IsTapped(Sonic::eKeyState_DpadDown);
	bool PressedLeft = input.IsTapped(Sonic::eKeyState_DpadLeft);
	bool PressedRight = input.IsTapped(Sonic::eKeyState_DpadRight);
	bool PushedUp = inputPtr->LeftStickVertical >= 0.5f;
	bool PushedDown = inputPtr->LeftStickVertical <= -0.5f;
	bool PushedLeft = inputPtr->LeftStickHorizontal <= -0.5f;
	bool PushedRight = inputPtr->LeftStickHorizontal >= 0.5f;
	auto arX = 1280;
	auto arY = 720;
	auto DebugUIX = 0.85;

	if (!UIExists)
	{
		if (!obTxtBGUI || !obTextUI) //Create UI project if it doesn't exist
			CreateChaosUI(This, Edx, in_rUpdateInfo);

		UIExists = true;

		scTxtBGArea = prTxtBGScreen->CreateScene("text_bg");

		scTxtBGArea->SetMotion("Intro_Anim");
		scTxtBGArea->SetPosition(0.5 * arX, 0.1 * arY);
		scTxtBGArea->SetScale(0.5, 0.7);
		scTxtBGArea->SetMotionFrame(0.0f);
		scTxtBGArea->m_MotionDisableFlag = false;
		scTxtBGArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scTxtBGArea->m_MotionSpeed = 1.0f;
		scTxtBGArea->Update();

		scTextArea = prTextScreen->CreateScene("deco_text");

		scTextArea->SetMotion("White_Anim");
		scTextArea->SetMotionFrame(0.0f);
		scTextArea->m_MotionDisableFlag = false;
		scTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scTextArea->m_MotionSpeed = 0.0f;
		scTextArea->Update();

		scTextArea->SetMotion("Intro_Anim");
		scTextArea->SetPosition(0.5 * arX, 0.1 * arY);
		scTextArea->SetScale(0.7, 0.7);
		scTextArea->SetMotionFrame(0.0f);
		scTextArea->m_MotionDisableFlag = false;
		scTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scTextArea->m_MotionSpeed = 1.0f;
		scTextArea->Update();

		SetTextCast("Chaos Edition V1.0");
	}

	if (PressedY && !DebugOpen)
	{
		if (!obTxtBGUI || !obTextUI) //Create UI project if it doesn't exist
			CreateChaosUI(This, Edx, in_rUpdateInfo);

		DebugSelect = 0;
		DebugScrollCount = 0;
		DebugOpen = true;

		scDebugOptDisp = prTextScreen->CreateScene("deco_text");
		scDebugOptDisp->SetMotion("Intro_Anim");
		scDebugOptDisp->SetPosition(0.8 * arX, 0.05 * arY);
		scDebugOptDisp->SetScale(0.7, 0.7);
		scDebugOptDisp->SetMotionFrame(0.0f);
		scDebugOptDisp->m_MotionDisableFlag = false;
		scDebugOptDisp->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scDebugOptDisp->m_MotionSpeed = 1.0f;
		scDebugOptDisp->Update();
		SetDebugDispText(0, scDebugOptDisp);

		scDebugOptScrollDisp = prTextScreen->CreateScene("deco_text");
		scDebugOptScrollDisp->SetMotion("Intro_Anim");
		scDebugOptScrollDisp->SetPosition(0.85 * arX, 0.05 * arY);
		scDebugOptScrollDisp->SetScale(0.7, 0.7);
		scDebugOptScrollDisp->SetMotionFrame(0.0f);
		scDebugOptScrollDisp->m_MotionDisableFlag = false;
		scDebugOptScrollDisp->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scDebugOptScrollDisp->m_MotionSpeed = 1.0f;
		scDebugOptScrollDisp->Update();
		SetDebugDispText(0, scDebugOptScrollDisp);

		scDebugOptSelDisp = prTextScreen->CreateScene("deco_text");
		scDebugOptSelDisp->SetMotion("Intro_Anim");
		scDebugOptSelDisp->SetPosition(0.9 * arX, 0.05 * arY);
		scDebugOptSelDisp->SetScale(0.7, 0.7);
		scDebugOptSelDisp->SetMotionFrame(0.0f);
		scDebugOptSelDisp->m_MotionDisableFlag = false;
		scDebugOptSelDisp->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scDebugOptSelDisp->m_MotionSpeed = 1.0f;
		scDebugOptSelDisp->Update();
		SetDebugDispText(0, scDebugOptSelDisp);

		scDebugOpt01 = prTextScreen->CreateScene("deco_text");
		scDebugOpt01->SetMotion("Intro_Anim");
		scDebugOpt01->SetPosition(DebugUIX * arX, 0.1 * arY);
		scDebugOpt01->SetScale(0.7, 0.7);
		scDebugOpt01->SetMotionFrame(0.0f);
		scDebugOpt01->m_MotionDisableFlag = false;
		scDebugOpt01->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scDebugOpt01->m_MotionSpeed = 1.0f;
		scDebugOpt01->Update();
		SetDebugText("Option 1", scDebugOpt01);

		scDebugOpt02 = prTextScreen->CreateScene("deco_text");
		scDebugOpt02->SetMotion("Intro_Anim");
		scDebugOpt02->SetPosition(DebugUIX * arX, 0.15 * arY);
		scDebugOpt02->SetScale(0.7, 0.7);
		scDebugOpt02->SetMotionFrame(0.0f);
		scDebugOpt02->m_MotionDisableFlag = false;
		scDebugOpt02->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scDebugOpt02->m_MotionSpeed = 1.0f;
		scDebugOpt02->Update();
		SetDebugText("Option 2", scDebugOpt02);

		scDebugOpt03 = prTextScreen->CreateScene("deco_text");
		scDebugOpt03->SetMotion("Intro_Anim");
		scDebugOpt03->SetPosition(DebugUIX * arX, 0.2 * arY);
		scDebugOpt03->SetScale(0.7, 0.7);
		scDebugOpt03->SetMotionFrame(0.0f);
		scDebugOpt03->m_MotionDisableFlag = false;
		scDebugOpt03->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scDebugOpt03->m_MotionSpeed = 1.0f;
		scDebugOpt03->Update();
		SetDebugText("Option 3", scDebugOpt03);

		scDebugOpt04 = prTextScreen->CreateScene("deco_text");
		scDebugOpt04->SetMotion("Intro_Anim");
		scDebugOpt04->SetPosition(DebugUIX* arX, 0.25 * arY);
		scDebugOpt04->SetScale(0.7, 0.7);
		scDebugOpt04->SetMotionFrame(0.0f);
		scDebugOpt04->m_MotionDisableFlag = false;
		scDebugOpt04->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scDebugOpt04->m_MotionSpeed = 1.0f;
		scDebugOpt04->Update();
		SetDebugText("Option 4", scDebugOpt04);

		scDebugOpt05 = prTextScreen->CreateScene("deco_text");
		scDebugOpt05->SetMotion("Intro_Anim");
		scDebugOpt05->SetPosition(DebugUIX* arX, 0.3 * arY);
		scDebugOpt05->SetScale(0.7, 0.7);
		scDebugOpt05->SetMotionFrame(0.0f);
		scDebugOpt05->m_MotionDisableFlag = false;
		scDebugOpt05->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scDebugOpt05->m_MotionSpeed = 1.0f;
		scDebugOpt05->Update();
		SetDebugText("Option 5", scDebugOpt05);

		scDebugSelector = prTxtBGScreen->CreateScene("text_bg");

		scDebugSelector->SetMotion("Intro_Anim");
		scDebugSelector->SetPosition(DebugUIX* arX, 0.1 * arY);
		scDebugSelector->SetScale(0.5, 0.7);
		scDebugSelector->SetMotionFrame(0.0f);
		scDebugSelector->m_MotionDisableFlag = false;
		scDebugSelector->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scDebugSelector->m_MotionSpeed = 1.0f;
		scDebugSelector->Update();
	}

	if (DebugOpen)
	{
		DebugOption01 = DebugScrollCount - 0;
		DebugOption02 = DebugScrollCount + 1;
		DebugOption03 = DebugScrollCount + 2;
		DebugOption04 = DebugScrollCount + 3;
		DebugOption05 = DebugScrollCount + 4;
		DebugOptionSelect = DebugSelect - DebugScrollCount;

		SetDebugDispText(DebugSelect, scDebugOptDisp);
		SetDebugDispText(DebugScrollCount, scDebugOptScrollDisp);
		SetDebugDispText(DebugOptionSelect, scDebugOptSelDisp);
		SetDebugOptions(DebugOption01, scDebugOpt01);
		SetDebugOptions(DebugOption02, scDebugOpt02);
		SetDebugOptions(DebugOption03, scDebugOpt03);
		SetDebugOptions(DebugOption04, scDebugOpt04);
		SetDebugOptions(DebugOption05, scDebugOpt05);
		//SetDebug01Text("1");
		switch (DebugOptionSelect)
		{
		case -2:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.025 * arY);
			break;
		case -1:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.05 * arY);
			break;
		case 0:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.1 * arY);
			break;
		case 1:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.15 * arY);
			break;
		case 2:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.2 * arY);
			break;
		case 3:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.25 * arY);
			break;
		case 4:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.3 * arY);
			break;
		case 5:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.35 * arY);
			break;
		case 6:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.4 * arY);
			break;
		default:
			scDebugSelector->SetPosition(DebugUIX * arX, 0.05 * arY);
			break;
		}
		//scDebugSelector->SetPosition(0.75 * arX, 0.1 * arY);

		if (PressedDown)
		{
			if (DebugSelect >= DebugCount)
			{
				DebugSelect = DebugListStart;
				DebugScrollCount = DebugListStart;
			}
			else
				DebugSelect++;

			if (DebugScrollCount <= (DebugListEnd - 1))
			{
				if (DebugSelect >= DebugListEnd)
					DebugScrollCount++;
			}
		}

		if (PressedUp)
		{
			if (DebugSelect <= DebugListStart)
			{
				DebugSelect = DebugCount;
				DebugScrollCount = DebugCount - (DebugListEnd - 1);
			}
			else
				DebugSelect--;

			if (DebugScrollCount > DebugListStart)
			{
				if (DebugSelect <= (DebugListEnd - 1))
					DebugScrollCount--;
			}
		}

		if (PressedY && DebugOpen)
		{
			//DebugSelect = DebugListStart;
			//DebugScrollCount = DebugListStart;
			switch (DebugSelect)
			{
			case 0:
				ChActSlowMaxSpd();
				break;
			case 1:
				ChActFlashbang();
				break;
			case 2:
				ChActSmallSonic();
				break;
			case 3:
				ChActBigSonic();
				break;
			case 4:
				ChActHideSonic();
				break;
			case 5:
				ChActInfMaxSpd();
				break;
			case 6:
				ChActSonicFrozen();
				break;
			case 7:
				printf("CLOUD");
				break;
			case 8:
				printf("BARRET");
				break;
			case 9:
				printf("TIFA");
				break;
			}
		}
	}

	return;
}
HOOK(void, __fastcall, CHudSonicStageUpdate, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto speedContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	auto sonic = speedContext->m_pPlayer;
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);
	if (IsModernSonic)
		CHudChaosUpdate(This, Edx, in_rUpdateInfo);
	//printf("Gameplay HUD\n");
	originalCHudSonicStageUpdate(This, Edx, in_rUpdateInfo);
}
HOOK(void, __fastcall, CHudPlayableMenuUpdate, 0x0108D510, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto speedContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	auto sonic = speedContext->m_pPlayer;
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);
	if (IsModernSonic)
		CHudChaosUpdate(This, Edx, in_rUpdateInfo);
	//printf("Hub world HUD\n");
	originalCHudPlayableMenuUpdate(This, Edx, in_rUpdateInfo);
}
HOOK(void, __fastcall, CHUDPauseUpdate, 0x0042A520, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	originalCHUDPauseUpdate(This);
}
void __fastcall CHudSonicStageRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreen();
}
HOOK(void, __fastcall, CHudResultStart, 0x010B6840, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	originalCHudResultStart(This);
}

void ChaosUI::applyPatches()
{
	INSTALL_HOOK(CHudSonicStageUpdate);
	INSTALL_HOOK(CHudPlayableMenuUpdate);
	INSTALL_HOOK(CHUDPauseUpdate);
	WRITE_MEMORY(0x16A467C, void*, CHudSonicStageRemoveCallback);
	WRITE_MEMORY(0x16A5598, void*, CHudSonicStageRemoveCallback);
	INSTALL_HOOK(CHudResultStart);
}