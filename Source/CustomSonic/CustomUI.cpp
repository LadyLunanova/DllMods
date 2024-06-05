#include <Common.h>
#include "UpdateDirector.h"
#include "CustomSonicItems.h"
#include "InstallCustomUI.h"
#include "InstallSetup.h"
#include "CustomUI.h"

//Debug Display setup
Chao::CSD::RCPtr<Chao::CSD::CProject> prDebugTxtScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugTxtVisSel;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugTxtScroll;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugTxtTrueSel;
boost::shared_ptr<Sonic::CGameObjectCSD> obDebugTxtUI;
bool DebugUIExists = false;
bool DebugOpen = false;
int DebugSelect = 0;
int DebugCount = 9;
int DebugScrollCount = 0;
int DebugOptionSelect = 0;

void SetDebugDispText(int Select, Chao::CSD::RCPtr<Chao::CSD::CScene>& Scene, int Prefix)
{
	char value0[256]; sprintf(value0, "%d - Visual Selection", Select);
	char value1[256]; sprintf(value1, "%d - Scroll Height", Select);
	char value2[256]; sprintf(value2, "%d - True Selection", Select);

	switch (Prefix)
	{
	case 0:
		Scene->GetNode("Text_sdw")->SetText(value0);
		Scene->GetNode("Text_yellow")->SetText(value0);
		Scene->GetNode("Text_blue")->SetText(value0);
		break;
	case 1:
		Scene->GetNode("Text_sdw")->SetText(value1);
		Scene->GetNode("Text_yellow")->SetText(value1);
		Scene->GetNode("Text_blue")->SetText(value1);
		break;
	case 2:
		Scene->GetNode("Text_sdw")->SetText(value2);
		Scene->GetNode("Text_yellow")->SetText(value2);
		Scene->GetNode("Text_blue")->SetText(value2);
		break;
	}
	
}
void SetDebugText(const char* Text, Chao::CSD::RCPtr<Chao::CSD::CScene>& Scene)
{
	Scene->GetNode("Text_sdw")->SetText(Text);
	Scene->GetNode("Text_yellow")->SetText(Text);
	Scene->GetNode("Text_blue")->SetText(Text);
}

void CreateDebugTxtUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	if (obDebugTxtUI == nullptr)
	{
		auto spTxtCsdProject = wrapper.GetCsdProject("ui_fittingtext_bb");
		prDebugTxtScreen = spTxtCsdProject->m_rcProject;
		obDebugTxtUI = boost::make_shared<Sonic::CGameObjectCSD>(prDebugTxtScreen, 0.5f, "HUD_Pause", true);
		Sonic::CGameDocument::GetInstance()->AddGameObject(obDebugTxtUI, "main", This);
	}
}
void KillDebugTxtUI()
{
	if (obDebugTxtUI)
	{
		obDebugTxtUI->SendMessage(obDebugTxtUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obDebugTxtUI = nullptr;
	}
	DebugUIExists = false;
	DebugOpen = false;
}

//Declare Funcs
void ReadINI(std::string saveFilePath);
void WriteINI(FILE* iniFile);

static std::string saveFilePath;

//Handle UI
void CreateFittingUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
	//Sonic::CCsdDatabaseWrapper wrapper(database_ui.get());

	if (!IsUnleashedHUD)
	{
		auto spCsdProject = wrapper.GetCsdProject("ui_fitting");
		if (spCsdProject->IsMadeAll())
		{
			prFittingScreenBB = spCsdProject->m_rcProject;
			obBBCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenBB, 0.5f, "HUD_Pause", true);
			Sonic::CGameDocument::GetInstance()->AddGameObject(obBBCustomUI, "main", This);
		}
	}
	else
	{
		auto spCsdProject = wrapper.GetCsdProject("ui_fitting_bb");
		if (spCsdProject->IsMadeAll())
		{
			prFittingScreenBB = spCsdProject->m_rcProject;
			obBBCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenBB, 0.5f, "HUD_Pause", true);
			Sonic::CGameDocument::GetInstance()->AddGameObject(obBBCustomUI, "main", This);
		}

		auto spCsdProjectSWA = wrapper.GetCsdProject("ui_fitting_swa");
		if (spCsdProjectSWA->IsMadeAll())
		{
			prFittingScreenSWA = spCsdProjectSWA->m_rcProject;
			obSWACustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenSWA, 0.4f, "HUD_Pause", true);
			Sonic::CGameDocument::GetInstance()->AddGameObject(obSWACustomUI, "main", This);
		}
	}
}

void CHudUIOpen(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo, float Sh, float Bd, float He, float HL, float HR, float SB)
{
	auto arX = 1280.0f;
	auto arY = 720.0f;
	auto scaleY = 0.88642f;
	auto rowX1 = 0.6885f;
	auto rowX2 = 0.7745f;
	auto rowX3 = 0.8611f;
	auto rowY1 = 0.3990f;
	auto rowY2 = 0.5515f;
	auto rowY3 = 0.7046f;

	ReadINI(saveFilePath);
	prevblur = *ENABLE_BLUR;
	*ENABLE_BLUR = false;
	if (MemoryOpenTimer <= 0)
	{
		CHudTabSel = UIPartShoes;
		CHudVarVisSel = 0;
		CHudVarScroll = 0;
		CHudVarTrueSel = 0;
	}
	if (!obBBCustomUI) //Create UI project if it doesn't exist
		CreateFittingUI(This, Edx, in_rUpdateInfo);

	//isLoadModel = true;
	//Main UI
	scBBGui = prFittingScreenBB->CreateScene("chara");
	scBBGui->SetMotion("Intro_Anim");
	scBBGui->SetMotionFrame(0.0f);
	scBBGui->m_MotionDisableFlag = false;
	scBBGui->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	scBBGui->m_MotionSpeed = 1.0f;
	scBBGui->Update();
	//Cursor
	scBBIcon = prFittingScreenBB->CreateScene("icon");
	scBBIcon->SetPosition(rowX1 * arX, rowY1 * arY);
	scBBIcon->SetMotion("ON_Anim");
	scBBIcon->SetMotionFrame(0.0f);
	scBBIcon->m_MotionDisableFlag = false;
	scBBIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	scBBIcon->m_MotionSpeed = 1.0f;
	scBBIcon->Update();
	//Category Text
	scBBTextArea = prFittingScreenBB->CreateScene("textarea");
	scBBTextArea->SetPosition(0, 0);
	scBBTextArea->SetMotion("Intro_Anim");
	scBBTextArea->SetMotionFrame(0.0f);
	scBBTextArea->m_MotionDisableFlag = false;
	scBBTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	scBBTextArea->m_MotionSpeed = 0.5f;
	scBBTextArea->Update();
	//L/R Bumpers
	scBBLRMove = prFittingScreenBB->CreateScene("LRmove");
	if (IsUnleashedHUD)
		scBBLRMove->SetHideFlag(true);
	scBBLRMove->SetMotion("Intro_Anim");
	scBBLRMove->SetMotionFrame(0.0f);
	scBBLRMove->m_MotionDisableFlag = false;
	scBBLRMove->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	scBBLRMove->m_MotionSpeed = 2.0f;
	scBBLRMove->Update();
	//Scroll Bar
	scBBScroll = prFittingScreenBB->CreateScene("scroll");
	if (IsUnleashedHUD)
		scBBScroll->SetHideFlag(true);
	scBBScroll->SetMotion("scroll_Anim");
	switch (CHudTabSel)
	{
	case UIPartShoes:
		scBBScroll->SetMotionFrame(Sh);
		break;
	case UIPartBody:
		scBBScroll->SetMotionFrame(Bd);
		break;
	case UIPartHead:
		scBBScroll->SetMotionFrame(He);
		break;
	case UIPartHandL:
		scBBScroll->SetMotionFrame(HL);
		break;
	case UIPartHandR:
		scBBScroll->SetMotionFrame(HR);
		break;
	case UISonicBody:
		scBBScroll->SetMotionFrame(SB);
		break;
	}
	scBBScroll->m_MotionSpeed = 0.0f;
	scBBScroll->Update();
	IsInScrollOpen = true;
	scBBScroll->SetPosition(0.9175f * arX, 0.5475f * arY);
	scBBScroll->SetMotion("Intro_Anim");
	scBBScroll->SetMotionFrame(0.0f);
	scBBScroll->m_MotionDisableFlag = false;
	scBBScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	scBBScroll->m_MotionSpeed = 1.0f;
	scBBScroll->Update();
	//Decoration
	if (ConfigDecoEnable && !IsUnleashedHUD)
	{
		scBBDeco = prFittingScreenBB->CreateScene("deco");
		scBBDeco->SetPosition(0, 0);
		scBBDeco->SetMotion("Color_change");
		scBBDeco->SetMotionFrame(1.0f);
		scBBDeco->m_MotionDisableFlag = false;
		scBBDeco->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBDeco->m_MotionSpeed = 0.0f;
		scBBDeco->Update();
		scBBDeco->SetMotion("Intro_Anim");
		scBBDeco->SetMotionFrame(0.0f);
		scBBDeco->m_MotionDisableFlag = false;
		scBBDeco->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBDeco->m_MotionSpeed = 1.0f;
		scBBDeco->Update();

	}
	//Unleashed HUD
	if (IsUnleashedHUD)
	{
		//BG
		scSWABG1 = prFittingScreenSWA->CreateScene("bg_1");
		scSWABG1->SetPosition(0, 0.025 * arY);
		scSWABG1->SetMotion("Size_Anim");
		scSWABG1->SetMotionFrame(100.0f);
		scSWABG1->m_MotionDisableFlag = false;
		scSWABG1->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scSWABG1->m_MotionSpeed = 0.0f;
		scSWABG1->Update();
		scSWABG1->SetMotion("Intro_Anim");
		scSWABG1->SetMotionFrame(0.0f);
		scSWABG1->m_MotionDisableFlag = false;
		scSWABG1->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scSWABG1->m_MotionSpeed = 1.5f;
		scSWABG1->Update();
		//Tag
		scSWATag = prFittingScreenSWA->CreateScene("tag");
		scSWATag->SetPosition(0, 0.025 * arY);
		scSWATag->SetMotion("Intro_3_Anim");
		scSWATag->SetMotionFrame(0.0f);
		scSWATag->m_MotionDisableFlag = false;
		scSWATag->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scSWATag->m_MotionSpeed = 1.0f;
		scSWATag->Update();
		//TagText
		scSWATagTxt = prFittingScreenSWA->CreateScene("tag_name_3");
		scSWATagTxt->SetPosition(0, 0.025 * arY);
		scSWATagTxt->SetMotion("Intro_Anim");
		scSWATagTxt->SetMotionFrame(0.0f);
		scSWATagTxt->m_MotionDisableFlag = false;
		scSWATagTxt->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scSWATagTxt->m_MotionSpeed = 1.0f;
		scSWATagTxt->Update();
		//Footer
		scSWAFooter = prFittingScreenSWA->CreateScene("footer_B");
		scSWAFooter->SetHideFlag(true);
		scSWAFooter->SetPosition(0.5 * arX, 0.333 * arY);
		scSWAFooter->SetScale(0.575, 0.575);
		scSWAFooter->SetMotion("Usual_Anim");
		scSWAFooter->SetMotionFrame(0.0f);
		scSWAFooter->m_MotionDisableFlag = false;
		scSWAFooter->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scSWAFooter->m_MotionSpeed = 1.0f;
		scSWAFooter->Update();
		//Arrows
		scSWAArrow = prFittingScreenSWA->CreateScene("arrow");
		scSWAArrow->SetPosition(0, 0.025 * arY);
		scSWAArrow->SetHideFlag(true);
		scSWAArrow->SetMotion("DefaultAnim");
		scSWAArrow->SetMotionFrame(0.0f);
		scSWAArrow->m_MotionDisableFlag = false;
		scSWAArrow->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scSWAArrow->m_MotionSpeed = 1.0f;
		scSWAArrow->Update();
		//SWACursor
		scSWASelect = prFittingScreenSWA->CreateScene("skill_select");
		scSWASelect->SetHideFlag(true);
		scSWASelect->SetPosition(rowX1 * arX, rowY1 * arY);
		scSWASelect->SetMotion("Usual_Anim");
		scSWASelect->SetMotionFrame(30.0f);
		scSWASelect->m_MotionDisableFlag = false;
		scSWASelect->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_Loop;
		scSWASelect->m_MotionSpeed = 1.0f;
		scSWASelect->Update();
		//SWAAltButton
		scSWAAlt = prFittingScreenSWA->CreateScene("footer_A");
		scSWAAlt->SetHideFlag(true);
		scSWAAlt->SetPosition(0, 0.025 * arY);
		scSWAAlt->SetMotion("Usual_Anim_2");
		scSWAAlt->SetMotionFrame(0.0f);
		scSWAAlt->m_MotionDisableFlag = false;
		scSWAAlt->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_Loop;
		scSWAAlt->m_MotionSpeed = 1.0f;
		scSWAAlt->Update();
		//SWAScroll
		scSWAScroll = prFittingScreenSWA->CreateScene("scroll_bar");
		scSWAScroll->SetHideFlag(true);
		scSWAScroll->SetScale(1, scaleY);
		scSWAScroll->SetMotion("Size_Anim");
		//scSWAScroll->SetPosition(0, 0.107 * arY);
		//scSWAScroll->SetMotionFrame(50.0f);
		switch (CHudTabSel)
		{
		case UIPartShoes:
			scSWAScroll->SetPosition(0, (0.107 + (Sh * (0.001 * (CHudVarShMaxScroll * 0.47)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarShMaxScroll);
			break;
		case UIPartBody:
			scSWAScroll->SetPosition(0, (0.107 + (Bd * (0.001 * (CHudVarBdMaxScroll * 0.95)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarBdMaxScroll);
			break;
		case UIPartHead:
			scSWAScroll->SetPosition(0, (0.107 + (He * (0.001 * (CHudVarHeMaxScroll * 1.06)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHeMaxScroll);
			break;
		case UIPartHandL:
			scSWAScroll->SetPosition(0, (0.107 + (HL * (0.001 * (CHudVarHLMaxScroll * 0.8)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHLMaxScroll);
			break;
		case UIPartHandR:
			scSWAScroll->SetPosition(0, (0.107 + (HR * (0.001 * (CHudVarHRMaxScroll * 0.685)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHRMaxScroll);
			break;
		}
		scSWAScroll->m_MotionDisableFlag = false;
		scSWAScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_Loop;
		scSWAScroll->m_MotionSpeed = 0.0f;
		scSWAScroll->Update();
		//SWAScrollBG
		scSWAScrollBG = prFittingScreenSWA->CreateScene("scroll_bar_bg");
		scSWAScrollBG->SetHideFlag(true);
		scSWAScrollBG->SetScale(1, scaleY);
		scSWAScrollBG->SetPosition(0, 0.107 * arY);
		scSWAScrollBG->SetMotion("Scroll_Anim");
		scSWAScrollBG->SetMotionFrame(0.0f);
		scSWAScrollBG->m_MotionDisableFlag = false;
		scSWAScrollBG->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_Loop;
		scSWAScrollBG->m_MotionSpeed = 0.0f;
		scSWAScrollBG->Update();

	}
	//Misc
	IsInMenu = true;
	CHudUISFXOpen();
	return;
}

void CHudUISelect()
{
	//SWAOpenTimer = 10;
	isLoadModel = true;
	switch (CHudTabSel)
	{
	case UIPartShoes:
		if (CHudVarTrueSel >= ShOverflow00)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectShoes = (enum SelectShoeType)CHudVarTrueSel;
		}
		return;
		break;
	case UIPartBody:
		if (CHudVarTrueSel >= BdOverflow00)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectBody = (enum SelectBodyType)CHudVarTrueSel;
		}
		return;
		break;
	case UIPartHead:
		if (CHudVarTrueSel >= HeOverflow00)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHead = (enum SelectHeadType)CHudVarTrueSel;
		}
		return;
		break;
	case UIPartHandL:
		if (CHudVarTrueSel >= HLOverflow00)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = (enum SelectHandLType)CHudVarTrueSel;
		}
		return;
		break;
	case UIPartHandR:
		if (CHudVarTrueSel >= HROverflow00)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandR = (enum SelectHandRType)CHudVarTrueSel;
		}
		return;
		break;
	case UISonicBody:
		if (CHudVarTrueSel >= SBOverflow06)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			switch (CHudVarTrueSel)
			{
			case (enum SelectSonicBodyType)SBSnMaterial:
				if (SelectSnSonMat != SnMatBlack)
					SelectSnSonMat = (SelectSnSonMatType)(SelectSnSonMat + 1);
				else
					SelectSnSonMat = SnMatOriginal;
				return;
				break;
			case (enum SelectSonicBodyType)SBSsnMaterial:
				if (SelectSsnSonMat != SsnMatPurple)
					SelectSsnSonMat = (SelectSsnSonMatType)(SelectSsnSonMat + 1);
				else
					SelectSsnSonMat = SsnMatOriginal;
				return;
				break;
			case (enum SelectSonicBodyType)SBEyelids:
				
				return;
				break;
			case (enum SelectSonicBodyType)SBSuperHead:

				return;
				break;
			case (enum SelectSonicBodyType)SBSuperForm:

				return;
				break;
			case (enum SelectSonicBodyType)SBJumpball:
				if (SelectJumpBall != JumpBallNoBall)
					SelectJumpBall = (SelectJumpBallType)(SelectJumpBall + 1);
				else
					SelectJumpBall = JumpBallDefault;
				return;
				break;
			}
		}
		return;
		break;
	}
}

void CHudUIMove(int Type)
{
	CHudUISFXMove();
	CHudUICursorAnim();
	switch (CHudTabSel)
	{
	case UIPartShoes:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll <= 0)
				{
					CHudVarVisSel += 6;
					CHudVarScroll = CHudVarShMaxScroll;
				}
				else if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			//CHudVarScroll--;
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll >= CHudVarShMaxScroll)
				{
					CHudVarVisSel -= 6;
					CHudVarScroll = 0;
				}
				else if (CHudVarScroll < CHudVarShMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			//CHudVarScroll++;
			return;
			break;
		case 2: //Left
			if (CHudVarVisSel <= 0 && CHudVarScroll <= 0)
			{
				CHudVarScroll = CHudVarShMaxScroll;
				CHudVarVisSel = 8;
			}
			else if (CHudVarVisSel <= 0 && CHudVarScroll > 0)
			{
				CHudVarScroll--;
				CHudVarVisSel += 2;
			}
			else
				CHudVarVisSel--;
			return;
			break;
		case 3: //Right
			if (CHudVarVisSel >= 8 && CHudVarScroll >= CHudVarShMaxScroll)
			{
				CHudVarScroll = 0;
				CHudVarVisSel = 0;
			}
			else if (CHudVarVisSel >= 8 && CHudVarScroll < CHudVarShMaxScroll)
			{
				CHudVarScroll++;
				CHudVarVisSel -= 2;
			}
			else
				CHudVarVisSel++;
			return;
			break;
		}
		return;
		break;
	case UIPartBody:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll <= 0)
				{
					CHudVarVisSel += 6;
					CHudVarScroll = CHudVarBdMaxScroll;
				}
				else if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll >= CHudVarBdMaxScroll)
				{
					CHudVarVisSel -= 6;
					CHudVarScroll = 0;
				}
				else if (CHudVarScroll < CHudVarBdMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			return;
			break;
		case 2: //Left
			if (CHudVarVisSel <= 0 && CHudVarScroll <= 0)
			{
				CHudVarScroll = CHudVarBdMaxScroll;
				CHudVarVisSel = 8;
			}
			else if (CHudVarVisSel <= 0 && CHudVarScroll > 0)
			{
				CHudVarScroll--;
				CHudVarVisSel += 2;
			}
			else
				CHudVarVisSel--;
			return;
			break;
		case 3: //Right
			if (CHudVarVisSel >= 8 && CHudVarScroll >= CHudVarBdMaxScroll)
			{
				CHudVarScroll = 0;
				CHudVarVisSel = 0;
			}
			else if (CHudVarVisSel >= 8 && CHudVarScroll < CHudVarBdMaxScroll)
			{
				CHudVarScroll++;
				CHudVarVisSel -= 2;
			}
			else
				CHudVarVisSel++;
			return;
			break;
		}
		return;
		break;
	case UIPartHead:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll <= 0)
				{
					CHudVarVisSel += 6;
					CHudVarScroll = CHudVarHeMaxScroll;
				}
				else if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll >= CHudVarHeMaxScroll)
				{
					CHudVarVisSel -= 6;
					CHudVarScroll = 0;
				}
				else if (CHudVarScroll < CHudVarHeMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			return;
			break;
		case 2: //Left
			if (CHudVarVisSel <= 0 && CHudVarScroll <= 0)
			{
				CHudVarScroll = CHudVarHeMaxScroll;
				CHudVarVisSel = 8;
			}
			else if (CHudVarVisSel <= 0 && CHudVarScroll > 0)
			{
				CHudVarScroll--;
				CHudVarVisSel += 2;
			}
			else
				CHudVarVisSel--;
			return;
			break;
		case 3: //Right
			if (CHudVarVisSel >= 8 && CHudVarScroll >= CHudVarHeMaxScroll)
			{
				CHudVarScroll = 0;
				CHudVarVisSel = 0;
			}
			else if (CHudVarVisSel >= 8 && CHudVarScroll < CHudVarHeMaxScroll)
			{
				CHudVarScroll++;
				CHudVarVisSel -= 2;
			}
			else
				CHudVarVisSel++;
			return;
			break;
		}
		return;
		break;
	case UIPartHandL:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll <= 0)
				{
					CHudVarVisSel += 6;
					CHudVarScroll = CHudVarHLMaxScroll;
				}
				else if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll >= CHudVarHLMaxScroll)
				{
					CHudVarVisSel -= 6;
					CHudVarScroll = 0;
				}
				else if (CHudVarScroll < CHudVarHLMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			return;
			break;
		case 2: //Left
			if (CHudVarVisSel <= 0 && CHudVarScroll <= 0)
			{
				CHudVarScroll = CHudVarHLMaxScroll;
				CHudVarVisSel = 8;
			}
			else if (CHudVarVisSel <= 0 && CHudVarScroll > 0)
			{
				CHudVarScroll--;
				CHudVarVisSel += 2;
			}
			else
				CHudVarVisSel--;
			return;
			break;
		case 3: //Right
			if (CHudVarVisSel >= 8 && CHudVarScroll >= CHudVarHLMaxScroll)
			{
				CHudVarScroll = 0;
				CHudVarVisSel = 0;
			}
			else if (CHudVarVisSel >= 8 && CHudVarScroll < CHudVarHLMaxScroll)
			{
				CHudVarScroll++;
				CHudVarVisSel -= 2;
			}
			else
				CHudVarVisSel++;
			return;
			break;
		}
		return;
		break;
	case UIPartHandR:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll <= 0)
				{
					CHudVarVisSel += 6;
					CHudVarScroll = CHudVarHRMaxScroll;
				}
				else if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll >= CHudVarHRMaxScroll)
				{
					CHudVarVisSel -= 6;
					CHudVarScroll = 0;
				}
				else if (CHudVarScroll < CHudVarHRMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			return;
			break;
		case 2: //Left
			if (CHudVarVisSel <= 0 && CHudVarScroll <= 0)
			{
				CHudVarScroll = CHudVarHRMaxScroll;
				CHudVarVisSel = 8;
			}
			else if (CHudVarVisSel <= 0 && CHudVarScroll > 0)
			{
				CHudVarScroll--;
				CHudVarVisSel += 2;
			}
			else
				CHudVarVisSel--;
			return;
			break;
		case 3: //Right
			if (CHudVarVisSel >= 8 && CHudVarScroll >= CHudVarHRMaxScroll)
			{
				CHudVarScroll = 0;
				CHudVarVisSel = 0;
			}
			else if (CHudVarVisSel >= 8 && CHudVarScroll < CHudVarHRMaxScroll)
			{
				CHudVarScroll++;
				CHudVarVisSel -= 2;
			}
			else
				CHudVarVisSel++;
			return;
			break;
		}
		return;
		break;
	case UISonicBody:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll <= 0)
				{
					CHudVarVisSel += 6;
					CHudVarScroll = CHudVarSBMaxScroll;
				}
				else if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			//CHudVarScroll--;
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll >= CHudVarSBMaxScroll)
				{
					CHudVarVisSel -= 6;
					CHudVarScroll = 0;
				}
				else if (CHudVarScroll < CHudVarSBMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			//CHudVarScroll++;
			return;
			break;
		case 2: //Left
			if (CHudVarVisSel <= 0 && CHudVarScroll <= 0)
			{
				CHudVarScroll = CHudVarSBMaxScroll;
				CHudVarVisSel = 8;
			}
			else if (CHudVarVisSel <= 0 && CHudVarScroll > 0)
			{
				CHudVarScroll--;
				CHudVarVisSel += 2;
			}
			else
				CHudVarVisSel--;
			return;
			break;
		case 3: //Right
			if (CHudVarVisSel >= 8 && CHudVarScroll >= CHudVarSBMaxScroll)
			{
				CHudVarScroll = 0;
				CHudVarVisSel = 0;
			}
			else if (CHudVarVisSel >= 8 && CHudVarScroll < CHudVarSBMaxScroll)
			{
				CHudVarScroll++;
				CHudVarVisSel -= 2;
			}
			else
				CHudVarVisSel++;
			return;
			break;
		}
		return;
		break;
	}

}

void CHudUIAlt()
{
	//isLoadModel = true;
	switch (CHudTabSel)
	{
	case UIPartShoes:
		switch (CHudVarTrueSel)
		{
		case ShDefault:
			switch (ShDefaultVariant)
			{
			case DefaultDefault:
				ShDefaultVariant = DefaultLightS;
				break;
			case DefaultLightS:
				ShDefaultVariant = DefaultDefault;
				break;
			}
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		case ShSA2Beta:
			switch (ShSA2BetaVariant)
			{
			case SA2Beta:
				ShSA2BetaVariant = SA2BetaLightS;
				break;
			case SA2BetaLightS:
				ShSA2BetaVariant = SA2Beta;
				break;
			}
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		case ShSA2Soap:
			switch (ShSA2SoapVariant)
			{
			case SA2Soap:
				ShSA2SoapVariant = SA2SoapLightS;
				break;
			case SA2SoapLightS:
				ShSA2SoapVariant = SA2SoapRacing;
				break;
			case SA2SoapRacing:
				ShSA2SoapVariant = SA2Soap;
				break;
			}
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		case Sh06Gem:
			switch (Sh06GemVariant)
			{
			case GemDefault:
				Sh06GemVariant = GemRed;
				break;
			case GemRed:
				Sh06GemVariant = GemBlue;
				break;
			case GemBlue:
				Sh06GemVariant = GemGreen;
				break;
			case GemGreen:
				Sh06GemVariant = GemPurple;
				break;
			case GemPurple:
				Sh06GemVariant = GemSky;
				break;
			case GemSky:
				Sh06GemVariant = GemWhite;
				break;
			case GemWhite:
				Sh06GemVariant = GemYellow;
				break;
			case GemYellow:
				Sh06GemVariant = GemDefault;
				break;
			}
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		}
		return;
		break;
	case UIPartBody:
		switch (CHudVarTrueSel)
		{
		case BdDefault:
			if (BdDefaultVariant == false) BdDefaultVariant = true; else BdDefaultVariant = false;
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		case BdWildFire:
			if (BdWildFireVariant == false) BdWildFireVariant = true; else BdWildFireVariant = false;
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		case BdScarf:
			if (BdScarfVariant == false) BdScarfVariant = true; else BdScarfVariant = false;
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		case BdMovieSkin:
			if (BdMovieVariant == false) BdMovieVariant = true; else BdMovieVariant = false;
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		}
		return;
		break;
	case UIPartHead:
		switch (CHudVarTrueSel)
		{
		case HeSimulator:
			if (HeSimulatorVariant == false) HeSimulatorVariant = true; else HeSimulatorVariant = false;
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		}
		return;
		break;
	case UIPartHandR:
		switch (CHudVarTrueSel)
		{
		case HRSA2Bounce:
			if (HRSA2BounceVariant == false) HRSA2BounceVariant = true; else HRSA2BounceVariant = false;
			CHudUISFXAlt();
			CHudUICursorAnim();
			isLoadModel = true;
			return;
			break;
		}
		return;
		break;
	}
}

void CHudUISwitch(int Type)
{
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedLB = input.IsTapped(Sonic::eKeyState_LeftBumper);
	bool PressedRB = input.IsTapped(Sonic::eKeyState_RightBumper);

	if (Type == 0)
	{
		CHudUISFXSwitch();
		IsInMenuChange = true;
		if (PressedRB)
		{
			IsInMenuChangeR = true;
			scBBLRMove->SetMotion("Right_ON_Anim");
			scBBLRMove->SetMotionFrame(0.0f);
			scBBLRMove->m_MotionDisableFlag = false;
			scBBLRMove->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scBBLRMove->m_MotionSpeed = 1.0f;
			scBBLRMove->Update();
		}
		if (PressedLB)
		{
			IsInMenuChangeL = true;
			scBBLRMove->SetMotion("Left_ON_Anim");
			scBBLRMove->SetMotionFrame(0.0f);
			scBBLRMove->m_MotionDisableFlag = false;
			scBBLRMove->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scBBLRMove->m_MotionSpeed = 1.0f;
			scBBLRMove->Update();
		}
		IsInMenuExit = false;
		//MenuOption = 0;
		//ItemOption = 0;
		SWAOpenTimer = 25;
		scBBGui->SetMotion("Intro_Anim");
		scBBGui->SetMotionFrame(22.0f);
		scBBGui->m_MotionDisableFlag = false;
		scBBGui->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBGui->m_MotionSpeed = -2.0f;
		scBBGui->Update();
		scBBIcon->SetMotion("OFF_Anim");
		scBBIcon->SetMotionFrame(0.0f);
		scBBIcon->m_MotionDisableFlag = false;
		scBBIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBIcon->m_MotionSpeed = 1.0f;
		scBBIcon->Update();
		scBBTextArea->SetMotion("Intro_Anim");
		scBBTextArea->SetMotionFrame(17.0f);
		scBBTextArea->m_MotionDisableFlag = false;
		scBBTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBTextArea->m_MotionSpeed = -1.8f;
		scBBTextArea->Update();
		IsInScrollOpen = true;
		scBBScroll->SetMotion("Intro_Anim");
		scBBScroll->SetMotionFrame(22.0f);
		scBBScroll->m_MotionDisableFlag = false;
		scBBScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBScroll->m_MotionSpeed = -2.0f;
		scBBScroll->Update();
		if (IsUnleashedHUD)
		{
			scSWABG1->SetMotion("Intro_Anim");
			scSWABG1->SetMotionFrame(0.0f);
			scSWABG1->m_MotionDisableFlag = false;
			scSWABG1->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scSWABG1->m_MotionSpeed = -2.5f;
			scSWABG1->Update();

			scSWATag->SetMotion("Intro_3_Anim");
			scSWATag->SetMotionFrame(0.0f);
			scSWATag->m_MotionDisableFlag = false;
			scSWATag->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scSWATag->m_MotionSpeed = -1.0f;
			scSWATag->Update();

			scSWATagTxt->SetMotion("Intro_Anim");
			scSWATagTxt->SetMotionFrame(0.0f);
			scSWATagTxt->m_MotionDisableFlag = false;
			scSWATagTxt->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scSWATagTxt->m_MotionSpeed = -1.0f;
			scSWATagTxt->Update();
		}
		return;
	}
	if (Type == 1)
	{
		IsInMenuChange = false;
		//isLoadModel = true;
		CHudVarVisSel = 0;
		CHudVarScroll = 0;
		CHudVarTrueSel = 0;
		if (IsInMenuChangeR)
		{
			switch (CHudTabSel)
			{
			case UIPartShoes:
				CHudTabSel = UIPartBody;
				break;
			case UIPartBody:
				CHudTabSel = UIPartHead;
				break;
			case UIPartHead:
				CHudTabSel = UIPartHandL;
				break;
			case UIPartHandL:
				CHudTabSel = UIPartHandR;
				break;
			case UIPartHandR:
				CHudTabSel = UISonicBody;
				break;
			case UISonicBody:
				CHudTabSel = UIPartShoes;
				break;
			}
		}
		if (IsInMenuChangeL)
		{
			switch (CHudTabSel)
			{
			case UIPartShoes:
				CHudTabSel = UISonicBody;
				break;
			case UIPartBody:
				CHudTabSel = UIPartShoes;
				break;
			case UIPartHead:
				CHudTabSel = UIPartBody;
				break;
			case UIPartHandL:
				CHudTabSel = UIPartHead;
				break;
			case UIPartHandR:
				CHudTabSel = UIPartHandL;
				break;
			case UISonicBody:
				CHudTabSel = UIPartHandR;
				break;
			}
		}
		IsInMenuChangeR = false;
		IsInMenuChangeL = false;
		scBBGui->SetMotion("Intro_Anim");
		scBBGui->SetMotionFrame(0.0f);
		scBBGui->m_MotionDisableFlag = false;
		scBBGui->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBGui->m_MotionSpeed = 2.0f;
		scBBGui->Update();
		CHudUICursorAnim();
		scBBTextArea->SetMotion("Intro_Anim");
		scBBTextArea->SetMotionFrame(0.0f);
		scBBTextArea->m_MotionDisableFlag = false;
		scBBTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBTextArea->m_MotionSpeed = 2.0f;
		scBBTextArea->Update();
		scBBScroll->SetMotion("Intro_Anim");
		scBBScroll->SetMotionFrame(15.0f);
		scBBScroll->m_MotionDisableFlag = false;
		scBBScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBScroll->m_MotionSpeed = 1.0f;
		scBBScroll->Update();
		if (IsUnleashedHUD)
		{
			scSWABG1->SetMotion("Intro_Anim");
			scSWABG1->SetMotionFrame(100.0f);
			scSWABG1->m_MotionDisableFlag = false;
			scSWABG1->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scSWABG1->m_MotionSpeed = 1.5f;
			scSWABG1->Update();

			scSWATag->SetMotion("Intro_3_Anim");
			scSWATag->SetMotionFrame(15.0f);
			scSWATag->m_MotionDisableFlag = false;
			scSWATag->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scSWATag->m_MotionSpeed = 1.5f;
			scSWATag->Update();

			scSWATagTxt->SetMotion("Intro_Anim");
			scSWATagTxt->SetMotionFrame(15.0f);
			scSWATagTxt->m_MotionDisableFlag = false;
			scSWATagTxt->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scSWATagTxt->m_MotionSpeed = 1.5f;
			scSWATagTxt->Update();
		}
		return;
	}
}

void CHudUIExit(int Type)
{
	if (Type == 0)
	{
		CHudUISFXExit();
		IsInMenuExit = true;
		SWAOpenTimer = 15;
		scBBGui->SetMotion("Intro_Anim");
		scBBGui->SetMotionFrame(22.0f);
		scBBGui->m_MotionDisableFlag = false;
		scBBGui->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBGui->m_MotionSpeed = -1.0f;
		scBBGui->Update();
		scBBIcon->SetMotion("OFF_Anim");
		scBBIcon->SetMotionFrame(0.0f);
		scBBIcon->m_MotionDisableFlag = false;
		scBBIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBIcon->m_MotionSpeed = 1.0f;
		scBBIcon->Update();
		scBBTextArea->SetMotion("Intro_Anim");
		scBBTextArea->SetMotionFrame(17.0f);
		scBBTextArea->m_MotionDisableFlag = false;
		scBBTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBTextArea->m_MotionSpeed = -0.5f;
		scBBTextArea->Update();
		scBBLRMove->SetMotion("Intro_Anim");
		scBBLRMove->SetMotionFrame(0.0f);
		scBBLRMove->m_MotionDisableFlag = false;
		scBBLRMove->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBLRMove->m_MotionSpeed = -1.0f;
		scBBLRMove->Update();
		IsInScrollOpen = true;
		scBBScroll->SetMotion("Intro_Anim");
		scBBScroll->SetMotionFrame(22.0f);
		scBBScroll->m_MotionDisableFlag = false;
		scBBScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBScroll->m_MotionSpeed = -1.0f;
		scBBScroll->Update();
		if (scBBDeco)
		{
			scBBDeco->SetMotion("Intro_Anim");
			scBBDeco->SetMotionFrame(23.0f);
			scBBDeco->m_MotionDisableFlag = false;
			scBBDeco->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scBBDeco->m_MotionSpeed = -1.0f;
			scBBDeco->Update();
		}
		if (scSWABG1)
		{
			scSWABG1->SetMotion("Intro_Anim");
			scSWABG1->SetMotionFrame(23.0f);
			scSWABG1->m_MotionDisableFlag = false;
			scSWABG1->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scSWABG1->m_MotionSpeed = -0.9f;
			scSWABG1->Update();
		}
		if (scSWATag)
		{
			scSWATag->SetMotion("Intro_3_Anim");
			scSWATag->SetMotionFrame(15.0f);
			scSWATag->m_MotionDisableFlag = false;
			scSWATag->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scSWATag->m_MotionSpeed = -0.5f;
			scSWATag->Update();
		}
		if (scSWATagTxt)
		{
			scSWATagTxt->SetMotion("Intro_Anim");
			scSWATagTxt->SetMotionFrame(15.0f);
			scSWATagTxt->m_MotionDisableFlag = false;
			scSWATagTxt->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scSWATagTxt->m_MotionSpeed = -0.5f;
			scSWATagTxt->Update();
		}
		if (scSWAFooter)
		{
			SWAOpenTimer = 15;
			scSWAFooter->SetHideFlag(true);
			scSWAFooter->Update();
		}
		if (scSWAArrow)
		{
			SWAOpenTimer = 15;
			scSWAArrow->SetHideFlag(true);
			scSWAArrow->Update();
		}
		if (scSWAAlt)
		{
			SWAOpenTimer = 15;
			scSWAAlt->SetHideFlag(true);
			scSWAAlt->Update();
		}
		if (scSWASelect)
		{
			SWAOpenTimer = 15;
			scSWASelect->SetHideFlag(true);
			scSWASelect->Update();
		}
		if (scSWAScroll)
		{
			SWAOpenTimer = 15;
			scSWAScroll->SetHideFlag(true);
			scSWAScroll->Update();
		}
		if (scSWAScrollBG)
		{
			SWAOpenTimer = 15;
			scSWAScrollBG->SetHideFlag(true);
			scSWAScrollBG->Update();
		}
		return;
	}
	if (Type == 1)
	{
		IsInMenuExit = false;
		IsInMenu = false;
		//isLoadModel = true;
		MemoryOpenTimer = 1800;
		*ENABLE_BLUR = prevblur;
		FILE* pFile = fopen(saveFilePath.c_str(), "wb");
		WriteINI(pFile);
		fclose(pFile);
		CHudUISceneDestroy();
		return;
	}
}

void CHudUITexsetData(Sonic::CGameObject* This, int texSetID, const char* texString)
{
	auto texList = static_cast<Sonic::CCsdTexListMirage*>(prFittingScreenBB->m_rcTexList.Get());
	auto ui_cat_item_alt_sel = boost::make_shared<hh::mr::CTextureData>();
	ui_cat_item_alt_sel->m_spPictureData = hh::mr::CMirageDatabaseWrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get()).GetPictureData(texString);

	texList->m_spTexsetData->m_TextureList[texSetID] = ui_cat_item_alt_sel;
}

const char* CHudUIThumbShoeString(int id, char* result)
{
	auto mapChar = MAP_FILE_SHOE[ShDefault + (CHudVarScroll * 3) + id];
	auto mapInt = (ShDefault + (CHudVarScroll * 3) + id);
	const char* texExtUI = "ui_";
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	const char* texExtVar = "_00";
	const char* texExt0 = "_00";
	const char* texExt1 = "_01";
	const char* texExt2 = "_02";
	const char* texExt3 = "_03";
	const char* texExt4 = "_04";
	const char* texExt5 = "_05";
	const char* texExt6 = "_06";
	const char* texExt7 = "_07";
	const char* texExt8 = "_08";
	const char* texExt9 = "_09";
	bool active = (mapInt == SelectShoes);
	bool isShDefault = (mapInt == ShDefault);
	bool isShDefaultLightS = (ShDefaultVariant == DefaultLightS);
	bool isShSA2Beta = (mapInt == ShSA2Beta);
	bool isShSA2BetaLightS = (ShSA2BetaVariant == SA2BetaLightS);
	bool isShSA2Soap = (mapInt == ShSA2Soap);
	bool isShSA2SoapLightS = (ShSA2SoapVariant == SA2SoapLightS);
	bool isShSA2SoapRacing = (ShSA2SoapVariant == SA2SoapRacing);
	bool isSh06Gem = (mapInt == Sh06Gem);
	bool isSh06GemRed = (Sh06GemVariant == GemRed);
	bool isSh06GemBlue = (Sh06GemVariant == GemBlue);
	bool isSh06GemGreen = (Sh06GemVariant == GemGreen);
	bool isSh06GemPurple = (Sh06GemVariant == GemPurple);
	bool isSh06GemSky = (Sh06GemVariant == GemSky);
	bool isSh06GemWhite = (Sh06GemVariant == GemWhite);
	bool isSh06GemYellow = (Sh06GemVariant == GemYellow);

	if ((isShDefault && isShDefaultLightS) || (isShSA2Beta && isShSA2BetaLightS) || (isShSA2Soap && isShSA2SoapLightS) || (isSh06Gem && isSh06GemRed))
		texExtVar = texExt1;
	else if ((isShSA2Soap && isShSA2SoapRacing) || (isSh06Gem && isSh06GemBlue))
		texExtVar = texExt2;
	else if (isSh06Gem && isSh06GemGreen)
		texExtVar = texExt3;
	else if (isSh06Gem && isSh06GemPurple)
		texExtVar = texExt4;
	else if (isSh06Gem && isSh06GemSky)
		texExtVar = texExt5;
	else if (isSh06Gem && isSh06GemWhite)
		texExtVar = texExt6;
	else if (isSh06Gem && isSh06GemYellow)
		texExtVar = texExt7;

	sprintf(result, "%s%s%s%s", texExtUI, mapChar, texExtVar, active ? texExtOn : texExtOff);

	return result;
}
const char* CHudUIThumbBodyString(int id, char* result)
{
	auto mapChar = MAP_FILE_BODY[BdDefault + (CHudVarScroll * 3) + id];
	auto mapInt = (BdDefault + (CHudVarScroll * 3) + id);
	const char* texExtUI = "ui_";
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	const char* texExtVar = "_00";
	const char* texExt0 = "_00";
	const char* texExt1 = "_01";
	const char* texExt2 = "_02";
	const char* texExt3 = "_03";
	const char* texExt4 = "_04";
	const char* texExt5 = "_05";
	const char* texExt6 = "_06";
	const char* texExt7 = "_07";
	const char* texExt8 = "_08";
	const char* texExt9 = "_09";
	bool active = (mapInt == SelectBody);
	bool isBdDefault = (mapInt == BdDefault);

	if ((isBdDefault && BdDefaultVariant))
		texExtVar = texExt1;

	sprintf(result, "%s%s%s%s", texExtUI, mapChar, texExtVar, active ? texExtOn : texExtOff);

	return result;
}
const char* CHudUIThumbHeadString(int id, char* result)
{
	auto mapChar = MAP_FILE_HEAD[HeDefault + (CHudVarScroll * 3) + id];
	auto mapInt = (HeDefault + (CHudVarScroll * 3) + id);
	const char* texExtUI = "ui_";
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	const char* texExtVar = "_00";
	const char* texExt0 = "_00";
	const char* texExt1 = "_01";
	const char* texExt2 = "_02";
	const char* texExt3 = "_03";
	const char* texExt4 = "_04";
	const char* texExt5 = "_05";
	const char* texExt6 = "_06";
	const char* texExt7 = "_07";
	const char* texExt8 = "_08";
	const char* texExt9 = "_09";
	bool active = (mapInt == SelectHead);
	bool isHeSimulator = (mapInt == HeSimulator);

	if ((isHeSimulator && HeSimulatorVariant))
		texExtVar = texExt1;

	sprintf(result, "%s%s%s%s", texExtUI, mapChar, texExtVar, active ? texExtOn : texExtOff);

	return result;
}
const char* CHudUIThumbHandLString(int id, char* result)
{
	auto mapChar = MAP_FILE_HANDL[HLDefault + (CHudVarScroll * 3) + id];
	auto mapInt = (HLDefault + (CHudVarScroll * 3) + id);
	const char* texExtUI = "ui_";
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	const char* texExtVar = "_00";
	const char* texExt0 = "_00";
	const char* texExt1 = "_01";
	const char* texExt2 = "_02";
	const char* texExt3 = "_03";
	const char* texExt4 = "_04";
	const char* texExt5 = "_05";
	const char* texExt6 = "_06";
	const char* texExt7 = "_07";
	const char* texExt8 = "_08";
	const char* texExt9 = "_09";
	bool active = (mapInt == SelectHandL);
	//bool isHLDefault = (mapInt == HLDefault);

	//if ((isHLDefault && HLDefaultVariant))
	//	texExtVar = texExt1;

	sprintf(result, "%s%s%s%s", texExtUI, mapChar, texExtVar, active ? texExtOn : texExtOff);

	return result;
}
const char* CHudUIThumbHandRString(int id, char* result)
{
	auto mapChar = MAP_FILE_HANDR[HRDefault + (CHudVarScroll * 3) + id];
	auto mapInt = (HRDefault + (CHudVarScroll * 3) + id);
	const char* texExtUI = "ui_";
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	const char* texExtVar = "_00";
	const char* texExt0 = "_00";
	const char* texExt1 = "_01";
	const char* texExt2 = "_02";
	const char* texExt3 = "_03";
	const char* texExt4 = "_04";
	const char* texExt5 = "_05";
	const char* texExt6 = "_06";
	const char* texExt7 = "_07";
	const char* texExt8 = "_08";
	const char* texExt9 = "_09";
	bool active = (mapInt == SelectHandR);
	bool isHRSA2Bounce = (mapInt == HRSA2Bounce);

	if ((isHRSA2Bounce && HRSA2BounceVariant))
		texExtVar = texExt1;

	sprintf(result, "%s%s%s%s", texExtUI, mapChar, texExtVar, active ? texExtOn : texExtOff);

	return result;
}
const char* CHudUIThumbSonicBodyString(int id, char* result)
{
	auto mapChar = MAP_FILE_SONICBODY[SBSnMaterial + (CHudVarScroll * 3) + id];
	auto mapInt = (SBSnMaterial + (CHudVarScroll * 3) + id);
	const char* texExtUI = "ui_";
	const char* texExtVar = "_00";
	const char* texExt0 = "_00";
	const char* texExt1 = "_01";
	const char* texExt2 = "_02";
	const char* texExt3 = "_03";
	const char* texExt4 = "_04";
	const char* texExt5 = "_05";
	const char* texExt6 = "_06";
	const char* texExt7 = "_07";
	const char* texExt8 = "_08";
	const char* texExt9 = "_09";

	sprintf(result, "%s%s%s", texExtUI, mapChar, texExtVar);
	//printf("%s%s%s", texExtUI, mapChar, texExtVar);

	return result;
}

void CHudUIThumbManager(Sonic::CGameObject* This)
{
	char ShBuffer[256];
	char BdBuffer[256];
	char HeBuffer[256];
	char HLBuffer[256];
	char HRBuffer[256];
	char SBBuffer[256];

	switch (CHudTabSel)
	{
	case UIPartShoes:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbShoeString(i, ShBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartShoes);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartShoes);
		return;
		break;
	case UIPartBody:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbBodyString(i, BdBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartBody);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartBody);
		return;
		break;
	case UIPartHead:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbHeadString(i, HeBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartHead);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartHead);
		return;
		break;
	case UIPartHandL:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbHandLString(i, HLBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartHandL);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartHandL);
		return;
		break;
	case UIPartHandR:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbHandRString(i, HRBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartHandR);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartHandR);
		return;
		break;
	case UISonicBody:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbSonicBodyString(i, SBBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UISonicBody);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UISonicBody);
		return;
		break;
	}
	
	//auto ui_sh_ShDefault_00_On = boost::make_shared<hh::mr::CTextureData>();
	//ui_sh_ShDefault_00_On->m_spPictureData = hh::mr::CMirageDatabaseWrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get()).GetPictureData(MAP_THUMB_SHOE[ShDefault]);
	//auto texList = static_cast<Sonic::CCsdTexListMirage*>(prFittingScreenBB->m_rcTexList.Get());
	// ((ShDefault + (CHudVarScroll * 3) + 0) == SelectShoes)
	//CHudUITexsetData(This,  6, MAP_THUMB_SHOE[ShDefault + (CHudVarScroll * 3) + 0]);

}

void CHudUIScrollManager(Sonic::CGameObject* This, float Sh, float Bd, float He, float HL, float HR, float SB)
{
	auto arY = 720.0f;
	auto scaleY = 0.88642f;
	scBBScroll->SetMotion("scroll_Anim");
	switch (CHudTabSel)
	{
	case UIPartShoes:
		scBBScroll->SetMotionFrame(Sh);
		break;
	case UIPartBody:
		scBBScroll->SetMotionFrame(Bd);
		break;
	case UIPartHead:
		scBBScroll->SetMotionFrame(He);
		break;
	case UIPartHandL:
		scBBScroll->SetMotionFrame(HL);
		break;
	case UIPartHandR:
		scBBScroll->SetMotionFrame(HR);
		break;
	case UISonicBody:
		scBBScroll->SetMotionFrame(SB);
		break;
	}
	scBBScroll->m_MotionSpeed = 0.0f;
	scBBScroll->Update();
	if (IsUnleashedHUD)
	{
		switch (CHudTabSel)
		{
		case UIPartShoes:
			scSWAScroll->SetPosition(0, (0.107 + (Sh * (0.001 * (CHudVarShMaxScroll * 0.47)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarShMaxScroll);
			break;
		case UIPartBody:
			scSWAScroll->SetPosition(0, (0.107 + (Bd * (0.001 * (CHudVarBdMaxScroll * 0.95)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarBdMaxScroll);
			break;
		case UIPartHead:
			scSWAScroll->SetPosition(0, (0.107 + (He * (0.001 * (CHudVarHeMaxScroll * 1.06)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHeMaxScroll);
			break;
		case UIPartHandL:
			scSWAScroll->SetPosition(0, (0.107 + (HL * (0.001 * (CHudVarHLMaxScroll * 0.8)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHLMaxScroll);
			break;
		case UIPartHandR:
			scSWAScroll->SetPosition(0, (0.107 + (HR * (0.001 * (CHudVarHRMaxScroll * 0.685)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHRMaxScroll);
			break;
		case UISonicBody:
			scSWAScroll->SetPosition(0, 0.107 * arY);
			scSWAScroll->SetMotionFrame(0);
			break;
		}
	}
}

void CHudFittingMenu(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
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
	bool PressedLST = input.IsTapped(Sonic::eKeyState_LeftStick);
	bool PressedRST = input.IsTapped(Sonic::eKeyState_RightStick);
	bool DownLB = input.IsDown(Sonic::eKeyState_LeftBumper);
	bool DownRB = input.IsDown(Sonic::eKeyState_RightBumper);
	bool DownLT = input.IsDown(Sonic::eKeyState_LeftTrigger);
	bool DownRT = input.IsDown(Sonic::eKeyState_RightTrigger);
	bool PushedUp = inputPtr->LeftStickVertical >= 0.5f;
	bool PushedDown = inputPtr->LeftStickVertical <= -0.5f;
	bool PushedLeft = inputPtr->LeftStickHorizontal <= -0.5f;
	bool PushedRight = inputPtr->LeftStickHorizontal >= 0.5f;
	auto speedContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	auto sonic = speedContext->m_pPlayer;
	auto Flags = speedContext->m_pStateFlag;
	bool IsOutOfControl = Flags->m_Flags[speedContext->eStateFlag_OutOfControl];
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);
	bool ExternalControl = sonic->m_StateMachine.GetCurrentState()->GetStateName() == "ExternalControl";
	bool Goal = sonic->m_StateMachine.GetCurrentState()->GetStateName() == "Goal";
	uint8_t getPackedID = GetCurrentStageID();
	auto arX = 1280;
	auto arY = 720;
	auto rowX1 = 0.6885;
	auto rowX2 = 0.7745;
	auto rowX3 = 0.8611;
	auto rowY1 = 0.3990;
	auto rowY2 = 0.5515;
	auto rowY3 = 0.7046;
	auto scenecheck = scBBIcon && scBBGui && scBBTextArea && scBBLRMove && scBBScroll;
	FUNCTION_PTR(void, __thiscall, changeState, 0x773250, void* This, boost::shared_ptr<void>&spState, const Hedgehog::Base::CSharedString name);
	boost::shared_ptr<void> spState;

	bool PressedActivation =
		(ActivateButton == 0 && PressedSL) ||
		(ActivateButton == 1 && PressedLST) ||
		(ActivateButton == 2 && PressedRST) ||
		(ActivateButton == 3 && DownLT && DownRT) ||
		(ActivateButton == 4 && DownLB && DownRB) ||
		(ActivateButton == 5 && PressedUp) ||
		(ActivateButton == 6 && PressedDown) ||
		(ActivateButton == 7 && PressedLeft) ||
		(ActivateButton == 8 && PressedRight);

	CHudVarTrueSel = CHudVarVisSel + (CHudVarScroll * 3);
	float CHudVarScrollBarStarSpin = (CHudVarScrollBarStarSpinFlt >= 360 ? CHudVarScrollBarStarSpinFlt = 0 : CHudVarScrollBarStarSpinFlt += 1.5);
	float CHudVarShScrollBar = (CHudVarScroll * (100.f / CHudVarShMaxScroll));
	float CHudVarBdScrollBar = (CHudVarScroll * (100.f / CHudVarBdMaxScroll));
	float CHudVarHeScrollBar = (CHudVarScroll * (100.f / CHudVarHeMaxScroll));
	float CHudVarHLScrollBar = (CHudVarScroll * (100.f / CHudVarHLMaxScroll));
	float CHudVarHRScrollBar = (CHudVarScroll * (100.f / CHudVarHRMaxScroll));
	float CHudVarSBScrollBar = (CHudVarScroll * (100.f / CHudVarSBMaxScroll));


	////------Open Fitting Menu------////
	if (PressedActivation && !IsInMenu && IsModernSonic && !ExternalControl && !Goal && !((getPackedID == pam000) && IsOutOfControl) && (IsLookAt == false))
	{
		switch (getPackedID)
		{
		case blb:
		case cnz100:
		case fig000:
		case evt041:
		case evt105:
		case evt121:
			return;
			break;
		default:
			CHudUIOpen(This, Edx, in_rUpdateInfo, CHudVarShScrollBar, CHudVarBdScrollBar, CHudVarHeScrollBar, CHudVarHLScrollBar, CHudVarHRScrollBar, CHudVarSBScrollBar);
			break;
		}
	}


	////------Main Fitting Menu------////
	if (IsInMenu && scBBIcon && scBBGui && IsModernSonic && !ExternalControl && !Goal && !((getPackedID == pam000) && IsOutOfControl))
	{
		switch (getPackedID)
		{
		case blb:
		case cnz100:
		case fig000:
		case evt041:
		case evt105:
		case evt121:
			return;
			break;
		default:

			////------Automatics Handling------////

			////------Handle Cursor Position
			switch (CHudVarVisSel)
			{
			case 0:
				scBBIcon->SetPosition(rowX1 * arX, rowY1 * arY);
				break;
			case 1:
				scBBIcon->SetPosition(rowX2 * arX, rowY1 * arY);
				break;
			case 2:
				scBBIcon->SetPosition(rowX3 * arX, rowY1 * arY);
				break;
			case 3:
				scBBIcon->SetPosition(rowX1 * arX, rowY2 * arY);
				break;
			case 4:
				scBBIcon->SetPosition(rowX2 * arX, rowY2 * arY);
				break;
			case 5:
				scBBIcon->SetPosition(rowX3 * arX, rowY2 * arY);
				break;
			case 6:
				scBBIcon->SetPosition(rowX1 * arX, rowY3 * arY);
				break;
			case 7:
				scBBIcon->SetPosition(rowX2 * arX, rowY3 * arY);
				break;
			case 8:
				scBBIcon->SetPosition(rowX3 * arX, rowY3 * arY);
				break;
			}
			if (IsUnleashedHUD)
			{
				switch (CHudVarVisSel)
				{
				case 0:
					scSWASelect->SetPosition(rowX1 * arX, rowY1 * arY);
					break;
				case 1:
					scSWASelect->SetPosition(rowX2 * arX, rowY1 * arY);
					break;
				case 2:
					scSWASelect->SetPosition(rowX3 * arX, rowY1 * arY);
					break;
				case 3:
					scSWASelect->SetPosition(rowX1 * arX, rowY2 * arY);
					break;
				case 4:
					scSWASelect->SetPosition(rowX2 * arX, rowY2 * arY);
					break;
				case 5:
					scSWASelect->SetPosition(rowX3 * arX, rowY2 * arY);
					break;
				case 6:
					scSWASelect->SetPosition(rowX1 * arX, rowY3 * arY);
					break;
				case 7:
					scSWASelect->SetPosition(rowX2 * arX, rowY3 * arY);
					break;
				case 8:
					scSWASelect->SetPosition(rowX3 * arX, rowY3 * arY);
					break;
				}
			}

			////------Icon Handler
			if (scenecheck)
				CHudUIThumbManager(This);

			////------Scrollbar Handler
			if (scBBScroll->m_MotionFrame >= scBBScroll->m_MotionEndFrame)
				IsInScrollOpen = false;
			if (scenecheck && IsInScrollOpen == false)
			{
				scBBScroll->GetNode("star")->SetRotation(CHudVarScrollBarStarSpin);
				CHudUIScrollManager(This, CHudVarShScrollBar, CHudVarBdScrollBar, CHudVarHeScrollBar, CHudVarHLScrollBar, CHudVarHRScrollBar, CHudVarSBScrollBar);
			}
				
			////------SWA Open Timer Unhide
			if (scSWAFooter)
			{
				if (SWAOpenTimer <= 0)
					scSWAFooter->SetHideFlag(false);
				else
					scSWAFooter->SetHideFlag(true);
			}
			if (scSWAArrow)
			{
				if (SWAOpenTimer <= 0)
					scSWAArrow->SetHideFlag(false);
				else
					scSWAArrow->SetHideFlag(true);
			}
			if (scSWAAlt)
			{
				if (SWAOpenTimer <= 0)
					scSWAAlt->SetHideFlag(false);
				else
					scSWAAlt->SetHideFlag(true);
			}
			if (scSWASelect)
			{
				if (SWAOpenTimer <= 0)
					scSWASelect->SetHideFlag(false);
				else
					scSWASelect->SetHideFlag(true);
			}
			if (scSWAScroll)
			{
				if (SWAOpenTimer <= 0)
					scSWAScroll->SetHideFlag(false);
				else
					scSWAScroll->SetHideFlag(true);
			}
			if (scSWAScrollBG)
			{
				if (SWAOpenTimer <= 0)
					scSWAScrollBG->SetHideFlag(false);
				else
					scSWAScrollBG->SetHideFlag(true);
			}

			////------Handle Alt Prompt
			switch (CHudTabSel)
			{
			case UIPartShoes:
				switch (CHudVarTrueSel)
				{
				case ShDefault:
				case ShSA2Beta:
				case ShSA2Soap:
				case Sh06Gem:
					scBBIcon->GetNode("star")->SetPatternIndex(1);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(false);
					break;
				default:
					scBBIcon->GetNode("star")->SetPatternIndex(0);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(true);
					break;
				}
				break;
			case UIPartBody:
				switch (CHudVarTrueSel)
				{
				case BdDefault:
				case BdWildFire:
				case BdScarf:
				case BdMovieSkin:
					scBBIcon->GetNode("star")->SetPatternIndex(1);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(false);
					break;
				default:
					scBBIcon->GetNode("star")->SetPatternIndex(0);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(true);
					break;
				}
				break;
			case UIPartHandR:
				switch (CHudVarTrueSel)
				{
				case HRSA2Bounce:
					scBBIcon->GetNode("star")->SetPatternIndex(1);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(false);
					break;
				default:
					scBBIcon->GetNode("star")->SetPatternIndex(0);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(true);
					break;
				}
				break;
			case UIPartHead:
				switch (CHudVarTrueSel)
				{
				case HeSimulator:
					scBBIcon->GetNode("star")->SetPatternIndex(1);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(false);
					break;
				default:
					scBBIcon->GetNode("star")->SetPatternIndex(0);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(true);
					break;
				}
				break;
			default:
				scBBIcon->GetNode("star")->SetPatternIndex(0);
				if (IsUnleashedHUD && SWAOpenTimer <= 0)
					scSWAAlt->SetHideFlag(true);
				break;
			}



			////------Menu Input Handling------////

			////------A Button Press
			if (scenecheck && (PressedA && scBBIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUISelect();
			}

			////------B Button Press
			if (scenecheck && PressedB && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUIExit(0);
				return;
			}

			////------Y Button Press
			if (scenecheck && (PressedY && scBBIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUIAlt();
			}

			////------Shoulder Button Press
			if (scenecheck && (PressedRB || PressedLB) && !IsInMenuExit)
			{
				CHudUISwitch(0);
			}

			////------B Button Exit
			if (scenecheck && IsInMenuExit && scBBGui->m_MotionFrame <= 0 && !IsInMenuChange)
			{
				CHudUIExit(1);
			}

			////------Shoulder Button Change
			if (scenecheck && IsInMenuChange && scBBGui->m_MotionFrame <= 0 && !IsInMenuExit)
			{
				CHudUISwitch(1);
			}

			////------Directional Movement Handle
			if (scenecheck && !IsInMenuExit && !IsInMenuChange && scBBIcon == !nullptr)
			{
				if ((PressedUp && scBBIcon->m_MotionFrame >= 3) || (PushedUp && scBBIcon->m_MotionFrame >= 12))
				{
					CHudUIMove(0);
				}

				if ((PressedDown && scBBIcon->m_MotionFrame >= 3) || (PushedDown && scBBIcon->m_MotionFrame >= 12))
				{
					CHudUIMove(1);
				}

				if ((PressedLeft && scBBIcon->m_MotionFrame >= 3) || (PushedLeft && scBBIcon->m_MotionFrame >= 12))
				{
					CHudUIMove(2);
				}

				if ((PressedRight && scBBIcon->m_MotionFrame >= 3) || (PushedRight && scBBIcon->m_MotionFrame >= 12))
				{
					CHudUIMove(3);
				}
			}

			break;
		}
	}

	////------Iterators and Timers------////

	if (!IsInMenu && MemoryOpenTimer >= 1)
		MemoryOpenTimer--;

	if (IsInMenu && !IsInMenuExit)
	{
		if (SWAOpenTimer >= 0)
			SWAOpenTimer--;
	}
	else
	{
		SWAOpenTimer = 15;
	}


	////------Debug UI------////

	//auto DebugUIX = 0.85;
	//if (PressedY && !DebugOpen)
	//{
	//	if (!obDebugTxtUI) //Create UI project if it doesn't exist
	//		CreateDebugTxtUI(This, Edx, in_rUpdateInfo);

	//	DebugSelect = 0;
	//	DebugScrollCount = 0;
	//	DebugOpen = true;

	//	scDebugTxtVisSel = prDebugTxtScreen->CreateScene("deco_text");
	//	scDebugTxtVisSel->SetMotion("Intro_Anim");
	//	scDebugTxtVisSel->SetPosition(0.7 * arX, 0.05 * arY);
	//	scDebugTxtVisSel->SetScale(0.7, 0.7);
	//	scDebugTxtVisSel->SetMotionFrame(0.0f);
	//	scDebugTxtVisSel->m_MotionDisableFlag = false;
	//	scDebugTxtVisSel->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	//	scDebugTxtVisSel->m_MotionSpeed = 1.0f;
	//	scDebugTxtVisSel->Update();
	//	SetDebugDispText(0, scDebugTxtVisSel, 0);
	//	scDebugTxtScroll = prDebugTxtScreen->CreateScene("deco_text");
	//	scDebugTxtScroll->SetMotion("Intro_Anim");
	//	scDebugTxtScroll->SetPosition(0.7 * arX, 0.10 * arY);
	//	scDebugTxtScroll->SetScale(0.7, 0.7);
	//	scDebugTxtScroll->SetMotionFrame(0.0f);
	//	scDebugTxtScroll->m_MotionDisableFlag = false;
	//	scDebugTxtScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	//	scDebugTxtScroll->m_MotionSpeed = 1.0f;
	//	scDebugTxtScroll->Update();
	//	SetDebugDispText(0, scDebugTxtScroll, 1);
	//	scDebugTxtTrueSel = prDebugTxtScreen->CreateScene("deco_text");
	//	scDebugTxtTrueSel->SetMotion("Intro_Anim");
	//	scDebugTxtTrueSel->SetPosition(0.7 * arX, 0.15 * arY);
	//	scDebugTxtTrueSel->SetScale(0.7, 0.7);
	//	scDebugTxtTrueSel->SetMotionFrame(0.0f);
	//	scDebugTxtTrueSel->m_MotionDisableFlag = false;
	//	scDebugTxtTrueSel->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	//	scDebugTxtTrueSel->m_MotionSpeed = 1.0f;
	//	scDebugTxtTrueSel->Update();
	//	SetDebugDispText(0, scDebugTxtTrueSel, 2);
	//}
	//
	//if (DebugOpen)
	//{
	//	//DebugOption01 = DebugScrollCount - 0;
	//	//DebugOption02 = DebugScrollCount + 1;
	//	//DebugOption03 = DebugScrollCount + 2;
	//	//DebugOption04 = DebugScrollCount + 3;
	//	//DebugOption05 = DebugScrollCount + 4;
	//	//DebugOptionSelect = DebugSelect - DebugScrollCount;
	//	SetDebugDispText(CHudVarVisSel, scDebugTxtVisSel, 0);
	//	SetDebugDispText(CHudVarScroll, scDebugTxtScroll, 1);
	//	SetDebugDispText(CHudVarTrueSel, scDebugTxtTrueSel, 2);
	//	//SetDebugOptions(DebugOption01, scDebugOpt01);
	//	//SetDebugOptions(DebugOption02, scDebugOpt02);
	//	//SetDebugOptions(DebugOption03, scDebugOpt03);
	//	//SetDebugOptions(DebugOption04, scDebugOpt04);
	//	//SetDebugOptions(DebugOption05, scDebugOpt05);
	//	//SetDebug01Text("1");
	//	//switch (DebugOptionSelect)
	//	//{
	//	//case -2:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.025 * arY);
	//	//	break;
	//	//case -1:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.05 * arY);
	//	//	break;
	//	//case 0:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.1 * arY);
	//	//	break;
	//	//case 1:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.15 * arY);
	//	//	break;
	//	//case 2:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.2 * arY);
	//	//	break;
	//	//case 3:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.25 * arY);
	//	//	break;
	//	//case 4:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.3 * arY);
	//	//	break;
	//	//case 5:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.35 * arY);
	//	//	break;
	//	//case 6:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.4 * arY);
	//	//	break;
	//	//default:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.05 * arY);
	//	//	break;
	//	//}
	//	//scDebugSelector->SetPosition(0.75 * arX, 0.1 * arY);
	

	//	//if (PressedY && DebugOpen)
	//	//{
	//	//	//DebugSelect = DebugListStart;
	//	//	//DebugScrollCount = DebugListStart;
	//	//	
	//	//}
	//}

}
HOOK(void, __fastcall, CHudSonicStageUpdate, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto speedContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	auto sonic = speedContext->m_pPlayer;
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);
	bool ExternalControl = sonic->m_StateMachine.GetCurrentState()->GetStateName() == "ExternalControl";
	bool Goal = sonic->m_StateMachine.GetCurrentState()->GetStateName() == "Goal";
	if (IsModernSonic && !ExternalControl && !Goal)
		CHudFittingMenu(This, Edx, in_rUpdateInfo);
	//printf("Gameplay HUD\n");
	originalCHudSonicStageUpdate(This, Edx, in_rUpdateInfo);
}
HOOK(void, __fastcall, CHudPlayableMenuUpdate, 0x0108D510, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto speedContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	auto sonic = speedContext->m_pPlayer;
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);
	bool ExternalControl = sonic->m_StateMachine.GetCurrentState()->GetStateName() == "ExternalControl";
	bool Goal = sonic->m_StateMachine.GetCurrentState()->GetStateName() == "Goal";
	if (IsModernSonic && !ExternalControl && !Goal)
		CHudFittingMenu(This, Edx, in_rUpdateInfo);
	//printf("Hub world HUD\n");
	originalCHudPlayableMenuUpdate(This, Edx, in_rUpdateInfo);
}
HOOK(void, __fastcall, CHUDPauseUpdate, 0x0042A520, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	IsInMenuExit = false;
	IsInMenu = false;
	CHudTabSel = UIPartShoes;
	CHudVarVisSel = 0;
	MemoryOpenTimer = 0;
	FILE* pFile = fopen(saveFilePath.c_str(), "wb");
	WriteINI(pFile);
	fclose(pFile);
	CHudUISceneDestroy();
	originalCHUDPauseUpdate(This);
}
void __fastcall CHudSonicStageRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreen();
	IsInMenuExit = false;
	IsInMenu = false;
	CHudTabSel = UIPartShoes;
	CHudVarVisSel = 0;
	MemoryOpenTimer = 0;
	FILE* pFile = fopen(saveFilePath.c_str(), "wb");
	WriteINI(pFile);
	fclose(pFile);
	CHudUISceneDestroy();
}
HOOK(void, __fastcall, CHudResultStart, 0x010B6840, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	IsInMenuExit = false;
	IsInMenu = false;
	CHudTabSel = UIPartShoes;
	CHudVarVisSel = 0;
	MemoryOpenTimer = 0;
	FILE* pFile = fopen(saveFilePath.c_str(), "wb");
	WriteINI(pFile);
	fclose(pFile);
	CHudUISceneDestroy();
	originalCHudResultStart(This);
}

//Handle Pausing
HOOK(void, __fastcall, CPlayerAddCallback, 0xE799F0, Sonic::Player::CPlayer* This, void* Edx, const Hedgehog::Base::THolder<Sonic::CWorld>& worldHolder, Sonic::CGameDocument* pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& spDatabase)
{
	pGameDocument->AddUpdateUnit("9", This);
	originalCPlayerAddCallback(This, Edx, worldHolder, pGameDocument, spDatabase);
}
void* updateDirectorCustom(void* context, float elapsedTime)
{
	addUpdateCommand(context, elapsedTime, "1");
	//addUpdateCommand(context, elapsedTime, "2");
	addUpdateCommand(context, elapsedTime, "3");
	addUpdateCommand(context, elapsedTime, "b");
	//addUpdateCommand(context, elapsedTime, "e");
	//addUpdateCommand(context, elapsedTime, "9");

	callUnknownFunction(context, elapsedTime);

	addRenderCommand(context, elapsedTime, "1");
	//addRenderCommand(context, elapsedTime, "2");
	addRenderCommand(context, elapsedTime, "3");
	addRenderCommand(context, elapsedTime, "b");
	//addRenderCommand(context, elapsedTime, "e");
	//addRenderCommand(context, elapsedTime, "9");

	finishRenderer(context);
	return finishUpdater(context);
}
HOOK(void*, __fastcall, UpdateDirectorSimple, 0x1105A60, void* This, void* Edx, void* context, float elapsedTime)
{
	if (IsInMenu)
		return updateDirectorCustom(context, elapsedTime);

	return originalUpdateDirectorSimple(This, Edx, context, elapsedTime);
}
HOOK(void*, __fastcall, UpdateDirectorNormal, 0x11B60B0, void* This, void* Edx, void* context, float elapsedTime)
{
	if (IsInMenu)
		return updateDirectorCustom(context, elapsedTime);

	return originalUpdateDirectorNormal(This, Edx, context, elapsedTime);
}

//Look At
HOOK(void, __fastcall, MsgLookAtStart, 0x00E3F420, void* This, void* notSonicContext)
{
	IsLookAt = true;

	printf("LOOK AT START");
	printf("\n");

	originalMsgLookAtStart(This, notSonicContext);
}
HOOK(void, __fastcall, MsgLookAtEnd, 0x00E3F3B0, void* This, void* notSonicContext)
{
	IsLookAt = false;

	printf("LOOK AT END");
	printf("\n");

	originalMsgLookAtEnd(This, notSonicContext);
}

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationUIParams, 0x00D65180, Sonic::CParameterFile* This)
{
	auto parameterGroup = This->CreateParameterGroup("Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce = parameterGroup->CreateParameterCategory("Customizable Sonic", "Parameters for customizable Sonic");

	cat_Bounce->CreateParamBool(&ConfigDecoEnable, "Show the fitting room UI decorations.");
	cat_Bounce->CreateParamTypeList((uint32_t*)&ActivateButton, "Activation Button", "Choose which button to press to open the Fitting Room",
		{
			{ "Select", 0},
			{ "Left Stick", 1},
			{ "Right Stick", 2},
			{ "Triggers", 3},
			{ "Shoulders", 4},
			{ "Dup", 5},
			{ "Ddown", 6},
			{ "Dleft", 7},
			{ "Dright", 8},
		});

	parameterGroup->Flush();

	originalInitializeApplicationUIParams(This);
}

//Install UI
void InstallCustomUI::applyPatches(ModInfo_t* modInfo)
{
	saveFilePath = modInfo->CurrentMod->Path;
	saveFilePath.erase(saveFilePath.find_last_of("\\/") + 1);
	saveFilePath += "CustomizeSave.ini";
	ReadINI(saveFilePath);

	INSTALL_HOOK(CHudSonicStageUpdate);
	INSTALL_HOOK(CHudPlayableMenuUpdate);
	INSTALL_HOOK(CHUDPauseUpdate);
	INSTALL_HOOK(CHudResultStart);
	WRITE_MEMORY(0x16A467C, void*, CHudSonicStageRemoveCallback);
	WRITE_MEMORY(0x16A5598, void*, CHudSonicStageRemoveCallback);
	INSTALL_HOOK(UpdateDirectorSimple);
	INSTALL_HOOK(UpdateDirectorNormal);
	INSTALL_HOOK(CPlayerAddCallback);
	INSTALL_HOOK(MsgLookAtStart);
	INSTALL_HOOK(MsgLookAtEnd);
	INSTALL_HOOK(InitializeApplicationUIParams);
	//INSTALL_HOOK(CastStuffTest);
	//if (ActivateButton >= 5 && ActivateButton <= 8)
		WRITE_JUMP(0xD97B56, (void*)0xD97B9E); // Ignore D-pad input for Sonic's control
}