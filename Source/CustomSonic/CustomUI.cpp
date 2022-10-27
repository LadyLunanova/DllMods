#include <Common.h>
#include "UpdateDirector.h"
#include "CustomSonicItems.h"
#include "InstallCustomUI.h"
#include "CustomUI.h"

//Declare Funcs
void ReadINI();
void WriteINI(FILE* iniFile);

//Handle UI
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

	ReadINI();
	prevblur = *ENABLE_BLUR;
	*ENABLE_BLUR = false;
	if (MemoryOpenTimer <= 0)
	{
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
	}
	//Misc
	IsInMenu = true;
	CHudUISFXOpen();
	return;
}

void CHudUISelect()
{
	//SWAOpenTimer = 10;
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
		}
		return;
		break;
	case UIPartBody:
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
		}
		return;
		break;
	case UIPartHead:
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
		}
		return;
		break;
	case UIPartHandL:
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
		}
		return;
		break;
	case UIPartHandR:
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
		}
		return;
		break;
	}
}

void CHudUIMove(int Type)
{
	CHudUISFXMove();
	CHudUICursorAnim();
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
			break;
		}
		return;
		break;
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
			break;
		}
		return;
		break;
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
			break;
		}
		return;
		break;
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
			break;
		}
		return;
		break;
	}
}

void CHudUIAlt()
{
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
				break;
			}
			CHudUISFXAlt();
			CHudUICursorAnim();
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
				break;
			}
			CHudUISFXAlt();
			CHudUICursorAnim();
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
				break;
			}
		}
		if (IsInMenuChangeL)
		{
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
	auto scenecheck = scBBIcon && scBBGui && scBBTextArea && scBBLRMove;
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
			CHudUIOpen(This, Edx, in_rUpdateInfo);
			break;
		}
	}

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
			//Handle Cursor Position//
			switch (ItemOption)
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
				switch (ItemOption)
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

			////Icon Handlers////
			if (scenecheck)
			{
				CHudUIThumbManager();
			}

			//SWA Open Timer Unhide
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
				switch (ItemOption)
				{
				case 1:
				case 3:
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
				switch (ItemOption)
				{
				case 0:
				case 4:
				case 8:
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

			//Functionality//
			if (scenecheck && (PressedA && scBBIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUISelect();
			}

			if (scenecheck && PressedB && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUIExit(0);
				return;
			}

			if (scenecheck && (PressedY && scBBIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUIAlt();
			}

			if (scenecheck && (PressedRB || PressedLB) && !IsInMenuExit)
			{
				CHudUISwitch(0);
			}

			if (scenecheck && IsInMenuExit && scBBGui->m_MotionFrame <= 0 && !IsInMenuChange)
			{
				CHudUIExit(1);
			}

			if (scenecheck && IsInMenuChange && scBBGui->m_MotionFrame <= 0 && !IsInMenuExit)
			{
				CHudUISwitch(1);
			}

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

			//printf("\n");
			break;
		}
	}

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
	MenuOption = UIPartShoes;
	ItemOption = 0;
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
	MenuOption = UIPartShoes;
	ItemOption = 0;
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
	MenuOption = UIPartShoes;
	ItemOption = 0;
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
HOOK(void, __cdecl, InitializeApplicationParams, 0x00D65180, Sonic::CParameterFile* This)
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

	originalInitializeApplicationParams(This);
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
	INSTALL_HOOK(InitializeApplicationParams);
}