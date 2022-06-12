#include <Common.h>
#include "UpdateDirector.h"

//Sonic setup
enum SelectShoeType
{
	ShDefault			= 00, 
	ShClassic			= 01, //TBA
	ShSA1LightSpd		= 10, 
	ShSoaps				= 20, 
	ShSoapsLightSpd		= 21, 
	ShRiders			= 30, //TBA
	Sh06Gem				= 40, //TBA
	ShSUAirboost		= 50, //TBA
	ShSULightSpd		= 51, //TBA
	ShSUStomp			= 52, //TBA
	ShSUWallJmp			= 53, //TBA
	ShSUWerehog			= 54, //TBA
	ShSUZoney			= 55, //TBA
	ShLinkBoots			= 60, //TBA
	ShBoom				= 70, //TBA
	ShPumas				= 80,
};
SelectShoeType SelectShoes = SelectShoeType::ShDefault;
enum SelectBodyType
{
	BdDefault			= 00,
	BdSA2RaceSuit		= 10, //TBA
	BdWildFire			= 20,
	BdLinkCos			= 30, //TBA
	BdScarf				= 40,
	BdSwimVest			= 50, //TBA
	BdSurfShirt			= 60, //TBA
	BdHorseShirt		= 70, //TBA
	BdMovieSkin			= 80, //TBA
};
SelectBodyType SelectBody = SelectBodyType::BdDefault;
enum SelectHeadType
{
	HeDefault			= 00,
	HeSA1Sunglass		= 10,
	HeRiders			= 20,
	HeHeadphones		= 30, //TBA
	HeLinkHat			= 40, //TBA
};
SelectHeadType SelectHead = SelectHeadType::HeDefault;
enum SelectHandLType
{
	HLDefault			= 00,
	HLCrystalRing		= 10,
	HLFlameRing			= 20, //TBA
	HLRiders			= 30, //TBA
	HL06Bounce			= 40, //TBA
	HLChipBracelet		= 50, //TBA
	HLLinkGlove			= 60, //TBA
};
SelectHandLType SelectHandL = SelectHandLType::HLDefault;
enum SelectHandRType
{
	HRDefault			= 00,
	HRBounceBracelet	= 10, //TBA
	HRMagicHands		= 11, //TBA
	HRRiders			= 20, //TBA
	HRArkCosmos			= 30, //TBA
	HR06AntiGrav		= 40, //TBA
	HRSecretRing		= 50, //TBA
	HRGauntlet			= 60, //TBA
	HRLinkGlove			= 70, //TBA
};
SelectHandRType SelectHandR = SelectHandRType::HRDefault;

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
	UIPartBody  = 1,
	UIPartHead  = 2,
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
bool ShSoapVariant = false;
enum ShSUVariantType
{
	SUAirboost	= 0,
	SULightSpd	= 1,
	SUStomp		= 2,
	SUWallJmp	= 3,
	SUWerehog	= 4,
	SUZoney		= 5,
};
ShSUVariantType ShSUVariant = ShSUVariantType::SUAirboost;
bool HRMagicVariant = false;
bool* const ENABLE_BLUR = (bool*)0x1A43103;

//Handle Fire VFX
SharedPtrTypeless WildfireVfxHandle;
void SpawnFireParticle(Sonic::Player::CPlayer* player)
{
	auto ChestNode = player->m_spCharacterModel->GetNode("Spine1"); //Set up Chest bone matrix for VFX
	Eigen::Affine3f affine;
	affine = ChestNode->m_WorldMatrix;

	if (!WildfireVfxHandle)
		Common::fCGlitterCreate(player->m_spContext.get(), WildfireVfxHandle, &ChestNode, "ef_ch_sng_wildfire", 1);  //Create Fire VFX
}
void KillFireParticle(Sonic::Player::CPlayer* player)
{
	if (!WildfireVfxHandle)
		return;

	Common::fCGlitterEnd(player->m_spContext.get(), WildfireVfxHandle, true);
	WildfireVfxHandle = nullptr;
}
HOOK(void, __fastcall, MsgRestartStage, 0xE76810, Sonic::Player::CPlayer* This, void* Edx, hh::fnd::Message& message)
{
	KillFireParticle(This);
	return originalMsgRestartStage(This, Edx, message);
}
void __fastcall CSonicRemoveCallback(Sonic::Player::CPlayer* This, void* Edx, void* A1)
{
	IsInMenuExit = false;
	IsInMenu = false;
	MenuOption = UIPartShoes;
	ItemOption = 0;
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
	KillFireParticle(This);
}

//Handle Sonic/UI
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto model = Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);

	//Handle Mesh visibility
	for (size_t i = 0; i < model->m_NodeGroupModels.size(); i++) 
	{
		auto node = model->m_NodeGroupModels[i];
		auto nodeName = node->m_Name;

		////Shoes////
		//Default Shoes
		if (nodeName == "Sonic_100_Shoes")
			node->m_Visible = (SelectShoes == ShDefault);
		//SA1 Lightspeed Shoes
		if (nodeName == "Sonic_110_SA1LightSpd")
			node->m_Visible = (SelectShoes == ShSA1LightSpd);
		//Soap Shoes
		if (nodeName == "Sonic_120_Soaps")
			node->m_Visible = ((SelectShoes == ShSoaps) || (SelectShoes == ShSoapsLightSpd));
		//Soaps Lightspeed Attachment
		if (nodeName == "Sonic_121_SoapLightSpd")
			node->m_Visible = (SelectShoes == ShSoapsLightSpd);
		//Gem Shoes
		if (nodeName == "Sonic_140_Gems")
			node->m_Visible = (SelectShoes == Sh06Gem);
		//Puma Shoes
		if (nodeName == "Sonic_180_Pumas")
			node->m_Visible = (SelectShoes == ShPumas);

		////Bodies////
		//Scarf
		if (nodeName == "Sonic_240_Scarf")
			node->m_Visible = (SelectBody == BdScarf);

		////Heads////
		//SA1 Sunglasses
		if (nodeName == "Sonic_310_SA1SunGlass")
			node->m_Visible = (SelectHead == HeSA1Sunglass);
		//Riders Glasses
		if (nodeName == "Sonic_320_RidersGlass")
			node->m_Visible = (SelectHead == HeRiders);

		////Left Hands////
		//Default Glove
		if (nodeName == "Sonic_400_Glove_L")
			node->m_Visible = ((SelectHandL != HLRiders) && (SelectHandL != HLLinkGlove));
		//Crystal Ring
		if (nodeName == "Sonic_410_CrystalRing")
			node->m_Visible = (SelectHandL == HLCrystalRing);

		////Right Hands////
		//Default Glove
		if (nodeName == "Sonic_500_Glove_R")
			node->m_Visible = ((SelectHandR != HRRiders) && (SelectHandR != HRLinkGlove) && (SelectHandR != HRGauntlet) && (SelectHandR != HRArkCosmos));

		//printf("%s\n", nodeName.c_str());
	}
	
	if (SelectBody == BdWildFire)
		SpawnFireParticle(This);
	if (SelectBody != BdWildFire)
		KillFireParticle(This);

	//if (PressedY)
	//{
	//	//model->m_NodeGroupModels[1]->m_Visible = 0;
	//	//MessageBoxA(nullptr, "You Pressed Y", "Success", MB_OK);
	//	if (ShoeType < 3)
	//		ShoeType++;
	//	else
	//		ShoeType = 0;
	//}

	if (IsInMenu)
		return;

	//printf("Shoe Type: ", "%d", ShoeType, " ");
	//printf("Headwear Type: ", "%d", HeadType, " ");
	//printf("Hand L Type: ", "%d", HandLType, " ");
	//printf("Hand R Type: ", "%d", HandRType, " ");
	//printf("Scarf: ", "%d", Scarf);
	//printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}
void CreateFittingUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
	auto spCsdProject = wrapper.GetCsdProject("ui_fitting");
	prFittingScreen = spCsdProject->m_rcProject;
	obCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreen, 0.5f, "HUD_Pause", true);
	Sonic::CGameDocument::GetInstance()->AddGameObject(obCustomUI, "main", This);
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
	bool PushedUp = inputPtr->LeftStickVertical >= 0.5f;
	bool PushedDown = inputPtr->LeftStickVertical <= -0.5f;
	bool PushedLeft = inputPtr->LeftStickHorizontal <= -0.5f;
	bool PushedRight = inputPtr->LeftStickHorizontal >= 0.5f;
	auto arX = 1280;
	auto arY = 720;
	auto rowX1 = 0.6885;
	auto rowX2 = 0.7745;
	auto rowX3 = 0.8611;
	auto rowY1 = 0.3990;
	auto rowY2 = 0.5515;
	auto rowY3 = 0.7046;
	FUNCTION_PTR(void, __thiscall, changeState, 0x773250, void* This, boost::shared_ptr<void>&spState, const Hedgehog::Base::CSharedString name);
	boost::shared_ptr<void> spState;

	if (PressedSL && !IsInMenu)
	{
		prevblur = *ENABLE_BLUR;
		*ENABLE_BLUR = false;
		MenuOption = UIPartShoes;
		ItemOption = 0;
		if (!prFittingScreen) //Create UI project if it doesn't exist
		{
			CreateFittingUI(This, Edx, in_rUpdateInfo);
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
	}

	if (IsInMenu && scIcon && scChara)
	{
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

		if (scIcon && scChara && scTextArea && scLRMove && PressedA && !IsInMenuExit && !IsInMenuChange)
		{
			switch (MenuOption)
			{
			case UIPartShoes:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectShoes = ShDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectShoes = ShSA1LightSpd;
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					if (!ShSoapVariant)
						SelectShoes = ShSoaps;
					else
						SelectShoes = ShSoapsLightSpd;
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectShoes = Sh06Gem;
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 6:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 7:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 8:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectShoes = ShPumas;
					break;
				}
				break;
			case UIPartBody:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectBody = BdDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectBody = BdWildFire;
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectBody = BdScarf;
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
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
				break;
			case UIPartHead:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHead = HeDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHead = HeSA1Sunglass;
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHead = HeRiders;
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
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
				break;
			case UIPartHandL:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHandL = HLDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHandL = HLCrystalRing;
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
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
				break;
			case UIPartHandR:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHandR = HRDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
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
				break;
			}
		}

		if (scIcon && scChara && scTextArea && scLRMove && PressedB && !IsInMenuExit && !IsInMenuChange)
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
		}

		if (scIcon && scChara && scTextArea && scLRMove && PressedY && !IsInMenuExit && !IsInMenuChange)
		{
			if (MenuOption == UIPartShoes)
			{
				switch (ItemOption)
				{
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					scIcon->SetMotion("ON_Anim");
					scIcon->SetMotionFrame(0.0f);
					scIcon->m_MotionDisableFlag = false;
					scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
					scIcon->m_MotionSpeed = 1.0f;
					scIcon->Update();
					if (!ShSoapVariant)
					{
						ShSoapVariant = true;
						if (SelectShoes == ShSoaps)
							SelectShoes = ShSoapsLightSpd;
					}
					else
					{
						ShSoapVariant = false;
						if (SelectShoes == ShSoapsLightSpd)
							SelectShoes = ShSoaps;
					}
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					scIcon->SetMotion("ON_Anim");
					scIcon->SetMotionFrame(0.0f);
					scIcon->m_MotionDisableFlag = false;
					scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
					scIcon->m_MotionSpeed = 1.0f;
					scIcon->Update();
					switch (ShSUVariant)
					{
					case SUAirboost:
						ShSUVariant = SULightSpd;
						break;
					case SULightSpd:
						ShSUVariant = SUStomp;
						break;
					case SUStomp:
						ShSUVariant = SUWallJmp;
						break;
					case SUWallJmp:
						ShSUVariant = SUWerehog;
						break;
					case SUWerehog:
						ShSUVariant = SUZoney;
						break;
					case SUZoney:
						ShSUVariant = SUAirboost;
						break;
					}
					break;
				}
			}
			if (MenuOption == UIPartHandR)
			{
				switch (ItemOption)
				{
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					scIcon->SetMotion("ON_Anim");
					scIcon->SetMotionFrame(0.0f);
					scIcon->m_MotionDisableFlag = false;
					scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
					scIcon->m_MotionSpeed = 1.0f;
					scIcon->Update();
					if (!HRMagicVariant)
					{
						HRMagicVariant = true;
						if (SelectHandR == HRBounceBracelet)
							SelectHandR = HRMagicHands;
					}
					else
					{
						HRMagicVariant = false;
						if (SelectHandR == HRMagicHands)
							SelectHandR = HRBounceBracelet;
					}
					break;
				}
			}
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedRB || PressedLB) && !IsInMenuExit)
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
		}

		if (scIcon && scChara && scTextArea && scLRMove && IsInMenuExit && scChara->m_MotionFrame <= 0 && !IsInMenuChange)
		{
			IsInMenuExit = false;
			IsInMenu = false;
			MenuOption = UIPartShoes;
			ItemOption = 0;
			*ENABLE_BLUR = prevblur;
			Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scChara);
			Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scIcon);
			Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scTextArea);
			Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scLRMove);
			if (scDeco)
				Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scDeco);
		}

		if (scIcon && scChara && scTextArea && scLRMove && IsInMenuChange && scChara->m_MotionFrame <= 0 && !IsInMenuExit)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
			scTextArea->SetMotion("Intro_Anim");
			scTextArea->SetMotionFrame(0.0f);
			scTextArea->m_MotionDisableFlag = false;
			scTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scTextArea->m_MotionSpeed = 1.0f;
			scTextArea->Update();
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedUp || (PushedUp && scIcon->m_MotionFrame >= 10)) && !IsInMenuExit && !IsInMenuChange)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedDown || (PushedDown && scIcon->m_MotionFrame >= 10)) && !IsInMenuExit && !IsInMenuChange)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedLeft || (PushedLeft && scIcon->m_MotionFrame >= 10)) && !IsInMenuExit && !IsInMenuChange)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedRight || (PushedRight && scIcon->m_MotionFrame >= 10)) && !IsInMenuExit && !IsInMenuChange)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
		}

		////Icon Handlers////
		if (scIcon && scChara)
		{
			switch (MenuOption)
			{
			case UIPartShoes: ////Shoe Icons////
				//Default Shoes
				if (SelectShoes == ShDefault)
					scChara->GetNode("img_ch_0")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_0")->SetPatternIndex(1);
				//SA1 Lightspeed Shoes
				if (SelectShoes == ShSA1LightSpd)
					scChara->GetNode("img_ch_1")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_1")->SetPatternIndex(1);
				//Soap Shoes
				if (!ShSoapVariant)
				{
					if (SelectShoes == ShSoaps)
						scChara->GetNode("img_ch_2")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_2")->SetPatternIndex(1);
				}
				else
				{
					if (SelectShoes == ShSoapsLightSpd)
						scChara->GetNode("img_ch_2")->SetPatternIndex(12);
					else
						scChara->GetNode("img_ch_2")->SetPatternIndex(11);
				}
				//Riders Shoes
				if (SelectShoes == Sh06Gem)
					scChara->GetNode("img_ch_3")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_3")->SetPatternIndex(1);
				//Gem Shoes
				if (SelectShoes == Sh06Gem)
					scChara->GetNode("img_ch_4")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_4")->SetPatternIndex(1);
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
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				case SUStomp:
					if (SelectShoes == ShSUStomp)
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				case SUWallJmp:
					if (SelectShoes == ShSUWallJmp)
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				case SUWerehog:
					if (SelectShoes == ShSUWerehog)
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				case SUZoney:
					if (SelectShoes == ShSUZoney)
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				}
				//Link Boots
				if (SelectShoes == ShPumas)
					scChara->GetNode("img_ch_6")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_6")->SetPatternIndex(1);
				//Boom Shoes
				if (SelectShoes == ShPumas)
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
				if (SelectBody == BdDefault)
					scChara->GetNode("img_ch_0")->SetPatternIndex(4);
				else
					scChara->GetNode("img_ch_0")->SetPatternIndex(3);
				//Wildfire
				if (SelectBody == BdWildFire)
					scChara->GetNode("img_ch_2")->SetPatternIndex(4);
				else
					scChara->GetNode("img_ch_2")->SetPatternIndex(3);
				//Scarf
				if (SelectBody == BdScarf)
					scChara->GetNode("img_ch_4")->SetPatternIndex(4);
				else
					scChara->GetNode("img_ch_4")->SetPatternIndex(3);
				//Empty Placeholders
				scChara->GetNode("img_ch_1")->SetPatternIndex(3);
				scChara->GetNode("img_ch_3")->SetPatternIndex(3);
				scChara->GetNode("img_ch_5")->SetPatternIndex(3);
				scChara->GetNode("img_ch_6")->SetPatternIndex(3);
				scChara->GetNode("img_ch_7")->SetPatternIndex(3);
				scChara->GetNode("img_ch_8")->SetPatternIndex(3);
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
				//Empty Placeholders
				scChara->GetNode("img_ch_3")->SetPatternIndex(5);
				scChara->GetNode("img_ch_4")->SetPatternIndex(5);
				scChara->GetNode("img_ch_5")->SetPatternIndex(5);
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
				//Empty Placeholders
				scChara->GetNode("img_ch_2")->SetPatternIndex(7);
				scChara->GetNode("img_ch_3")->SetPatternIndex(7);
				scChara->GetNode("img_ch_4")->SetPatternIndex(7);
				scChara->GetNode("img_ch_5")->SetPatternIndex(7);
				scChara->GetNode("img_ch_6")->SetPatternIndex(7);
				scChara->GetNode("img_ch_7")->SetPatternIndex(7);
				scChara->GetNode("img_ch_8")->SetPatternIndex(7);
				scTextArea->GetNode("textarea_textbox")->SetPatternIndex(3);
				break;
			case UIPartHandR: ////Hand R Icons////
				//Default Right Glove
				if (SelectHandR == HRDefault)
					scChara->GetNode("img_ch_0")->SetPatternIndex(10);
				else
					scChara->GetNode("img_ch_0")->SetPatternIndex(9);
				//Empty Placeholders
				scChara->GetNode("img_ch_1")->SetPatternIndex(9);
				scChara->GetNode("img_ch_2")->SetPatternIndex(9);
				scChara->GetNode("img_ch_3")->SetPatternIndex(9);
				scChara->GetNode("img_ch_4")->SetPatternIndex(9);
				scChara->GetNode("img_ch_5")->SetPatternIndex(9);
				scChara->GetNode("img_ch_6")->SetPatternIndex(9);
				scChara->GetNode("img_ch_7")->SetPatternIndex(9);
				scChara->GetNode("img_ch_8")->SetPatternIndex(9);
				scTextArea->GetNode("textarea_textbox")->SetPatternIndex(4);
				break;
			}
		}

		//printf("%f", scChara->m_MotionFrame);
	}

}
HOOK(void, __fastcall, CHudSonicStageUpdate, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	CHudFittingMenu(This, Edx, in_rUpdateInfo);
	/*
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
	auto rowX1 = 0.6885;
	auto rowX2 = 0.7745;
	auto rowX3 = 0.8611;
	auto rowY1 = 0.3990;
	auto rowY2 = 0.5515;
	auto rowY3 = 0.7046;
	FUNCTION_PTR(void, __thiscall, changeState, 0x773250, void* This, boost::shared_ptr<void>&spState, const Hedgehog::Base::CSharedString name);
	boost::shared_ptr<void> spState;

	if (PressedSL && !IsInMenu)
	{
		MenuOption = UIPartShoes;
		ItemOption = 0;
		if (!prFittingScreen) //Create UI project if it doesn't exist
		{
			CreateFittingUI(This, Edx, in_rUpdateInfo);
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
		scIcon->SetPosition(rowX1*arX, rowY1*arY);
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
	}

	if (IsInMenu && scIcon && scChara)
	{
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

		if (scIcon && scChara && scTextArea && scLRMove && PressedA && !IsInMenuExit && !IsInMenuChange)
		{
			switch (MenuOption)
			{
			case UIPartShoes:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectShoes = ShDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectShoes = ShSA1LightSpd;
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					if (!ShSoapVariant)
						SelectShoes = ShSoaps;
					else
						SelectShoes = ShSoapsLightSpd;
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectShoes = Sh06Gem;
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 6:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 7:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 8:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectShoes = ShPumas;
					break;
				}
				break;
			case UIPartBody:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectBody = BdDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectBody = BdWildFire;
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectBody = BdScarf;
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
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
				break;
			case UIPartHead:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHead = HeDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHead = HeSA1Sunglass;
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHead = HeRiders;
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
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
				break;
			case UIPartHandL:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHandL = HLDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHandL = HLCrystalRing;
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
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
				break;
			case UIPartHandR:
				switch (ItemOption)
				{
				case 0:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					SelectHandR = HRDefault;
					break;
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 3:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 4:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000007);
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
				break;
			}
		}

		if (scIcon && scChara && scTextArea && scLRMove && PressedB && !IsInMenuExit && !IsInMenuChange)
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
		}

		if (scIcon && scChara && scTextArea && scLRMove && PressedY && !IsInMenuExit && !IsInMenuChange)
		{
			if (MenuOption == UIPartShoes)
			{
				switch (ItemOption)
				{
				case 2:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					scIcon->SetMotion("ON_Anim");
					scIcon->SetMotionFrame(0.0f);
					scIcon->m_MotionDisableFlag = false;
					scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
					scIcon->m_MotionSpeed = 1.0f;
					scIcon->Update();
					if (!ShSoapVariant)
					{
						ShSoapVariant = true;
						if (SelectShoes == ShSoaps)
							SelectShoes = ShSoapsLightSpd;
					}
					else
					{
						ShSoapVariant = false;
						if (SelectShoes == ShSoapsLightSpd)
							SelectShoes = ShSoaps;
					}
					break;
				case 5:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					scIcon->SetMotion("ON_Anim");
					scIcon->SetMotionFrame(0.0f);
					scIcon->m_MotionDisableFlag = false;
					scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
					scIcon->m_MotionSpeed = 1.0f;
					scIcon->Update();
					switch (ShSUVariant)
					{
					case SUAirboost:
						ShSUVariant = SULightSpd;
						break;
					case SULightSpd:
						ShSUVariant = SUStomp;
						break;
					case SUStomp:
						ShSUVariant = SUWallJmp;
						break;
					case SUWallJmp:
						ShSUVariant = SUWerehog;
						break;
					case SUWerehog:
						ShSUVariant = SUZoney;
						break;
					case SUZoney:
						ShSUVariant = SUAirboost;
						break;
					}
					break;
				}
			}
			if (MenuOption == UIPartHandR)
			{
				switch (ItemOption)
				{
				case 1:
					Common::PlaySoundStatic(menuSoundHandle, 1000005);
					scIcon->SetMotion("ON_Anim");
					scIcon->SetMotionFrame(0.0f);
					scIcon->m_MotionDisableFlag = false;
					scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
					scIcon->m_MotionSpeed = 1.0f;
					scIcon->Update();
					if (!HRMagicVariant)
					{
						HRMagicVariant = true;
						if (SelectHandR == HRBounceBracelet)
							SelectHandR = HRMagicHands;
					}
					else
					{
						HRMagicVariant = false;
						if (SelectHandR == HRMagicHands)
							SelectHandR = HRBounceBracelet;
					}
					break;
				}
			}
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedRB || PressedLB) && !IsInMenuExit)
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
		}

		if (scIcon && scChara && scTextArea && scLRMove && IsInMenuExit && scChara->m_MotionFrame <= 0 && !IsInMenuChange)
		{
			IsInMenuExit = false;
			IsInMenu = false;
			MenuOption = UIPartShoes;
			ItemOption = 0;
			Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scChara);
			Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scIcon);
			Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scTextArea);
			Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scLRMove);
			if (scDeco)
				Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scDeco);
		}

		if (scIcon && scChara && scTextArea && scLRMove && IsInMenuChange && scChara->m_MotionFrame <= 0 && !IsInMenuExit)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
			scTextArea->SetMotion("Intro_Anim");
			scTextArea->SetMotionFrame(0.0f);
			scTextArea->m_MotionDisableFlag = false;
			scTextArea->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scTextArea->m_MotionSpeed = 1.0f;
			scTextArea->Update();
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedUp || (PushedUp && scIcon->m_MotionFrame >= 10)) && !IsInMenuExit && !IsInMenuChange)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedDown || (PushedDown && scIcon->m_MotionFrame >= 10)) && !IsInMenuExit && !IsInMenuChange)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedLeft || (PushedLeft && scIcon->m_MotionFrame >= 10)) && !IsInMenuExit && !IsInMenuChange)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
		}

		if (scIcon && scChara && scTextArea && scLRMove && (PressedRight || (PushedRight && scIcon->m_MotionFrame >= 10)) && !IsInMenuExit && !IsInMenuChange)
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
			scIcon->SetMotion("ON_Anim");
			scIcon->SetMotionFrame(0.0f);
			scIcon->m_MotionDisableFlag = false;
			scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			scIcon->m_MotionSpeed = 1.0f;
			scIcon->Update();
		}

		////Icon Handlers////
		if (scIcon && scChara)
		{
			switch (MenuOption)
			{
			case UIPartShoes: ////Shoe Icons////
				//Default Shoes
				if (SelectShoes == ShDefault)
					scChara->GetNode("img_ch_0")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_0")->SetPatternIndex(1);
				//SA1 Lightspeed Shoes
				if (SelectShoes == ShSA1LightSpd)
					scChara->GetNode("img_ch_1")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_1")->SetPatternIndex(1);
				//Soap Shoes
				if (!ShSoapVariant)
				{
					if (SelectShoes == ShSoaps)
						scChara->GetNode("img_ch_2")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_2")->SetPatternIndex(1);
				}
				else
				{
					if (SelectShoes == ShSoapsLightSpd)
						scChara->GetNode("img_ch_2")->SetPatternIndex(12);
					else
						scChara->GetNode("img_ch_2")->SetPatternIndex(11);
				}
				//Riders Shoes
				if (SelectShoes == Sh06Gem)
					scChara->GetNode("img_ch_3")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_3")->SetPatternIndex(1);
				//Gem Shoes
				if (SelectShoes == Sh06Gem)
					scChara->GetNode("img_ch_4")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_4")->SetPatternIndex(1);
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
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				case SUStomp:
					if (SelectShoes == ShSUStomp)
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				case SUWallJmp:
					if (SelectShoes == ShSUWallJmp)
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				case SUWerehog:
					if (SelectShoes == ShSUWerehog)
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				case SUZoney:
					if (SelectShoes == ShSUZoney)
						scChara->GetNode("img_ch_5")->SetPatternIndex(2);
					else
						scChara->GetNode("img_ch_5")->SetPatternIndex(1);
					break;
				}
				//Link Boots
				if (SelectShoes == ShPumas)
					scChara->GetNode("img_ch_6")->SetPatternIndex(2);
				else
					scChara->GetNode("img_ch_6")->SetPatternIndex(1);
				//Boom Shoes
				if (SelectShoes == ShPumas)
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
				if (SelectBody == BdDefault)
					scChara->GetNode("img_ch_0")->SetPatternIndex(4);
				else
					scChara->GetNode("img_ch_0")->SetPatternIndex(3);
				//Wildfire
				if (SelectBody == BdWildFire)
					scChara->GetNode("img_ch_2")->SetPatternIndex(4);
				else
					scChara->GetNode("img_ch_2")->SetPatternIndex(3);
				//Scarf
				if (SelectBody == BdScarf)
					scChara->GetNode("img_ch_4")->SetPatternIndex(4);
				else
					scChara->GetNode("img_ch_4")->SetPatternIndex(3);
				//Empty Placeholders
				scChara->GetNode("img_ch_1")->SetPatternIndex(3);
				scChara->GetNode("img_ch_3")->SetPatternIndex(3);
				scChara->GetNode("img_ch_5")->SetPatternIndex(3);
				scChara->GetNode("img_ch_6")->SetPatternIndex(3);
				scChara->GetNode("img_ch_7")->SetPatternIndex(3);
				scChara->GetNode("img_ch_8")->SetPatternIndex(3);
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
				//Empty Placeholders
				scChara->GetNode("img_ch_3")->SetPatternIndex(5);
				scChara->GetNode("img_ch_4")->SetPatternIndex(5);
				scChara->GetNode("img_ch_5")->SetPatternIndex(5);
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
				//Empty Placeholders
				scChara->GetNode("img_ch_2")->SetPatternIndex(7);
				scChara->GetNode("img_ch_3")->SetPatternIndex(7);
				scChara->GetNode("img_ch_4")->SetPatternIndex(7);
				scChara->GetNode("img_ch_5")->SetPatternIndex(7);
				scChara->GetNode("img_ch_6")->SetPatternIndex(7);
				scChara->GetNode("img_ch_7")->SetPatternIndex(7);
				scChara->GetNode("img_ch_8")->SetPatternIndex(7);
				scTextArea->GetNode("textarea_textbox")->SetPatternIndex(3);
				break;
			case UIPartHandR: ////Hand R Icons////
				//Default Right Glove
				if (SelectHandR == HRDefault)
					scChara->GetNode("img_ch_0")->SetPatternIndex(10);
				else
					scChara->GetNode("img_ch_0")->SetPatternIndex(9);
				//Empty Placeholders
				scChara->GetNode("img_ch_1")->SetPatternIndex(9);
				scChara->GetNode("img_ch_2")->SetPatternIndex(9);
				scChara->GetNode("img_ch_3")->SetPatternIndex(9);
				scChara->GetNode("img_ch_4")->SetPatternIndex(9);
				scChara->GetNode("img_ch_5")->SetPatternIndex(9);
				scChara->GetNode("img_ch_6")->SetPatternIndex(9);
				scChara->GetNode("img_ch_7")->SetPatternIndex(9);
				scChara->GetNode("img_ch_8")->SetPatternIndex(9);
				scTextArea->GetNode("textarea_textbox")->SetPatternIndex(4);
				break;
			}
		}

		//printf("%f", scChara->m_MotionFrame);
	}
	*/
	//printf("Gameplay HUD\n");
	originalCHudSonicStageUpdate(This, Edx, in_rUpdateInfo);
}
HOOK(void, __fastcall, CHudPlayableMenuUpdate, 0x0108D510, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	CHudFittingMenu(This, Edx, in_rUpdateInfo);
	//printf("Hub world HUD\n");
	originalCHudPlayableMenuUpdate(This, Edx, in_rUpdateInfo);
}
HOOK(void, __fastcall, CHUDPauseUpdate, 0x0042A520, hh::fnd::CStateMachineBase::CStateBase* This)
{
	IsInMenuExit = false;
	IsInMenu = false;
	MenuOption = UIPartShoes;
	ItemOption = 0;
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
	addUpdateCommand(context, elapsedTime, "9");
	
	callUnknownFunction(context, elapsedTime);

	addRenderCommand(context, elapsedTime, "1");
	//addRenderCommand(context, elapsedTime, "2");
	addRenderCommand(context, elapsedTime, "3");
	addRenderCommand(context, elapsedTime, "b");
	//addRenderCommand(context, elapsedTime, "e");
	addRenderCommand(context, elapsedTime, "9");

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

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationParams, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CParameterCategory* cat_Bounce = parameterGroup->CreateParameterCategory("Customizable Sonic", "Parameters for customizable Sonic");

	//cat_Bounce->CreateParamInt(&someInt, "Integer");
	//cat_Bounce->CreateParamBool(&someBool, "Bool");
	//cat_Bounce->CreateParamFloat(&someFloat, "Float");

	cat_Bounce->CreateParamTypeList((uint32_t*)&SelectShoes, "Shoe Selection", "Select what Sonic should wear.",
		{
			{ "Default Shoes", ShDefault},
			{ "Classic Shoes", ShClassic},
			{ "Lightspeed Shoes", ShSA1LightSpd},
			{ "Soap Shoes", ShSoaps},
			{ "Soaps + Lightspeed", ShSoapsLightSpd},
			{ "Riders Shoes", ShRiders},
			{ "Gem Shoes", Sh06Gem},
			{ "Airboost Shoes", ShSUAirboost},
			{ "Lightspeed Dash Shoes", ShSULightSpd},
			{ "Stomp Shoes", ShSUStomp},
			{ "Wall Jump Shoes", ShSUWallJmp},
			{ "Werehog Shoes", ShSUWerehog},
			{ "Thicc Soles", ShSUZoney},
			{ "Link Boots", ShLinkBoots},
			{ "Boom Shoes", ShBoom},
			{ "Puma Shoes", ShPumas},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&SelectBody, "Body Selection", "Select what Sonic should wear.",
		{
			{ "Default Body", BdDefault},
			{ "SA2 Racing Suit", BdSA2RaceSuit},
			{ "WildFire", BdWildFire},
			{ "Link Tunic", BdLinkCos},
			{ "Scarf", BdScarf},
			{ "Surf Shirt", BdSurfShirt},
			{ "Swim Vest", BdSwimVest},
			{ "Horse Shirt", BdHorseShirt},
			{ "Movie Quills", BdMovieSkin},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&SelectHead, "Head Selection", "Select what Sonic should wear.",
		{
			{ "Default Head", HeDefault},
			{ "SA1 Sunlasses", HeSA1Sunglass},
			{ "Riders Glasses", HeRiders},
			{ "Headphones", HeHeadphones},
			{ "Link Hat", HeLinkHat},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&SelectHandL, "Hand Left Selection", "Select what Sonic should wear.",
		{
			{ "Default Glove", HLDefault},
			{ "Crystal Ring", HLCrystalRing},
			{ "Flame Ring", HLFlameRing},
			{ "Riders Glove", HLRiders},
			{ "Bounce Bracelet '06", HL06Bounce},
			{ "Chip's Bracelet", HLChipBracelet},
			{ "Link Glove", HLLinkGlove},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&SelectHandR, "Hand Right Selection", "Select what Sonic should wear.",
		{
			{ "Default Glove", HRDefault},
			{ "Bounce Bracelet", HRBounceBracelet},
			{ "Magic Hands", HRMagicHands},
			{ "Riders Glove", HRRiders},
			{ "Ark Of The Cosmos", HRArkCosmos},
			{ "Antigravity Ring", HR06AntiGrav},
			{ "Shara's Ring", HRSecretRing},
			{ "Gauntlet", HRGauntlet},
			{ "Link Glove", HRLinkGlove},
		});
	//cat_Bounce->CreateParamBool(&Scarf, "Choose if Sonic should wear his scarf.");

	parameterGroup->Flush();

	originalInitializeApplicationParams(This);
}

//Handle Files
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>SonicSp</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Title</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev031</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev041</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev042</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev091</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicWildFire</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam_cmn</Name>\n";
		stream << "    <Archive>SystemCommon</Archive>\n";
		stream << "  </DefAppend>\n";

		str = stream.str();
	}

	const size_t newSize = size + str.size();
	const std::unique_ptr<char[]> buffer = std::make_unique<char[]>(newSize);
	memcpy(buffer.get(), data, size);

	char* insertionPos = strstr(buffer.get(), "<Include>");

	memmove(insertionPos + str.size(), insertionPos, size - (size_t)(insertionPos - buffer.get()));
	memcpy(insertionPos, str.c_str(), str.size());

	bool result;
	{
		result = originalParseArchiveTree(A1, buffer.get(), newSize, database);
	}

	return result;
}
HOOK(void, __fastcall, LoadArchive, 0x69AB10, void* This, void* Edx, void* A3, void* A4, const hh::base::CSharedString& name, void* archiveInfo, void* A7, void* A8)
{
	if (strstr(name.c_str(), "evSonic") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchive(This, Edx, A3, A4, name, archiveInfo, A7, A8);
}
HOOK(void, __fastcall, LoadArchiveList, 0x69C270, void* This, void* Edx, void* A3, void* A4, const hh::base::CSharedString& name, void* archiveInfo)
{
	if (strstr(name.c_str(), "evSonic") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchiveList(This, Edx, A3, A4, name, archiveInfo);
}

EXPORT void Init()
{
	//INIReader reader("Mod.ini");
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(CHudSonicStageUpdate);
	INSTALL_HOOK(CHudPlayableMenuUpdate);
	INSTALL_HOOK(CHUDPauseUpdate);
	INSTALL_HOOK(InitializeApplicationParams);
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(LoadArchive);
	INSTALL_HOOK(LoadArchiveList);
	INSTALL_HOOK(MsgRestartStage);
	INSTALL_HOOK(UpdateDirectorSimple);
	INSTALL_HOOK(UpdateDirectorNormal);
	INSTALL_HOOK(CPlayerAddCallback);
	WRITE_MEMORY(0x16D4B4C, void*, CSonicRemoveCallback);
	//WRITE_NOP(0x0042AB50, 5); //idk i forgor
	//WRITE_JUMP(0x42A82C, (void*)0x42A9E5); //Jumps over entire state if
	//WRITE_JUMP(0x42A83D, (void*)0x42A843); //Jumps over stage switch
}