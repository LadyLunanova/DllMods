#include <Common.h>
#include "UpdateDirector.h"
#include "CustomSonicItems.h"
#include "InstallCustomUI.h"
#include "CustomUI.h"

//Declare Funcs
void ReadINI();
void WriteINI(FILE* iniFile);

//Handle UI
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
	auto scenecheck = scIcon && scChara && scTextArea && scLRMove;
	FUNCTION_PTR(void, __thiscall, changeState, 0x773250, void* This, boost::shared_ptr<void>&spState, const Hedgehog::Base::CSharedString name);
	boost::shared_ptr<void> spState;

	if (PressedSL && !IsInMenu && IsModernSonic && !ExternalControl && !Goal && !((getPackedID == pam000) && IsOutOfControl) && (IsLookAt == false))
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
			ReadINI();
			prevblur = *ENABLE_BLUR;
			*ENABLE_BLUR = false;
			MenuOption = UIPartShoes;
			ItemOption = 0;
			if (!obCustomUI) //Create UI project if it doesn't exist
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
			scChara = prFittingScreen->CreateScene("chara");
			scChara->SetPosition(0, 0);
			scChara->SetMotion("Intro_Anim");
			scChara->SetMotionFrame(0.0f);
			scChara->m_MotionDisableFlag = false;
			scChara->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scChara->m_MotionSpeed = 1.0f;
			scChara->Update();
			//Cursor
			scIcon = prFittingScreen->CreateScene("icon");
			scIcon->SetPosition(rowX1 * arX, rowY1 * arY);
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
			//Category Text
			scTextArea = prFittingScreen->CreateScene("textarea");
			scTextArea->SetPosition(0, 0);
			scTextArea->SetMotion("Intro_Anim");
			scTextArea->SetMotionFrame(0.0f);
			scTextArea->m_MotionDisableFlag = false;
			scTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scTextArea->m_MotionSpeed = 0.5f;
			scTextArea->Update();
			//L/R Bumpers
			scLRMove = prFittingScreen->CreateScene("LRmove");
			scLRMove->SetPosition(0, 0);
			scLRMove->SetMotion("Intro_Anim");
			scLRMove->SetMotionFrame(0.0f);
			scLRMove->m_MotionDisableFlag = false;
			scLRMove->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scLRMove->m_MotionSpeed = 1.0f;
			scLRMove->Update();
			//Decoration
			if (ConfigDecoEnable)
			{
				scDeco = prFittingScreen->CreateScene("deco");
				scDeco->SetPosition(0, 0);
				scDeco->SetMotion("Color_change");
				scDeco->SetMotionFrame(1.0f);
				scDeco->m_MotionDisableFlag = false;
				scDeco->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scDeco->m_MotionSpeed = 0.0f;
				scDeco->Update();
				scDeco->SetMotion("Intro_Anim");
				scDeco->SetMotionFrame(0.0f);
				scDeco->m_MotionDisableFlag = false;
				scDeco->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scDeco->m_MotionSpeed = 1.0f;
				scDeco->Update();

			}
			//Misc
			IsInMenu = true;
			Common::PlaySoundStatic(menuSoundHandle, 1000002);
			return;
			break;
		}
	}

	if (IsInMenu && scIcon && scChara && IsModernSonic && !ExternalControl && !Goal && !((getPackedID == pam000) && IsOutOfControl))
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
				scIcon->SetPosition(rowX1 * arX, rowY1 * arY);
				break;
			case 1:
				scIcon->SetPosition(rowX2 * arX, rowY1 * arY);
				break;
			case 2:
				scIcon->SetPosition(rowX3 * arX, rowY1 * arY);
				break;
			case 3:
				scIcon->SetPosition(rowX1 * arX, rowY2 * arY);
				break;
			case 4:
				scIcon->SetPosition(rowX2 * arX, rowY2 * arY);
				break;
			case 5:
				scIcon->SetPosition(rowX3 * arX, rowY2 * arY);
				break;
			case 6:
				scIcon->SetPosition(rowX1 * arX, rowY3 * arY);
				break;
			case 7:
				scIcon->SetPosition(rowX2 * arX, rowY3 * arY);
				break;
			case 8:
				scIcon->SetPosition(rowX3 * arX, rowY3 * arY);
				break;
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
					scIcon->GetNode("star")->SetPatternIndex(1);
					break;
				case 8:
					//scIcon->GetNode("star")->SetText("1");
					break;
				default:
					scIcon->GetNode("star")->SetPatternIndex(0);
					break;
				}
				break;
			case UIPartHandR:
				switch (ItemOption)
				{
				case 1:
				case 3:
					scIcon->GetNode("star")->SetPatternIndex(1);
					break;
				default:
					scIcon->GetNode("star")->SetPatternIndex(0);
					break;
				}
				break;
			case UIPartBody:
				switch (ItemOption)
				{
				case 0:
				case 4:
				case 8:
					scIcon->GetNode("star")->SetPatternIndex(1);
					break;
				default:
					scIcon->GetNode("star")->SetPatternIndex(0);
					break;
				}
				break;
			default:
				scIcon->GetNode("star")->SetPatternIndex(0);
				break;
			}

			////Icon Handlers////
			if (scenecheck)
			{
				switch (MenuOption)
				{
				case UIPartShoes: ////Shoe Icons////
					//Default Shoes
					switch (ShDefaultVariant)
					{
					case ShoeDefault:
						if (SelectShoes == ShDefault)
							scChara->GetNode("img_ch_0")->SetPatternIndex(2);
						else
							scChara->GetNode("img_ch_0")->SetPatternIndex(1);
						break;
					case ShoeClassic:
						if (SelectShoes == ShClassic)
							scChara->GetNode("img_ch_0")->SetPatternIndex(12);
						else
							scChara->GetNode("img_ch_0")->SetPatternIndex(11);
						break;
					case ShoeArchie:
						if (SelectShoes == ShArchie)
							scChara->GetNode("img_ch_0")->SetPatternIndex(14);
						else
							scChara->GetNode("img_ch_0")->SetPatternIndex(13);
						break;
					}
					//SA1 Lightspeed Shoes
					if (SelectShoes == ShSA1LightSpd)
						scChara->GetNode("img_ch_1")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_1")->SetPatternIndex(1);
					//Soap Shoes
					switch (ShSA2Variant)
					{
					case SA2Soap:
						if (SelectShoes == ShSoaps)
							scChara->GetNode("img_ch_2")->SetPatternIndex(2);
						else
							scChara->GetNode("img_ch_2")->SetPatternIndex(1);
						break;
					case SA2LightSpd:
						if (SelectShoes == ShSoapsLightSpd)
							scChara->GetNode("img_ch_2")->SetPatternIndex(12);
						else
							scChara->GetNode("img_ch_2")->SetPatternIndex(11);
						break;
					case SA2Racing:
						if (SelectShoes == ShSoapsRacing)
							scChara->GetNode("img_ch_2")->SetPatternIndex(14);
						else
							scChara->GetNode("img_ch_2")->SetPatternIndex(13);
						break;
					}
					//Riders Shoes
					if (SelectShoes == ShRiders)
						scChara->GetNode("img_ch_3")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_3")->SetPatternIndex(1);
					//Gem Shoes
					switch (Sh06Variant)
					{
					case GemDefault:
						if (SelectShoes == Sh06GemDefault)
							scChara->GetNode("img_ch_4")->SetPatternIndex(2);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(1);
						break;
					case GemRed:
						if (SelectShoes == Sh06GemRed)
							scChara->GetNode("img_ch_4")->SetPatternIndex(12);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(11);
						break;
					case GemBlue:
						if (SelectShoes == Sh06GemBlue)
							scChara->GetNode("img_ch_4")->SetPatternIndex(14);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(13);
						break;
					case GemGreen:
						if (SelectShoes == Sh06GemGreen)
							scChara->GetNode("img_ch_4")->SetPatternIndex(16);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(15);
						break;
					case GemPurple:
						if (SelectShoes == Sh06GemPurple)
							scChara->GetNode("img_ch_4")->SetPatternIndex(18);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(17);
						break;
					case GemSky:
						if (SelectShoes == Sh06GemSky)
							scChara->GetNode("img_ch_4")->SetPatternIndex(20);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(19);
						break;
					case GemWhite:
						if (SelectShoes == Sh06GemWhite)
							scChara->GetNode("img_ch_4")->SetPatternIndex(22);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(21);
						break;
					case GemYellow:
						if (SelectShoes == Sh06GemYellow)
							scChara->GetNode("img_ch_4")->SetPatternIndex(24);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(23);
						break;
					}
					//Unleashed Shoes
					switch (ShSUVariant)
					{
					case SUAirboost:
						if (SelectShoes == ShSUAirboost)
							scChara->GetNode("img_ch_5")->SetPatternIndex(2);
						else
							scChara->GetNode("img_ch_5")->SetPatternIndex(1);
						break;
					case SULightSpd:
						if (SelectShoes == ShSULightSpd)
							scChara->GetNode("img_ch_5")->SetPatternIndex(12);
						else
							scChara->GetNode("img_ch_5")->SetPatternIndex(11);
						break;
					case SUStomp:
						if (SelectShoes == ShSUStomp)
							scChara->GetNode("img_ch_5")->SetPatternIndex(14);
						else
							scChara->GetNode("img_ch_5")->SetPatternIndex(13);
						break;
					case SUWallJmp:
						if (SelectShoes == ShSUWallJmp)
							scChara->GetNode("img_ch_5")->SetPatternIndex(16);
						else
							scChara->GetNode("img_ch_5")->SetPatternIndex(15);
						break;
					case SUWerehog:
						if (SelectShoes == ShSUWerehog)
							scChara->GetNode("img_ch_5")->SetPatternIndex(18);
						else
							scChara->GetNode("img_ch_5")->SetPatternIndex(17);
						break;
					case SUZoney:
						if (SelectShoes == ShSUZoney)
							scChara->GetNode("img_ch_5")->SetPatternIndex(20);
						else
							scChara->GetNode("img_ch_5")->SetPatternIndex(19);
						break;
					}
					//Link Boots
					if (SelectShoes == ShLinkBoots)
						scChara->GetNode("img_ch_6")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_6")->SetPatternIndex(1);
					//Boom Shoes
					if (SelectShoes == ShBoom)
						scChara->GetNode("img_ch_7")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_7")->SetPatternIndex(1);
					//Puma Shoes
					if (SelectShoes == ShPumas)
						scChara->GetNode("img_ch_8")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_8")->SetPatternIndex(1);
					scTextArea->GetNode("textarea_textbox")->SetPatternIndex(0);
					break;
				case UIPartBody: ////Body Icons////
					//Default Body
					if (SelectBody == BdDefault || SelectBody == BdBlueArms)
						scChara->GetNode("img_ch_0")->SetPatternIndex(4);
					else
						scChara->GetNode("img_ch_0")->SetPatternIndex(3);
					//Racing Suit
					if (SelectBody == BdRaceSuit)
						scChara->GetNode("img_ch_1")->SetPatternIndex(4);
					else
						scChara->GetNode("img_ch_1")->SetPatternIndex(3);
					//Wildfire
					if (SelectBody == BdWildFire)
						scChara->GetNode("img_ch_2")->SetPatternIndex(4);
					else
						scChara->GetNode("img_ch_2")->SetPatternIndex(3);
					//Link Tunic
					if (SelectBody == BdLinkCos)
						scChara->GetNode("img_ch_3")->SetPatternIndex(4);
					else
						scChara->GetNode("img_ch_3")->SetPatternIndex(3);
					//Scarf
					if (BdScarfVariant)
					{
						if (SelectBody == BdScarfArms)
							scChara->GetNode("img_ch_4")->SetPatternIndex(26);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(25);
					}
					else
					{
						if (SelectBody == BdScarf)
							scChara->GetNode("img_ch_4")->SetPatternIndex(4);
						else
							scChara->GetNode("img_ch_4")->SetPatternIndex(3);
					}
					//Swim Vest
					if (SelectBody == Bd2020SwimVest)
						scChara->GetNode("img_ch_5")->SetPatternIndex(4);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(3);
					//Surf Suit
					if (SelectBody == Bd2020SurfSuit)
						scChara->GetNode("img_ch_6")->SetPatternIndex(4);
					else
						scChara->GetNode("img_ch_6")->SetPatternIndex(3);
					//Horse Shirt
					if (SelectBody == Bd2020HorseShirt)
						scChara->GetNode("img_ch_7")->SetPatternIndex(4);
					else
						scChara->GetNode("img_ch_7")->SetPatternIndex(3);
					//Movie Skin
					if (SelectBody == BdMovieSkin || SelectBody == BdMovieSkinArms)
						scChara->GetNode("img_ch_8")->SetPatternIndex(4);
					else
						scChara->GetNode("img_ch_8")->SetPatternIndex(3);
					//Empty Placeholders
					scTextArea->GetNode("textarea_textbox")->SetPatternIndex(1);
					break;
				case UIPartHead: ////Head Icons////
					//Default Head
					if (SelectHead == HeDefault)
						scChara->GetNode("img_ch_0")->SetPatternIndex(6);
					else
						scChara->GetNode("img_ch_0")->SetPatternIndex(5);
					//SA1 Sunglasses
					if (SelectHead == HeSA1Sunglass)
						scChara->GetNode("img_ch_1")->SetPatternIndex(6);
					else
						scChara->GetNode("img_ch_1")->SetPatternIndex(5);
					//Riders Glasses
					if (SelectHead == HeRiders)
						scChara->GetNode("img_ch_2")->SetPatternIndex(6);
					else
						scChara->GetNode("img_ch_2")->SetPatternIndex(5);
					//Headphones
					if (SelectHead == HeHeadphones)
						scChara->GetNode("img_ch_3")->SetPatternIndex(6);
					else
						scChara->GetNode("img_ch_3")->SetPatternIndex(5);
					//Link Hat
					if (SelectHead == HeLinkHat)
						scChara->GetNode("img_ch_4")->SetPatternIndex(6);
					else
						scChara->GetNode("img_ch_4")->SetPatternIndex(5);
					//Boom Spikes
					if (SelectHead == HeBoomSpikes)
						scChara->GetNode("img_ch_5")->SetPatternIndex(6);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(5);
					//Empty Placeholders
					scChara->GetNode("img_ch_6")->SetPatternIndex(5);
					scChara->GetNode("img_ch_7")->SetPatternIndex(5);
					scChara->GetNode("img_ch_8")->SetPatternIndex(5);
					scTextArea->GetNode("textarea_textbox")->SetPatternIndex(2);
					break;
				case UIPartHandL: ////Hand L Icons////
					//Default Left Glove
					if (SelectHandL == HLDefault)
						scChara->GetNode("img_ch_0")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_0")->SetPatternIndex(7);
					//Crystal Ring
					if (SelectHandL == HLCrystalRing)
						scChara->GetNode("img_ch_1")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_1")->SetPatternIndex(7);
					//Flame Ring
					if (SelectHandL == HLFlameRing)
						scChara->GetNode("img_ch_2")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_2")->SetPatternIndex(7);
					//Racing Left Glove
					if (SelectHandL == HLRacingGlove)
						scChara->GetNode("img_ch_3")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_3")->SetPatternIndex(7);
					//Riders Left Glove
					if (SelectHandL == HLRiders)
						scChara->GetNode("img_ch_4")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_4")->SetPatternIndex(7);
					//06 Bounce Bracelet
					if (SelectHandL == HL06Bounce)
						scChara->GetNode("img_ch_5")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(7);
					//Chip Ring
					if (SelectHandL == HLChipBracelet)
						scChara->GetNode("img_ch_6")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_6")->SetPatternIndex(7);
					//Link Left Glove
					if (SelectHandL == HLLinkGlove)
						scChara->GetNode("img_ch_7")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_7")->SetPatternIndex(7);
					//Boom Left Glove
					if (SelectHandL == HLBoomGlove)
						scChara->GetNode("img_ch_8")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_8")->SetPatternIndex(7);
					//Empty Placeholders
					scTextArea->GetNode("textarea_textbox")->SetPatternIndex(3);
					break;
				case UIPartHandR: ////Hand R Icons////
					//Default Right Glove
					if (SelectHandR == HRDefault)
						scChara->GetNode("img_ch_0")->SetPatternIndex(10);
					else
						scChara->GetNode("img_ch_0")->SetPatternIndex(9);
					//Bounce Bracelet
					if (HRMagicVariant)
					{
						if (SelectHandR == HRMagicHands)
							scChara->GetNode("img_ch_1")->SetPatternIndex(12);
						else
							scChara->GetNode("img_ch_1")->SetPatternIndex(11);
					}
					else
					{
						if (SelectHandR == HRBounceBracelet)
							scChara->GetNode("img_ch_1")->SetPatternIndex(10);
						else
							scChara->GetNode("img_ch_1")->SetPatternIndex(9);
					}
					//Racing Right Glove
					if (SelectHandR == HRRacingGlove)
						scChara->GetNode("img_ch_2")->SetPatternIndex(10);
					else
						scChara->GetNode("img_ch_2")->SetPatternIndex(9);
					//Riders Right Glove
					if (HRRidersVariant)
					{
						if (SelectHandR == HRArkCosmos)
							scChara->GetNode("img_ch_3")->SetPatternIndex(12);
						else
							scChara->GetNode("img_ch_3")->SetPatternIndex(11);
					}
					else
					{
						if (SelectHandR == HRRiders)
							scChara->GetNode("img_ch_3")->SetPatternIndex(10);
						else
							scChara->GetNode("img_ch_3")->SetPatternIndex(9);
					}
					//06 Homing Smash Bracelet
					if (SelectHandR == HR06HomingSmash)
						scChara->GetNode("img_ch_4")->SetPatternIndex(10);
					else
						scChara->GetNode("img_ch_4")->SetPatternIndex(9);
					//Secret Ring
					if (SelectHandR == HRSecretRing)
						scChara->GetNode("img_ch_5")->SetPatternIndex(10);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(9);
					//Gauntlet
					if (SelectHandR == HRGauntlet)
						scChara->GetNode("img_ch_6")->SetPatternIndex(10);
					else
						scChara->GetNode("img_ch_6")->SetPatternIndex(9);
					//Link Right Glove
					if (SelectHandR == HRLinkGlove)
						scChara->GetNode("img_ch_7")->SetPatternIndex(10);
					else
						scChara->GetNode("img_ch_7")->SetPatternIndex(9);
					//Boom Right Glove
					if (SelectHandR == HRBoomGlove)
						scChara->GetNode("img_ch_8")->SetPatternIndex(10);
					else
						scChara->GetNode("img_ch_8")->SetPatternIndex(9);
					//Empty Placeholders
					scTextArea->GetNode("textarea_textbox")->SetPatternIndex(4);
					break;
				}

			}

			//Functionality//
			if (scenecheck && (PressedA && scIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				switch (MenuOption)
				{
				case UIPartShoes:
					switch (ItemOption)
					{
					case 0:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectShoes = ShSA1LightSpd;
						break;
					case 2:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectShoes = ShRiders;
						break;
					case 4:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectShoes = ShLinkBoots;
						break;
					case 7:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectShoes = ShBoom;
						break;
					case 8:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectShoes = ShPumas;
						break;
					}
					return;
					break;
				case UIPartBody:
					switch (ItemOption)
					{
					case 0:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						if (!BdBlueArmVariant)
							SelectBody = BdDefault;
						else
							SelectBody = BdBlueArms;
						break;
					case 1:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectBody = BdRaceSuit;
						break;
					case 2:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectBody = BdWildFire;
						break;
					case 3:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectBody = BdLinkCos;
						break;
					case 4:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						if (!BdScarfVariant)
							SelectBody = BdScarf;
						else
							SelectBody = BdScarfArms;
						break;
					case 5:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectBody = Bd2020SwimVest;
						break;
					case 6:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectBody = Bd2020SurfSuit;
						break;
					case 7:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectBody = Bd2020HorseShirt;
						break;
					case 8:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHead = HeDefault;
						break;
					case 1:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHead = HeSA1Sunglass;
						break;
					case 2:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHead = HeRiders;
						break;
					case 3:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHead = HeHeadphones;
						break;
					case 4:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHead = HeLinkHat;
						break;
					case 5:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHead = HeBoomSpikes;
						break;
					case 6:
						Common::PlaySoundStatic(menuSoundHandle, 1000007);
						break;
					case 7:
						Common::PlaySoundStatic(menuSoundHandle, 1000007);
						break;
					case 8:
						Common::PlaySoundStatic(menuSoundHandle, 1000007);
						break;
					}
					return;
					break;
				case UIPartHandL:
					switch (ItemOption)
					{
					case 0:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HLDefault;
						break;
					case 1:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HLCrystalRing;
						break;
					case 2:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HLFlameRing;
						break;
					case 3:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HLRacingGlove;
						break;
					case 4:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HLRiders;
						break;
					case 5:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HL06Bounce;
						break;
					case 6:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HLChipBracelet;
						break;
					case 7:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HLLinkGlove;
						break;
					case 8:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HLBoomGlove;
						break;
					}
					return;
					break;
				case UIPartHandR:
					switch (ItemOption)
					{
					case 0:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandR = HRDefault;
						break;
					case 1:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						if (!HRMagicVariant)
							SelectHandR = HRBounceBracelet;
						else
							SelectHandR = HRMagicHands;
						break;
					case 2:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandR = HRRacingGlove;
						break;
					case 3:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						if (!HRRidersVariant)
							SelectHandR = HRRiders;
						else
							SelectHandR = HRArkCosmos;
						break;
					case 4:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandR = HR06HomingSmash;
						break;
					case 5:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandR = HRSecretRing;
						break;
					case 6:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandR = HRGauntlet;
						break;
					case 7:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandR = HRLinkGlove;
						break;
					case 8:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandR = HRBoomGlove;
						break;
					}
					return;
					break;
				}
			}

			if (scenecheck && PressedB && !IsInMenuExit && !IsInMenuChange)
			{
				Common::PlaySoundStatic(menuSoundHandle, 1000003);
				IsInMenuExit = true;
				//IsInMenu = false;
				//MenuOption = 0;
				//ItemOption = 0;
				//Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scChara);
				//Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scIcon);
				scChara->SetMotion("Intro_Anim");
				scChara->SetMotionFrame(22.0f);
				scChara->m_MotionDisableFlag = false;
				scChara->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scChara->m_MotionSpeed = -1.0f;
				scChara->Update();
				scIcon->SetMotion("OFF_Anim");
				scIcon->SetMotionFrame(0.0f);
				scIcon->m_MotionDisableFlag = false;
				scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scIcon->m_MotionSpeed = 1.0f;
				scIcon->Update();
				scTextArea->SetMotion("Intro_Anim");
				scTextArea->SetMotionFrame(17.0f);
				scTextArea->m_MotionDisableFlag = false;
				scTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scTextArea->m_MotionSpeed = -0.5f;
				scTextArea->Update();
				if (scDeco)
				{
					scDeco->SetMotion("Intro_Anim");
					scDeco->SetMotionFrame(23.0f);
					scDeco->m_MotionDisableFlag = false;
					scDeco->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
					scDeco->m_MotionSpeed = -1.0f;
					scDeco->Update();
				}
				return;
			}

			if (scenecheck && (PressedY && scIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						break;
					case 4:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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

			if (scenecheck && (PressedRB || PressedLB) && !IsInMenuExit)
			{
				Common::PlaySoundStatic(menuSoundHandle, 1000006);
				IsInMenuChange = true;
				if (PressedRB)
				{
					IsInMenuChangeR = true;
					scLRMove->SetMotion("Right_ON_Anim");
					scLRMove->SetMotionFrame(0.0f);
					scLRMove->m_MotionDisableFlag = false;
					scLRMove->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
					scLRMove->m_MotionSpeed = 1.0f;
					scLRMove->Update();
				}
				if (PressedLB)
				{
					IsInMenuChangeL = true;
					scLRMove->SetMotion("Left_ON_Anim");
					scLRMove->SetMotionFrame(0.0f);
					scLRMove->m_MotionDisableFlag = false;
					scLRMove->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
					scLRMove->m_MotionSpeed = 1.0f;
					scLRMove->Update();
				}
				IsInMenuExit = false;
				//MenuOption = 0;
				//ItemOption = 0;
				scChara->SetMotion("Intro_Anim");
				scChara->SetMotionFrame(22.0f);
				scChara->m_MotionDisableFlag = false;
				scChara->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scChara->m_MotionSpeed = -2.0f;
				scChara->Update();
				scIcon->SetMotion("OFF_Anim");
				scIcon->SetMotionFrame(0.0f);
				scIcon->m_MotionDisableFlag = false;
				scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scIcon->m_MotionSpeed = 1.0f;
				scIcon->Update();
				scTextArea->SetMotion("Intro_Anim");
				scTextArea->SetMotionFrame(17.0f);
				scTextArea->m_MotionDisableFlag = false;
				scTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scTextArea->m_MotionSpeed = -1.8f;
				scTextArea->Update();
				return;
			}

			if (scenecheck && IsInMenuExit && scChara->m_MotionFrame <= 0 && !IsInMenuChange)
			{
				IsInMenuExit = false;
				IsInMenu = false;
				MenuOption = UIPartShoes;
				ItemOption = 0;
				*ENABLE_BLUR = prevblur;
				FILE* pFile = fopen("CustomSelect.ini", "wb");
				WriteINI(pFile);
				fclose(pFile);
				Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scChara);
				Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scIcon);
				Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scTextArea);
				Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scLRMove);
				if (scDeco)
					Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scDeco);
				return;
			}

			if (scenecheck && IsInMenuChange && scChara->m_MotionFrame <= 0 && !IsInMenuExit)
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
				scChara->SetMotion("Intro_Anim");
				scChara->SetMotionFrame(0.0f);
				scChara->m_MotionDisableFlag = false;
				scChara->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scChara->m_MotionSpeed = 2.0f;
				scChara->Update();
				PlayCursorAnim();
				//scIcon->SetMotion("ON_Anim");
				//scIcon->SetMotionFrame(0.0f);
				//scIcon->m_MotionDisableFlag = false;
				//scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				//scIcon->m_MotionSpeed = 1.0f;
				//scIcon->Update();
				scTextArea->SetMotion("Intro_Anim");
				scTextArea->SetMotionFrame(0.0f);
				scTextArea->m_MotionDisableFlag = false;
				scTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				scTextArea->m_MotionSpeed = 1.0f;
				scTextArea->Update();
				return;
			}

			if (scenecheck && ((PressedUp && scIcon->m_MotionFrame >= 3) || (PushedUp && scIcon->m_MotionFrame >= 7)) && !IsInMenuExit && !IsInMenuChange)
			{
				Common::PlaySoundStatic(menuSoundHandle, 1000004);
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
				PlayCursorAnim();
				//scIcon->SetMotion("ON_Anim");
				//scIcon->SetMotionFrame(0.0f);
				//scIcon->m_MotionDisableFlag = false;
				//scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				//scIcon->m_MotionSpeed = 1.0f;
				//scIcon->Update();
				return;
			}

			if (scenecheck && ((PressedDown && scIcon->m_MotionFrame >= 3) || (PushedDown && scIcon->m_MotionFrame >= 7)) && !IsInMenuExit && !IsInMenuChange)
			{
				Common::PlaySoundStatic(menuSoundHandle, 1000004);
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
				PlayCursorAnim();
				//scIcon->SetMotion("ON_Anim");
				//scIcon->SetMotionFrame(0.0f);
				//scIcon->m_MotionDisableFlag = false;
				//scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				//scIcon->m_MotionSpeed = 1.0f;
				//scIcon->Update();
				return;
			}

			if (scenecheck && ((PressedLeft && scIcon->m_MotionFrame >= 3) || (PushedLeft && scIcon->m_MotionFrame >= 7)) && !IsInMenuExit && !IsInMenuChange)
			{
				Common::PlaySoundStatic(menuSoundHandle, 1000004);
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
				PlayCursorAnim();
				//scIcon->SetMotion("ON_Anim");
				//scIcon->SetMotionFrame(0.0f);
				//scIcon->m_MotionDisableFlag = false;
				//scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				//scIcon->m_MotionSpeed = 1.0f;
				//scIcon->Update();
				return;
			}

			if (scenecheck && ((PressedRight && scIcon->m_MotionFrame >= 3) || (PushedRight && scIcon->m_MotionFrame >= 7)) && !IsInMenuExit && !IsInMenuChange)
			{
				Common::PlaySoundStatic(menuSoundHandle, 1000004);
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
				PlayCursorAnim();
				//scIcon->SetMotion("ON_Anim");
				//scIcon->SetMotionFrame(0.0f);
				//scIcon->m_MotionDisableFlag = false;
				//scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				//scIcon->m_MotionSpeed = 1.0f;
				//scIcon->Update();
				return;
			}

			//printf("\n");
			break;
		}
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
	FILE* pFile = fopen("CustomSelect.ini", "wb");
	WriteINI(pFile);
	fclose(pFile);
	if (scChara)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scChara);
	if (scIcon)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scIcon);
	if (scTextArea)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scTextArea);
	if (scLRMove)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scLRMove);
	if (scDeco)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scDeco);
	originalCHUDPauseUpdate(This);
}
void __fastcall CHudSonicStageRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreen();
	IsInMenuExit = false;
	IsInMenu = false;
	MenuOption = UIPartShoes;
	ItemOption = 0;
	FILE* pFile = fopen("CustomSelect.ini", "wb");
	WriteINI(pFile);
	fclose(pFile);
	if (scChara)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scChara);
	if (scIcon)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scIcon);
	if (scTextArea)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scTextArea);
	if (scLRMove)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scLRMove);
	if (scDeco)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scDeco);
}
HOOK(void, __fastcall, CHudResultStart, 0x010B6840, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	IsInMenuExit = false;
	IsInMenu = false;
	MenuOption = UIPartShoes;
	ItemOption = 0;
	FILE* pFile = fopen("CustomSelect.ini", "wb");
	WriteINI(pFile);
	fclose(pFile);
	if (scChara)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scChara);
	if (scIcon)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scIcon);
	if (scTextArea)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scTextArea);
	if (scLRMove)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scLRMove);
	if (scDeco)
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scDeco);
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

//Install UI
void InstallCustomUI::applyPatches()
{
	INIReader reader("Config.ini");
	ConfigDecoEnable = reader.GetBoolean("Mod", "ConfigDecoEnable", ConfigDecoEnable);
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
}