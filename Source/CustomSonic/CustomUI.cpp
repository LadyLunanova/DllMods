#include <Common.h>
#include "UpdateDirector.h"
#include "CustomSonicItems.h"
#include "InstallCustomUI.h"
<<<<<<< HEAD
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

=======
#include "CustomUI.h"

>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
//Declare Funcs
void ReadINI();
void WriteINI(FILE* iniFile);

//Handle UI
<<<<<<< HEAD
void CreateFittingUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	//Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
	Sonic::CCsdDatabaseWrapper wrapper(database.get());

	if (!IsUnleashedHUD)
	{
		auto spCsdProject = wrapper.GetCsdProject("ui_fitting");
		prFittingScreenBB = spCsdProject->m_rcProject;
		if (spCsdProject->IsMadeAll())
		{
			obBBCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenBB, 0.5f, "HUD_Pause", true);
			Sonic::CGameDocument::GetInstance()->AddGameObject(obBBCustomUI, "main", This);
		}
	}
	else
	{
		auto spCsdProject = wrapper.GetCsdProject("ui_fitting_bb");
		prFittingScreenBB = spCsdProject->m_rcProject;
		if (spCsdProject->IsMadeAll())
		{
			obBBCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenBB, 0.5f, "HUD_Pause", true);
			Sonic::CGameDocument::GetInstance()->AddGameObject(obBBCustomUI, "main", This);
		}

		auto spCsdProjectSWA = wrapper.GetCsdProject("ui_fitting_swa");
		prFittingScreenSWA = spCsdProjectSWA->m_rcProject;
		if (spCsdProjectSWA->IsMadeAll())
		{
			obSWACustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenSWA, 0.4f, "HUD_Pause", true);
			Sonic::CGameDocument::GetInstance()->AddGameObject(obSWACustomUI, "main", This);
		}
	}
}

void CHudUIOpen(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo, float Sh, float Bd, float He, float HL, float HR)
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
=======
void CHudUIOpen(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto arX = 1280;
	auto arY = 720;
	auto rowX1 = 0.6885;
	auto rowX2 = 0.7745;
	auto rowX3 = 0.8611;
	auto rowY1 = 0.3990;
	auto rowY2 = 0.5515;
	auto rowY3 = 0.7046;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9

	ReadINI();
	prevblur = *ENABLE_BLUR;
	*ENABLE_BLUR = false;
	if (MemoryOpenTimer <= 0)
	{
<<<<<<< HEAD
		CHudTabSel = UIPartShoes;
		CHudVarVisSel = 0;
		CHudVarScroll = 0;
		CHudVarTrueSel = 0;
	}
	if (!obBBCustomUI) //Create UI project if it doesn't exist
		CreateFittingUI(This, Edx, in_rUpdateInfo);
=======
		MenuOption = UIPartShoes;
		ItemOption = 0;
	}
	if (!obBBCustomUI) //Create UI project if it doesn't exist
		CreateFittingUI(This, Edx, in_rUpdateInfo);
	switch (SelectShoes) //Set proper variants on open
	{
	case ShClassic:
		ShDefaultVariant = ShoeClassic;
		ShSA2Variant = SA2Soap;
		Sh06Variant = GemDefault;
		ShSUVariant = SUAirboost;
		break;
	case ShArchie:
		ShDefaultVariant = ShoeArchie;
		ShSA2Variant = SA2Soap;
		Sh06Variant = GemDefault;
		ShSUVariant = SUAirboost;
		break;
	case ShSoapsLightSpd:
		ShSA2Variant = SA2LightSpd;
		ShDefaultVariant = ShoeDefault;
		Sh06Variant = GemDefault;
		ShSUVariant = SUAirboost;
		break;
	case ShSoapsRacing:
		ShSA2Variant = SA2Racing;
		ShDefaultVariant = ShoeDefault;
		Sh06Variant = GemDefault;
		ShSUVariant = SUAirboost;
		break;
	case Sh06GemRed:
		Sh06Variant = GemRed;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		ShSUVariant = SUAirboost;
		break;
	case Sh06GemBlue:
		Sh06Variant = GemBlue;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		ShSUVariant = SUAirboost;
		break;
	case Sh06GemGreen:
		Sh06Variant = GemGreen;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		ShSUVariant = SUAirboost;
		break;
	case Sh06GemPurple:
		Sh06Variant = GemPurple;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		ShSUVariant = SUAirboost;
		break;
	case Sh06GemSky:
		Sh06Variant = GemSky;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		ShSUVariant = SUAirboost;
		break;
	case Sh06GemWhite:
		Sh06Variant = GemWhite;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		ShSUVariant = SUAirboost;
		break;
	case Sh06GemYellow:
		Sh06Variant = GemYellow;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		ShSUVariant = SUAirboost;
		break;
	case ShSULightSpd:
		ShSUVariant = SULightSpd;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		Sh06Variant = GemDefault;
		break;
	case ShSUStomp:
		ShSUVariant = SUStomp;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		Sh06Variant = GemDefault;
		break;
	case ShSUWallJmp:
		ShSUVariant = SUWallJmp;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		Sh06Variant = GemDefault;
		break;
	case ShSUWerehog:
		ShSUVariant = SUWerehog;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		Sh06Variant = GemDefault;
		break;
	case ShSUZoney:
		ShSUVariant = SUZoney;
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		Sh06Variant = GemDefault;
		break;
	default:
		ShDefaultVariant = ShoeDefault;
		ShSA2Variant = SA2Soap;
		Sh06Variant = GemDefault;
		ShSUVariant = SUAirboost;
		break;
	}
	if (SelectHandR == HRMagicHands)
		HRMagicVariant = true;
	else
		HRMagicVariant = false;
	if (SelectHandR == HRArkCosmos)
		HRRidersVariant = true;
	else
		HRRidersVariant = false;
	switch (SelectBody)
	{
	case BdScarfArms:
		BdBlueArmVariant = false;
		BdScarfVariant = true;
		BdMovieVariant = false;
		break;
	case BdBlueArms:
		BdBlueArmVariant = true;
		BdScarfVariant = false;
		BdMovieVariant = false;
		break;
	case BdMovieSkinArms:
		BdBlueArmVariant = false;
		BdScarfVariant = false;
		BdMovieVariant = true;
		break;
	default:
		BdBlueArmVariant = false;
		BdScarfVariant = false;
		BdMovieVariant = false;
		break;
	}
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9

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
<<<<<<< HEAD
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
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
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

=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
	}
	//Misc
	IsInMenu = true;
	CHudUISFXOpen();
	return;
}

void CHudUISelect()
{
	//SWAOpenTimer = 10;
<<<<<<< HEAD
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
=======
	switch (MenuOption)
	{
	case UIPartShoes:
		switch (ItemOption)
		{
		case 0:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			switch (ShDefaultVariant)
			{
			case ShoeDefault:
				SelectShoes = ShDefault;
				break;
			case ShoeClassic:
				SelectShoes = ShClassic;
				break;
			case ShoeArchie:
				SelectShoes = ShArchie;
				break;
			}
			break;
		case 1:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectShoes = ShSA1LightSpd;
			break;
		case 2:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			switch (ShSA2Variant)
			{
			case SA2Soap:
				SelectShoes = ShSoaps;
				break;
			case SA2LightSpd:
				SelectShoes = ShSoapsLightSpd;
				break;
			case SA2Racing:
				SelectShoes = ShSoapsRacing;
				break;
			}
			break;
		case 3:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectShoes = ShRiders;
			break;
		case 4:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			switch (Sh06Variant)
			{
			case GemDefault:
				SelectShoes = Sh06GemDefault;
				break;
			case GemRed:
				SelectShoes = Sh06GemRed;
				break;
			case GemBlue:
				SelectShoes = Sh06GemBlue;
				break;
			case GemGreen:
				SelectShoes = Sh06GemGreen;
				break;
			case GemPurple:
				SelectShoes = Sh06GemPurple;
				break;
			case GemSky:
				SelectShoes = Sh06GemSky;
				break;
			case GemWhite:
				SelectShoes = Sh06GemWhite;
				break;
			case GemYellow:
				SelectShoes = Sh06GemYellow;
				break;
			}
			break;
		case 5:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			switch (ShSUVariant)
			{
			case SUAirboost:
				SelectShoes = ShSUAirboost;
				break;
			case SULightSpd:
				SelectShoes = ShSULightSpd;
				break;
			case SUStomp:
				SelectShoes = ShSUStomp;
				break;
			case SUWallJmp:
				SelectShoes = ShSUWallJmp;
				break;
			case SUWerehog:
				SelectShoes = ShSUWerehog;
				break;
			case SUZoney:
				SelectShoes = ShSUZoney;
				break;
			}
			break;
		case 6:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectShoes = ShLinkBoots;
			break;
		case 7:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectShoes = ShBoom;
			break;
		case 8:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectShoes = ShPumas;
			break;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
		}
		return;
		break;
	case UIPartBody:
<<<<<<< HEAD
		if (CHudVarTrueSel >= BdOverflow00)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectBody = (enum SelectBodyType)CHudVarTrueSel;
=======
		switch (ItemOption)
		{
		case 0:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			if (!BdBlueArmVariant)
				SelectBody = BdDefault;
			else
				SelectBody = BdBlueArms;
			break;
		case 1:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectBody = BdRaceSuit;
			break;
		case 2:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectBody = BdWildFire;
			break;
		case 3:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectBody = BdLinkCos;
			break;
		case 4:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			if (!BdScarfVariant)
				SelectBody = BdScarf;
			else
				SelectBody = BdScarfArms;
			break;
		case 5:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectBody = Bd2020SwimVest;
			break;
		case 6:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectBody = Bd2020SurfSuit;
			break;
		case 7:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectBody = Bd2020HorseShirt;
			break;
		case 8:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			if (!BdMovieVariant)
				SelectBody = BdMovieSkin;
			else
				SelectBody = BdMovieSkinArms;
			break;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
		}
		return;
		break;
	case UIPartHead:
<<<<<<< HEAD
		if (CHudVarTrueSel >= HeOverflow00)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHead = (enum SelectHeadType)CHudVarTrueSel;
=======
		switch (ItemOption)
		{
		case 0:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHead = HeDefault;
			break;
		case 1:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHead = HeSA1Sunglass;
			break;
		case 2:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHead = HeRiders;
			break;
		case 3:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHead = HeHeadphones;
			break;
		case 4:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHead = HeLinkHat;
			break;
		case 5:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHead = HeBoomSpikes;
			break;
		case 6:
			CHudUISFXSelect(false);
			break;
		case 7:
			CHudUISFXSelect(false);
			break;
		case 8:
			CHudUISFXSelect(false);
			break;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
		}
		return;
		break;
	case UIPartHandL:
<<<<<<< HEAD
		if (CHudVarTrueSel >= HLOverflow00)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = (enum SelectHandLType)CHudVarTrueSel;
=======
		switch (ItemOption)
		{
		case 0:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = HLDefault;
			break;
		case 1:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = HLCrystalRing;
			break;
		case 2:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = HLFlameRing;
			break;
		case 3:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = HLRacingGlove;
			break;
		case 4:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = HLRiders;
			break;
		case 5:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = HL06Bounce;
			break;
		case 6:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = HLChipBracelet;
			break;
		case 7:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = HLLinkGlove;
			break;
		case 8:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandL = HLBoomGlove;
			break;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
		}
		return;
		break;
	case UIPartHandR:
<<<<<<< HEAD
		if (CHudVarTrueSel >= HROverflow00)
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandR = (enum SelectHandRType)CHudVarTrueSel;
=======
		switch (ItemOption)
		{
		case 0:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandR = HRDefault;
			break;
		case 1:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			if (!HRMagicVariant)
				SelectHandR = HRBounceBracelet;
			else
				SelectHandR = HRMagicHands;
			break;
		case 2:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandR = HRRacingGlove;
			break;
		case 3:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			if (!HRRidersVariant)
				SelectHandR = HRRiders;
			else
				SelectHandR = HRArkCosmos;
			break;
		case 4:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandR = HR06HomingSmash;
			break;
		case 5:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandR = HRSecretRing;
			break;
		case 6:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandR = HRGauntlet;
			break;
		case 7:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandR = HRLinkGlove;
			break;
		case 8:
			CHudUISFXSelect(true);
			CHudUICursorAnim();
			SelectHandR = HRBoomGlove;
			break;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
		}
		return;
		break;
	}
}

void CHudUIMove(int Type)
{
	CHudUISFXMove();
	CHudUICursorAnim();
<<<<<<< HEAD
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
=======
	switch (Type)
	{
	case 0:
		switch (ItemOption)
		{
		case 0:
			ItemOption = 6;
			break;
		case 1:
			ItemOption = 7;
			break;
		case 2:
			ItemOption = 8;
			break;
		case 3:
			ItemOption = 0;
			break;
		case 4:
			ItemOption = 1;
			break;
		case 5:
			ItemOption = 2;
			break;
		case 6:
			ItemOption = 3;
			break;
		case 7:
			ItemOption = 4;
			break;
		case 8:
			ItemOption = 5;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			break;
		}
		return;
		break;
<<<<<<< HEAD
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
=======
	case 1:
		switch (ItemOption)
		{
		case 0:
			ItemOption = 3;
			break;
		case 1:
			ItemOption = 4;
			break;
		case 2:
			ItemOption = 5;
			break;
		case 3:
			ItemOption = 6;
			break;
		case 4:
			ItemOption = 7;
			break;
		case 5:
			ItemOption = 8;
			break;
		case 6:
			ItemOption = 0;
			break;
		case 7:
			ItemOption = 1;
			break;
		case 8:
			ItemOption = 2;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			break;
		}
		return;
		break;
<<<<<<< HEAD
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
=======
	case 2:
		switch (ItemOption)
		{
		case 0:
			ItemOption = 2;
			break;
		case 1:
			ItemOption = 0;
			break;
		case 2:
			ItemOption = 1;
			break;
		case 3:
			ItemOption = 5;
			break;
		case 4:
			ItemOption = 3;
			break;
		case 5:
			ItemOption = 4;
			break;
		case 6:
			ItemOption = 8;
			break;
		case 7:
			ItemOption = 6;
			break;
		case 8:
			ItemOption = 7;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			break;
		}
		return;
		break;
<<<<<<< HEAD
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
=======
	case 3:
		switch (ItemOption)
		{
		case 0:
			ItemOption = 1;
			break;
		case 1:
			ItemOption = 2;
			break;
		case 2:
			ItemOption = 0;
			break;
		case 3:
			ItemOption = 4;
			break;
		case 4:
			ItemOption = 5;
			break;
		case 5:
			ItemOption = 3;
			break;
		case 6:
			ItemOption = 7;
			break;
		case 7:
			ItemOption = 8;
			break;
		case 8:
			ItemOption = 6;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			break;
		}
		return;
		break;
	}
<<<<<<< HEAD

=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
}

void CHudUIAlt()
{
<<<<<<< HEAD
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
=======
	if (MenuOption == UIPartShoes)
	{
		switch (ItemOption)
		{
		case 0:
			switch (ShDefaultVariant)
			{
			case ShoeDefault:
				if (SelectShoes == ShDefault)
					SelectShoes = ShClassic;
				ShDefaultVariant = ShoeClassic;
				break;
			case ShoeClassic:
				if (SelectShoes == ShClassic)
					SelectShoes = ShArchie;
				ShDefaultVariant = ShoeArchie;
				break;
			case ShoeArchie:
				if (SelectShoes == ShArchie)
					SelectShoes = ShDefault;
				ShDefaultVariant = ShoeDefault;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
				break;
			}
			CHudUISFXAlt();
			CHudUICursorAnim();
<<<<<<< HEAD
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
=======
			break;
		case 2:
			switch (ShSA2Variant)
			{
			case SA2Soap:
				if (SelectShoes == ShSoaps)
					SelectShoes = ShSoapsLightSpd;
				ShSA2Variant = SA2LightSpd;
				break;
			case SA2LightSpd:
				if (SelectShoes == ShSoapsLightSpd)
					SelectShoes = ShSoapsRacing;
				ShSA2Variant = SA2Racing;
				break;
			case SA2Racing:
				if (SelectShoes == ShSoapsRacing)
					SelectShoes = ShSoaps;
				ShSA2Variant = SA2Soap;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
				break;
			}
			CHudUISFXAlt();
			CHudUICursorAnim();
<<<<<<< HEAD
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
			return;
			break;
		case BdWildFire:
			if (BdWildFireVariant == false) BdWildFireVariant = true; else BdWildFireVariant = false;
			CHudUISFXAlt();
			CHudUICursorAnim();
			return;
			break;
		case BdScarf:
			if (BdScarfVariant == false) BdScarfVariant = true; else BdScarfVariant = false;
			CHudUISFXAlt();
			CHudUICursorAnim();
			return;
			break;
		case BdMovieSkin:
			if (BdMovieVariant == false) BdMovieVariant = true; else BdMovieVariant = false;
			CHudUISFXAlt();
			CHudUICursorAnim();
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
			return;
			break;
		}
		return;
		break;
=======
			break;
		case 4:
			CHudUISFXAlt();
			CHudUICursorAnim();
			switch (Sh06Variant)
			{
			case GemDefault:
				if (SelectShoes == Sh06GemDefault)
					SelectShoes = Sh06GemRed;
				Sh06Variant = GemRed;
				break;
			case GemRed:
				if (SelectShoes == Sh06GemRed)
					SelectShoes = Sh06GemBlue;
				Sh06Variant = GemBlue;
				break;
			case GemBlue:
				if (SelectShoes == Sh06GemBlue)
					SelectShoes = Sh06GemGreen;
				Sh06Variant = GemGreen;
				break;
			case GemGreen:
				if (SelectShoes == Sh06GemGreen)
					SelectShoes = Sh06GemPurple;
				Sh06Variant = GemPurple;
				break;
			case GemPurple:
				if (SelectShoes == Sh06GemPurple)
					SelectShoes = Sh06GemSky;
				Sh06Variant = GemSky;
				break;
			case GemSky:
				if (SelectShoes == Sh06GemSky)
					SelectShoes = Sh06GemWhite;
				Sh06Variant = GemWhite;
				break;
			case GemWhite:
				if (SelectShoes == Sh06GemWhite)
					SelectShoes = Sh06GemYellow;
				Sh06Variant = GemYellow;
				break;
			case GemYellow:
				if (SelectShoes == Sh06GemYellow)
					SelectShoes = Sh06GemDefault;
				Sh06Variant = GemDefault;
				break;
			}
			break;
		case 5:
			CHudUISFXAlt();
			CHudUICursorAnim();
			switch (ShSUVariant)
			{
			case SUAirboost:
				ShSUVariant = SULightSpd;
				if (SelectShoes == ShSUAirboost)
					SelectShoes = ShSULightSpd;
				break;
			case SULightSpd:
				ShSUVariant = SUStomp;
				if (SelectShoes == ShSULightSpd)
					SelectShoes = ShSUStomp;
				break;
			case SUStomp:
				ShSUVariant = SUWallJmp;
				if (SelectShoes == ShSUStomp)
					SelectShoes = ShSUWallJmp;
				break;
			case SUWallJmp:
				ShSUVariant = SUWerehog;
				if (SelectShoes == ShSUWallJmp)
					SelectShoes = ShSUWerehog;
				break;
			case SUWerehog:
				ShSUVariant = SUZoney;
				if (SelectShoes == ShSUWerehog)
					SelectShoes = ShSUZoney;
				break;
			case SUZoney:
				ShSUVariant = SUAirboost;
				if (SelectShoes == ShSUZoney)
					SelectShoes = ShSUAirboost;
				break;
			}
			break;
		}
		return;
	}
	if (MenuOption == UIPartHandR)
	{
		switch (ItemOption)
		{
		case 1:
			CHudUISFXAlt();
			CHudUICursorAnim();
			if (HRMagicVariant)
			{
				HRMagicVariant = false;
				if (SelectHandR == HRMagicHands)
					SelectHandR = HRBounceBracelet;
			}
			else
			{
				HRMagicVariant = true;
				if (SelectHandR == HRBounceBracelet)
					SelectHandR = HRMagicHands;
			}
			break;
		case 3:
			CHudUISFXAlt();
			CHudUICursorAnim();
			if (HRRidersVariant)
			{
				HRRidersVariant = false;
				if (SelectHandR == HRArkCosmos)
					SelectHandR = HRRiders;
			}
			else
			{
				HRRidersVariant = true;
				if (SelectHandR == HRRiders)
					SelectHandR = HRArkCosmos;
			}
			break;
		}
		return;
	}
	if (MenuOption == UIPartBody)
	{
		switch (ItemOption)
		{
		case 0:
			CHudUISFXAlt();
			CHudUICursorAnim();
			if (BdBlueArmVariant)
			{
				BdBlueArmVariant = false;
				if (SelectBody == BdBlueArms)
					SelectBody = BdDefault;
			}
			else
			{
				BdBlueArmVariant = true;
				if (SelectBody == BdDefault)
					SelectBody = BdBlueArms;
			}
			break;
		case 4:
			CHudUISFXAlt();
			CHudUICursorAnim();
			if (BdScarfVariant)
			{
				BdScarfVariant = false;
				if (SelectBody == BdScarfArms)
					SelectBody = BdScarf;
			}
			else
			{
				BdScarfVariant = true;
				if (SelectBody == BdScarf)
					SelectBody = BdScarfArms;
			}
			break;
		case 8:
			CHudUISFXAlt();
			CHudUICursorAnim();
			if (BdMovieVariant)
			{
				BdMovieVariant = false;
				if (SelectBody == BdMovieSkinArms)
					SelectBody = BdMovieSkin;
			}
			else
			{
				BdMovieVariant = true;
				if (SelectBody == BdMovieSkin)
					SelectBody = BdMovieSkinArms;
			}
			break;
		}
		return;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
		IsInScrollOpen = true;
		scBBScroll->SetMotion("Intro_Anim");
		scBBScroll->SetMotionFrame(22.0f);
		scBBScroll->m_MotionDisableFlag = false;
		scBBScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBScroll->m_MotionSpeed = -2.0f;
		scBBScroll->Update();
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
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
				CHudTabSel = UISonicPart;
				break;
			case UISonicPart:
				CHudTabSel = UIPartShoes;
=======
		//MenuOption = UIPartShoes;
		if (IsInMenuChangeR)
		{
			switch (MenuOption)
			{
			case UIPartShoes:
				MenuOption = UIPartBody;
				break;
			case UIPartBody:
				MenuOption = UIPartHead;
				break;
			case UIPartHead:
				MenuOption = UIPartHandL;
				break;
			case UIPartHandL:
				MenuOption = UIPartHandR;
				break;
			case UIPartHandR:
				MenuOption = UIPartShoes;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
				break;
			}
		}
		if (IsInMenuChangeL)
		{
<<<<<<< HEAD
			switch (CHudTabSel)
			{
			case UIPartShoes:
				CHudTabSel = UISonicPart;
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
			case UISonicPart:
				CHudTabSel = UIPartHandR;
				break;
			}
		}
=======
			switch (MenuOption)
			{
			case UIPartShoes:
				MenuOption = UIPartHandR;
				break;
			case UIPartBody:
				MenuOption = UIPartShoes;
				break;
			case UIPartHead:
				MenuOption = UIPartBody;
				break;
			case UIPartHandL:
				MenuOption = UIPartHead;
				break;
			case UIPartHandR:
				MenuOption = UIPartHandL;
				break;
			}
		}
		//ItemOption = 0;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
		scBBScroll->SetMotion("Intro_Anim");
		scBBScroll->SetMotionFrame(15.0f);
		scBBScroll->m_MotionDisableFlag = false;
		scBBScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBScroll->m_MotionSpeed = 1.0f;
		scBBScroll->Update();
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
		IsInScrollOpen = true;
		scBBScroll->SetMotion("Intro_Anim");
		scBBScroll->SetMotionFrame(22.0f);
		scBBScroll->m_MotionDisableFlag = false;
		scBBScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scBBScroll->m_MotionSpeed = -1.0f;
		scBBScroll->Update();
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
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
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
		return;
	}
	if (Type == 1)
	{
		IsInMenuExit = false;
		IsInMenu = false;
		MemoryOpenTimer = 1800;
		*ENABLE_BLUR = prevblur;
		FILE* pFile = fopen("CustomSelect.ini", "wb");
		WriteINI(pFile);
		fclose(pFile);
		CHudUISceneDestroy();
		return;
	}
}

<<<<<<< HEAD
void CHudUITexsetData(Sonic::CGameObject* This, int texSetID, const char* texString)
{
	auto texList = static_cast<Sonic::CCsdTexListMirage*>(prFittingScreenBB->m_rcTexList.Get());
	auto ui_cat_item_alt_sel = boost::make_shared<hh::mr::CTextureData>();
	ui_cat_item_alt_sel->m_spPictureData = hh::mr::CMirageDatabaseWrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get()).GetPictureData(texString);

	texList->m_spTexsetData->m_TextureList[texSetID] = ui_cat_item_alt_sel;
}

const char* CHudUIThumbShoeString(int id, char* result)
{
	auto mapChar = MAP_THUMB_SHOE[ShDefault + (CHudVarScroll * 3) + id];
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
	auto mapChar = MAP_THUMB_BODY[BdDefault + (CHudVarScroll * 3) + id];
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
	auto mapChar = MAP_THUMB_HEAD[HeDefault + (CHudVarScroll * 3) + id];
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
	auto mapChar = MAP_THUMB_HANDL[HLDefault + (CHudVarScroll * 3) + id];
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
	auto mapChar = MAP_THUMB_HANDR[HRDefault + (CHudVarScroll * 3) + id];
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
const char* CHudUIThumbSnSonMatString(int id, char* result)
{
	auto mapChar = MAP_THUMB_SONICBODY[SBSnMaterial + (CHudVarScroll * 3) + id];
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
	printf("%s%s%s", texExtUI, mapChar, texExtVar);

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
	case UISonicPart:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbSnSonMatString(i, SBBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UISonicPart);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UISonicPart);
		return;
		break;
	}
	
	//auto ui_sh_ShDefault_00_On = boost::make_shared<hh::mr::CTextureData>();
	//ui_sh_ShDefault_00_On->m_spPictureData = hh::mr::CMirageDatabaseWrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get()).GetPictureData(MAP_THUMB_SHOE[ShDefault]);
	//auto texList = static_cast<Sonic::CCsdTexListMirage*>(prFittingScreenBB->m_rcTexList.Get());
	// ((ShDefault + (CHudVarScroll * 3) + 0) == SelectShoes)
	//CHudUITexsetData(This,  6, MAP_THUMB_SHOE[ShDefault + (CHudVarScroll * 3) + 0]);

}

void CHudUIScrollManager(Sonic::CGameObject* This, float Sh, float Bd, float He, float HL, float HR)
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
	case UISonicPart:
		scBBScroll->SetMotionFrame(0);
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
		}
=======
void CHudUIThumbManager()
{
	switch (MenuOption)
	{
	case UIPartShoes: ////Shoe Icons////
		//Default Shoes
		switch (ShDefaultVariant)
		{
		case ShoeDefault:
			if (SelectShoes == ShDefault)
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(2);
			else
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(1);
			break;
		case ShoeClassic:
			if (SelectShoes == ShClassic)
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(12);
			else
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(11);
			break;
		case ShoeArchie:
			if (SelectShoes == ShArchie)
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(14);
			else
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(13);
			break;
		}
		//SA1 Lightspeed Shoes
		if (SelectShoes == ShSA1LightSpd)
			scBBGui->GetNode("img_ch_1")->SetPatternIndex(2);
		else
			scBBGui->GetNode("img_ch_1")->SetPatternIndex(1);
		//Soap Shoes
		switch (ShSA2Variant)
		{
		case SA2Soap:
			if (SelectShoes == ShSoaps)
				scBBGui->GetNode("img_ch_2")->SetPatternIndex(2);
			else
				scBBGui->GetNode("img_ch_2")->SetPatternIndex(1);
			break;
		case SA2LightSpd:
			if (SelectShoes == ShSoapsLightSpd)
				scBBGui->GetNode("img_ch_2")->SetPatternIndex(12);
			else
				scBBGui->GetNode("img_ch_2")->SetPatternIndex(11);
			break;
		case SA2Racing:
			if (SelectShoes == ShSoapsRacing)
				scBBGui->GetNode("img_ch_2")->SetPatternIndex(14);
			else
				scBBGui->GetNode("img_ch_2")->SetPatternIndex(13);
			break;
		}
		//Riders Shoes
		if (SelectShoes == ShRiders)
			scBBGui->GetNode("img_ch_3")->SetPatternIndex(2);
		else
			scBBGui->GetNode("img_ch_3")->SetPatternIndex(1);
		//Gem Shoes
		switch (Sh06Variant)
		{
		case GemDefault:
			if (SelectShoes == Sh06GemDefault)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(2);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(1);
			break;
		case GemRed:
			if (SelectShoes == Sh06GemRed)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(12);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(11);
			break;
		case GemBlue:
			if (SelectShoes == Sh06GemBlue)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(14);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(13);
			break;
		case GemGreen:
			if (SelectShoes == Sh06GemGreen)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(16);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(15);
			break;
		case GemPurple:
			if (SelectShoes == Sh06GemPurple)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(18);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(17);
			break;
		case GemSky:
			if (SelectShoes == Sh06GemSky)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(20);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(19);
			break;
		case GemWhite:
			if (SelectShoes == Sh06GemWhite)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(22);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(21);
			break;
		case GemYellow:
			if (SelectShoes == Sh06GemYellow)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(24);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(23);
			break;
		}
		//Unleashed Shoes
		switch (ShSUVariant)
		{
		case SUAirboost:
			if (SelectShoes == ShSUAirboost)
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(2);
			else
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(1);
			break;
		case SULightSpd:
			if (SelectShoes == ShSULightSpd)
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(12);
			else
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(11);
			break;
		case SUStomp:
			if (SelectShoes == ShSUStomp)
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(14);
			else
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(13);
			break;
		case SUWallJmp:
			if (SelectShoes == ShSUWallJmp)
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(16);
			else
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(15);
			break;
		case SUWerehog:
			if (SelectShoes == ShSUWerehog)
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(18);
			else
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(17);
			break;
		case SUZoney:
			if (SelectShoes == ShSUZoney)
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(20);
			else
				scBBGui->GetNode("img_ch_5")->SetPatternIndex(19);
			break;
		}
		//Link Boots
		if (SelectShoes == ShLinkBoots)
			scBBGui->GetNode("img_ch_6")->SetPatternIndex(2);
		else
			scBBGui->GetNode("img_ch_6")->SetPatternIndex(1);
		//Boom Shoes
		if (SelectShoes == ShBoom)
			scBBGui->GetNode("img_ch_7")->SetPatternIndex(2);
		else
			scBBGui->GetNode("img_ch_7")->SetPatternIndex(1);
		//Puma Shoes
		if (SelectShoes == ShPumas)
			scBBGui->GetNode("img_ch_8")->SetPatternIndex(2);
		else
			scBBGui->GetNode("img_ch_8")->SetPatternIndex(1);
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(0);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(0);
		break;
	case UIPartBody: ////Body Icons////
		//Default Body
		if (BdBlueArmVariant)
		{
			if (SelectBody == BdBlueArms)
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(16);
			else
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(15);
		}
		else
		{
			if (SelectBody == BdDefault)
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(4);
			else
				scBBGui->GetNode("img_ch_0")->SetPatternIndex(3);
		}
		//Racing Suit
		if (SelectBody == BdRaceSuit)
			scBBGui->GetNode("img_ch_1")->SetPatternIndex(4);
		else
			scBBGui->GetNode("img_ch_1")->SetPatternIndex(3);
		//Wildfire
		if (SelectBody == BdWildFire)
			scBBGui->GetNode("img_ch_2")->SetPatternIndex(4);
		else
			scBBGui->GetNode("img_ch_2")->SetPatternIndex(3);
		//Link Tunic
		if (SelectBody == BdLinkCos)
			scBBGui->GetNode("img_ch_3")->SetPatternIndex(4);
		else
			scBBGui->GetNode("img_ch_3")->SetPatternIndex(3);
		//Scarf
		if (BdScarfVariant)
		{
			if (SelectBody == BdScarfArms)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(26);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(25);
		}
		else
		{
			if (SelectBody == BdScarf)
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(4);
			else
				scBBGui->GetNode("img_ch_4")->SetPatternIndex(3);
		}
		//Swim Vest
		if (SelectBody == Bd2020SwimVest)
			scBBGui->GetNode("img_ch_5")->SetPatternIndex(4);
		else
			scBBGui->GetNode("img_ch_5")->SetPatternIndex(3);
		//Surf Suit
		if (SelectBody == Bd2020SurfSuit)
			scBBGui->GetNode("img_ch_6")->SetPatternIndex(4);
		else
			scBBGui->GetNode("img_ch_6")->SetPatternIndex(3);
		//Horse Shirt
		if (SelectBody == Bd2020HorseShirt)
			scBBGui->GetNode("img_ch_7")->SetPatternIndex(4);
		else
			scBBGui->GetNode("img_ch_7")->SetPatternIndex(3);
		//Movie Skin
		if (BdMovieVariant)
		{
			if (SelectBody == BdMovieSkinArms)
				scBBGui->GetNode("img_ch_8")->SetPatternIndex(12);
			else
				scBBGui->GetNode("img_ch_8")->SetPatternIndex(11);
		}
		else
		{
			if (SelectBody == BdMovieSkin)
				scBBGui->GetNode("img_ch_8")->SetPatternIndex(4);
			else
				scBBGui->GetNode("img_ch_8")->SetPatternIndex(3);
		}
		//Empty Placeholders
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(1);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(1);
		break;
	case UIPartHead: ////Head Icons////
		//Default Head
		if (SelectHead == HeDefault)
			scBBGui->GetNode("img_ch_0")->SetPatternIndex(6);
		else
			scBBGui->GetNode("img_ch_0")->SetPatternIndex(5);
		//SA1 Sunglasses
		if (SelectHead == HeSA1Sunglass)
			scBBGui->GetNode("img_ch_1")->SetPatternIndex(6);
		else
			scBBGui->GetNode("img_ch_1")->SetPatternIndex(5);
		//Riders Glasses
		if (SelectHead == HeRiders)
			scBBGui->GetNode("img_ch_2")->SetPatternIndex(6);
		else
			scBBGui->GetNode("img_ch_2")->SetPatternIndex(5);
		//Headphones
		if (SelectHead == HeHeadphones)
			scBBGui->GetNode("img_ch_3")->SetPatternIndex(6);
		else
			scBBGui->GetNode("img_ch_3")->SetPatternIndex(5);
		//Link Hat
		if (SelectHead == HeLinkHat)
			scBBGui->GetNode("img_ch_4")->SetPatternIndex(6);
		else
			scBBGui->GetNode("img_ch_4")->SetPatternIndex(5);
		//Boom Spikes
		if (SelectHead == HeBoomSpikes)
			scBBGui->GetNode("img_ch_5")->SetPatternIndex(6);
		else
			scBBGui->GetNode("img_ch_5")->SetPatternIndex(5);
		//Empty Placeholders
		scBBGui->GetNode("img_ch_6")->SetPatternIndex(0);
		scBBGui->GetNode("img_ch_7")->SetPatternIndex(0);
		scBBGui->GetNode("img_ch_8")->SetPatternIndex(0);
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(2);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(2);
		break;
	case UIPartHandL: ////Hand L Icons////
		//Default Left Glove
		if (SelectHandL == HLDefault)
			scBBGui->GetNode("img_ch_0")->SetPatternIndex(8);
		else
			scBBGui->GetNode("img_ch_0")->SetPatternIndex(7);
		//Crystal Ring
		if (SelectHandL == HLCrystalRing)
			scBBGui->GetNode("img_ch_1")->SetPatternIndex(8);
		else
			scBBGui->GetNode("img_ch_1")->SetPatternIndex(7);
		//Flame Ring
		if (SelectHandL == HLFlameRing)
			scBBGui->GetNode("img_ch_2")->SetPatternIndex(8);
		else
			scBBGui->GetNode("img_ch_2")->SetPatternIndex(7);
		//Racing Left Glove
		if (SelectHandL == HLRacingGlove)
			scBBGui->GetNode("img_ch_3")->SetPatternIndex(8);
		else
			scBBGui->GetNode("img_ch_3")->SetPatternIndex(7);
		//Riders Left Glove
		if (SelectHandL == HLRiders)
			scBBGui->GetNode("img_ch_4")->SetPatternIndex(8);
		else
			scBBGui->GetNode("img_ch_4")->SetPatternIndex(7);
		//06 Bounce Bracelet
		if (SelectHandL == HL06Bounce)
			scBBGui->GetNode("img_ch_5")->SetPatternIndex(8);
		else
			scBBGui->GetNode("img_ch_5")->SetPatternIndex(7);
		//Chip Ring
		if (SelectHandL == HLChipBracelet)
			scBBGui->GetNode("img_ch_6")->SetPatternIndex(8);
		else
			scBBGui->GetNode("img_ch_6")->SetPatternIndex(7);
		//Link Left Glove
		if (SelectHandL == HLLinkGlove)
			scBBGui->GetNode("img_ch_7")->SetPatternIndex(8);
		else
			scBBGui->GetNode("img_ch_7")->SetPatternIndex(7);
		//Boom Left Glove
		if (SelectHandL == HLBoomGlove)
			scBBGui->GetNode("img_ch_8")->SetPatternIndex(8);
		else
			scBBGui->GetNode("img_ch_8")->SetPatternIndex(7);
		//Empty Placeholders
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(3);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(4);
		break;
	case UIPartHandR: ////Hand R Icons////
		//Default Right Glove
		if (SelectHandR == HRDefault)
			scBBGui->GetNode("img_ch_0")->SetPatternIndex(10);
		else
			scBBGui->GetNode("img_ch_0")->SetPatternIndex(9);
		//Bounce Bracelet
		if (HRMagicVariant)
		{
			if (SelectHandR == HRMagicHands)
				scBBGui->GetNode("img_ch_1")->SetPatternIndex(12);
			else
				scBBGui->GetNode("img_ch_1")->SetPatternIndex(11);
		}
		else
		{
			if (SelectHandR == HRBounceBracelet)
				scBBGui->GetNode("img_ch_1")->SetPatternIndex(10);
			else
				scBBGui->GetNode("img_ch_1")->SetPatternIndex(9);
		}
		//Racing Right Glove
		if (SelectHandR == HRRacingGlove)
			scBBGui->GetNode("img_ch_2")->SetPatternIndex(10);
		else
			scBBGui->GetNode("img_ch_2")->SetPatternIndex(9);
		//Riders Right Glove
		if (HRRidersVariant)
		{
			if (SelectHandR == HRArkCosmos)
				scBBGui->GetNode("img_ch_3")->SetPatternIndex(12);
			else
				scBBGui->GetNode("img_ch_3")->SetPatternIndex(11);
		}
		else
		{
			if (SelectHandR == HRRiders)
				scBBGui->GetNode("img_ch_3")->SetPatternIndex(10);
			else
				scBBGui->GetNode("img_ch_3")->SetPatternIndex(9);
		}
		//06 Homing Smash Bracelet
		if (SelectHandR == HR06HomingSmash)
			scBBGui->GetNode("img_ch_4")->SetPatternIndex(10);
		else
			scBBGui->GetNode("img_ch_4")->SetPatternIndex(9);
		//Secret Ring
		if (SelectHandR == HRSecretRing)
			scBBGui->GetNode("img_ch_5")->SetPatternIndex(10);
		else
			scBBGui->GetNode("img_ch_5")->SetPatternIndex(9);
		//Gauntlet
		if (SelectHandR == HRGauntlet)
			scBBGui->GetNode("img_ch_6")->SetPatternIndex(10);
		else
			scBBGui->GetNode("img_ch_6")->SetPatternIndex(9);
		//Link Right Glove
		if (SelectHandR == HRLinkGlove)
			scBBGui->GetNode("img_ch_7")->SetPatternIndex(10);
		else
			scBBGui->GetNode("img_ch_7")->SetPatternIndex(9);
		//Boom Right Glove
		if (SelectHandR == HRBoomGlove)
			scBBGui->GetNode("img_ch_8")->SetPatternIndex(10);
		else
			scBBGui->GetNode("img_ch_8")->SetPatternIndex(9);
		//Empty Placeholders
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(4);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(3);
		break;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
	auto scenecheck = scBBIcon && scBBGui && scBBTextArea && scBBLRMove && scBBScroll;
=======
	auto scenecheck = scBBIcon && scBBGui && scBBTextArea && scBBLRMove;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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

<<<<<<< HEAD
	CHudVarTrueSel = CHudVarVisSel + (CHudVarScroll * 3);
	float CHudVarScrollBarStarSpin = (CHudVarScrollBarStarSpinFlt >= 360 ? CHudVarScrollBarStarSpinFlt = 0 : CHudVarScrollBarStarSpinFlt += 1.5);
	float CHudVarShScrollBar = (CHudVarScroll * (100.f / CHudVarShMaxScroll));
	float CHudVarBdScrollBar = (CHudVarScroll * (100.f / CHudVarBdMaxScroll));
	float CHudVarHeScrollBar = (CHudVarScroll * (100.f / CHudVarHeMaxScroll));
	float CHudVarHLScrollBar = (CHudVarScroll * (100.f / CHudVarHLMaxScroll));
	float CHudVarHRScrollBar = (CHudVarScroll * (100.f / CHudVarHRMaxScroll));


	////------Open Fitting Menu------////
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
			CHudUIOpen(This, Edx, in_rUpdateInfo, CHudVarShScrollBar, CHudVarBdScrollBar, CHudVarHeScrollBar, CHudVarHLScrollBar, CHudVarHRScrollBar);
=======
			CHudUIOpen(This, Edx, in_rUpdateInfo);
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			break;
		}
	}

<<<<<<< HEAD

	////------Main Fitting Menu------////
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD

			////------Automatics Handling------////

			////------Handle Cursor Position
			switch (CHudVarVisSel)
=======
			//Handle Cursor Position//
			switch (ItemOption)
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
				switch (CHudVarVisSel)
=======
				switch (ItemOption)
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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

<<<<<<< HEAD
			////------Icon Handler
			if (scenecheck)
				CHudUIThumbManager(This);

			////------Scrollbar Handler
			if (scBBScroll->m_MotionFrame >= scBBScroll->m_MotionEndFrame)
				IsInScrollOpen = false;
			if (scenecheck && IsInScrollOpen == false)
			{
				scBBScroll->GetNode("star")->SetRotation(CHudVarScrollBarStarSpin);
				CHudUIScrollManager(This, CHudVarShScrollBar, CHudVarBdScrollBar, CHudVarHeScrollBar, CHudVarHLScrollBar, CHudVarHRScrollBar);
			}
				
			////------SWA Open Timer Unhide
=======
			////Icon Handlers////
			if (scenecheck)
			{
				CHudUIThumbManager();
			}

			//SWA Open Timer Unhide
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
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
=======

			//Handle Alt Prompt
			switch (MenuOption)
			{
			case UIPartShoes:
				switch (ItemOption)
				{
				case 0:
				case 2:
				case 4:
				case 5:
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
				switch (CHudVarTrueSel)
				{
				case HRSA2Bounce:
=======
				switch (ItemOption)
				{
				case 1:
				case 3:
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
			case UIPartHead:
				switch (CHudVarTrueSel)
				{
				case HeSimulator:
=======
			case UIPartBody:
				switch (ItemOption)
				{
				case 0:
				case 4:
				case 8:
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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

<<<<<<< HEAD


			////------Menu Input Handling------////

			////------A Button Press
=======
			//Functionality//
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			if (scenecheck && (PressedA && scBBIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUISelect();
			}

<<<<<<< HEAD
			////------B Button Press
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			if (scenecheck && PressedB && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUIExit(0);
				return;
			}

<<<<<<< HEAD
			////------Y Button Press
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			if (scenecheck && (PressedY && scBBIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUIAlt();
			}

<<<<<<< HEAD
			////------Shoulder Button Press
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			if (scenecheck && (PressedRB || PressedLB) && !IsInMenuExit)
			{
				CHudUISwitch(0);
			}

<<<<<<< HEAD
			////------B Button Exit
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			if (scenecheck && IsInMenuExit && scBBGui->m_MotionFrame <= 0 && !IsInMenuChange)
			{
				CHudUIExit(1);
			}

<<<<<<< HEAD
			////------Shoulder Button Change
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			if (scenecheck && IsInMenuChange && scBBGui->m_MotionFrame <= 0 && !IsInMenuExit)
			{
				CHudUISwitch(1);
			}

<<<<<<< HEAD
			////------Directional Movement Handle
=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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

<<<<<<< HEAD
=======
			//printf("\n");
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
			break;
		}
	}

<<<<<<< HEAD
	////------Iterators and Timers------////

=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD


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

=======
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
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
<<<<<<< HEAD
	CHudTabSel = UIPartShoes;
	CHudVarVisSel = 0;
=======
	MenuOption = UIPartShoes;
	ItemOption = 0;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
	MemoryOpenTimer = 0;
	FILE* pFile = fopen("CustomSelect.ini", "wb");
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
<<<<<<< HEAD
	CHudTabSel = UIPartShoes;
	CHudVarVisSel = 0;
=======
	MenuOption = UIPartShoes;
	ItemOption = 0;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
	MemoryOpenTimer = 0;
	FILE* pFile = fopen("CustomSelect.ini", "wb");
	WriteINI(pFile);
	fclose(pFile);
	CHudUISceneDestroy();
}
HOOK(void, __fastcall, CHudResultStart, 0x010B6840, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	IsInMenuExit = false;
	IsInMenu = false;
<<<<<<< HEAD
	CHudTabSel = UIPartShoes;
	CHudVarVisSel = 0;
=======
	MenuOption = UIPartShoes;
	ItemOption = 0;
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
	MemoryOpenTimer = 0;
	FILE* pFile = fopen("CustomSelect.ini", "wb");
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
<<<<<<< HEAD
HOOK(void, __cdecl, InitializeApplicationUIParams, 0x00D65180, Sonic::CParameterFile* This)
=======
HOOK(void, __cdecl, InitializeApplicationParams, 0x00D65180, Sonic::CParameterFile* This)
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
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

<<<<<<< HEAD
	originalInitializeApplicationUIParams(This);
=======
	originalInitializeApplicationParams(This);
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
}

//Install UI
void InstallCustomUI::applyPatches()
{
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
<<<<<<< HEAD
	INSTALL_HOOK(InitializeApplicationUIParams);
	//INSTALL_HOOK(CastStuffTest);
	//if (ActivateButton >= 5 && ActivateButton <= 8)
		WRITE_JUMP(0xD97B56, (void*)0xD97B9E); // Ignore D-pad input for Sonic's control
=======
	INSTALL_HOOK(InitializeApplicationParams);
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
}