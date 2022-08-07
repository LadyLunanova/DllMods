#include <Common.h>
#include "UpdateDirector.h"

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

//Sonic setup
std::mutex mutex;
std::map<boost::shared_ptr<hh::mr::CModelData>, std::vector<boost::shared_ptr<hh::mr::CNodeGroupModelData>>> models;
enum SelectShoeType
{
	ShDefault = 00,
	ShClassic = 01,
	ShArchie = 02,
	ShSA1LightSpd = 10,
	ShSoaps = 20,
	ShSoapsLightSpd = 21,
	ShRiders = 30,
	Sh06GemDefault = 40,
	Sh06GemRed = 41,
	Sh06GemBlue = 42,
	Sh06GemGreen = 43,
	Sh06GemPurple = 44,
	Sh06GemSky = 45,
	Sh06GemWhite = 46,
	Sh06GemYellow = 47,
	ShSUAirboost = 50,
	ShSULightSpd = 51, //TBA
	ShSUStomp = 52, //TBA
	ShSUWallJmp = 53, //TBA
	ShSUWerehog = 54, //TBA
	ShSUZoney = 55, //TBA
	ShLinkBoots = 60, //TBA
	ShBoom = 70, //TBA
	ShPumas = 80,
};
SelectShoeType SelectShoes = SelectShoeType::ShDefault;
enum SelectBodyType
{
	BdDefault = 00,
	BdSA2RaceSuit = 10, //TBA
	BdWildFire = 20,
	BdLinkCos = 30, //TBA
	BdScarf = 40,
	BdSwimVest = 50, //TBA
	BdSurfShirt = 60, //TBA
	BdHorseShirt = 70, //TBA
	BdMovieSkin = 80, //TBA
};
SelectBodyType SelectBody = SelectBodyType::BdDefault;
enum SelectHeadType
{
	HeDefault = 00,
	HeSA1Sunglass = 10,
	HeRiders = 20,
	HeHeadphones = 30, //TBA
	HeLinkHat = 40, //TBA
};
SelectHeadType SelectHead = SelectHeadType::HeDefault;
enum SelectHandLType
{
	HLDefault = 00,
	HLCrystalRing = 10,
	HLFlameRing = 20, //TBA
	HLRiders = 30,
	HL06Bounce = 40, //TBA
	HLChipBracelet = 50, //TBA
	HLLinkGlove = 60, //TBA
};
SelectHandLType SelectHandL = SelectHandLType::HLDefault;
enum SelectHandRType
{
	HRDefault = 00,
	HRBounceBracelet = 10, //TBA
	HRMagicHands = 11, //TBA
	HRRiders = 20,
	HRArkCosmos = 21, //TBA
	HR06AntiGrav = 30, //TBA
	HRSecretRing = 40, //TBA
	HRGauntlet = 50, //TBA
	HRLinkGlove = 60, //TBA
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
enum Sh06VariantType
{
	GemDefault = 0,
	GemRed = 1,
	GemBlue = 2,
	GemGreen = 3,
	GemPurple = 4,
	GemSky = 5,
	GemWhite = 6,
	GemYellow = 7,
};
Sh06VariantType Sh06Variant = Sh06VariantType::GemDefault;
enum ShDefaultVariantType
{
	ShoeDefault = 0,
	ShoeClassic = 1,
	ShoeArchie = 2,
};
ShDefaultVariantType ShDefaultVariant = ShDefaultVariantType::ShoeDefault;
bool HRMagicVariant = false;
bool* const ENABLE_BLUR = (bool*)0x1A43103;
void PlayCursorAnim()
{
	scIcon->SetMotion("ON_Anim");
	scIcon->SetMotionFrame(0.0f);
	scIcon->m_MotionDisableFlag = false;
	scIcon->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	scIcon->m_MotionSpeed = 1.0f;
	scIcon->Update();
}

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
	KillFireParticle(This);
}

//Handle Sonic
void meshVisibilityHandler(const boost::shared_ptr<hh::mr::CModelData>& model, std::vector<boost::shared_ptr<hh::mr::CNodeGroupModelData>>& nodeGroupModels)
{
	if (!model || !(model->m_Flags & hh::db::eDatabaseDataFlags_IsMadeOne))
		return;

	if (nodeGroupModels.empty())
		nodeGroupModels.assign(model->m_NodeGroupModels.begin(), model->m_NodeGroupModels.end());

	model->m_NodeGroupModels.clear();

	//Handle Mesh visibility
	for (auto& node : nodeGroupModels)
	{
		auto nodeName = node->m_Name;

		/////Shoes//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Shoes
		if (nodeName == "Sonic_100_Shoes")
			node->m_Visible = (SelectShoes == ShDefault);
		//Classic Shoes
		if (nodeName == "Sonic_101_Classic")
			node->m_Visible = (SelectShoes == ShClassic);
		//Archie Shoes
		if (nodeName == "Sonic_102_Archie")
			node->m_Visible = (SelectShoes == ShArchie);
		//SA1 Lightspeed Shoes
		if (nodeName == "Sonic_110_SA1LightSpd")
			node->m_Visible = (SelectShoes == ShSA1LightSpd);
		//Soap Shoes
		if (nodeName == "Sonic_120_Soaps")
			node->m_Visible = ((SelectShoes == ShSoaps) || (SelectShoes == ShSoapsLightSpd));
		//Soaps Lightspeed Attachment
		if (nodeName == "Sonic_121_SoapLightSpd")
			node->m_Visible = (SelectShoes == ShSoapsLightSpd);
		//Riders Shoes
		if (nodeName == "Sonic_130_Riders")
			node->m_Visible = (SelectShoes == ShRiders);
		//Gem Shoes
		if (nodeName == "Sonic_140_Gems")
			node->m_Visible = (SelectShoes == Sh06GemDefault);
		if (nodeName == "Sonic_141_Gem_Red")
			node->m_Visible = (SelectShoes == Sh06GemRed);
		if (nodeName == "Sonic_142_Gem_Blue")
			node->m_Visible = (SelectShoes == Sh06GemBlue);
		if (nodeName == "Sonic_143_Gem_Green")
			node->m_Visible = (SelectShoes == Sh06GemGreen);
		if (nodeName == "Sonic_144_Gem_Purple")
			node->m_Visible = (SelectShoes == Sh06GemPurple);
		if (nodeName == "Sonic_145_Gem_Sky")
			node->m_Visible = (SelectShoes == Sh06GemSky);
		if (nodeName == "Sonic_146_Gem_White")
			node->m_Visible = (SelectShoes == Sh06GemWhite);
		if (nodeName == "Sonic_147_Gem_Yellow")
			node->m_Visible = (SelectShoes == Sh06GemYellow);
		//Unleashed Shoes
		if (nodeName == "Sonic_150_SUAirboost")
			node->m_Visible = (SelectShoes == ShSUAirboost);
		if (nodeName == "Sonic_151_SULightSpd")
			node->m_Visible = (SelectShoes == ShSULightSpd);
		if (nodeName == "Sonic_152_SUStomp")
			node->m_Visible = (SelectShoes == ShSUStomp);
		if (nodeName == "Sonic_153_SUWallJmp")
			node->m_Visible = (SelectShoes == ShSUWallJmp);
		if (nodeName == "Sonic_154_SUWerehog")
			node->m_Visible = (SelectShoes == ShSUWerehog);
		if (nodeName == "Sonic_155_SUZoney")
			node->m_Visible = (SelectShoes == ShSUZoney);
		//Puma Shoes
		if (nodeName == "Sonic_180_Pumas")
			node->m_Visible = (SelectShoes == ShPumas);
		/////Bodies//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Base Body
		if (nodeName == "Sonic_000_Body")
			node->m_Visible = ((SelectBody != BdSA2RaceSuit) && (SelectBody != BdMovieSkin));
		//Scarf
		if (nodeName == "Sonic_240_Scarf")
			node->m_Visible = (SelectBody == BdScarf);
		/////Heads/////
		//SA1 Sunglasses
		if (nodeName == "Sonic_310_SA1SunGlass")
			node->m_Visible = (SelectHead == HeSA1Sunglass);
		//Riders Glasses
		if (nodeName == "Sonic_320_RidersGlass")
			node->m_Visible = (SelectHead == HeRiders);
		/////Left Hands//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "Sonic_400_Glove_L")
			node->m_Visible = ((SelectHandL != HLRiders) && (SelectHandL != HLLinkGlove));
		//Crystal Ring
		if (nodeName == "Sonic_410_CrystalRing")
			node->m_Visible = (SelectHandL == HLCrystalRing);
		//Riders Glove
		if (nodeName == "Sonic_430_RiderGlove_L")
			node->m_Visible = (SelectHandL == HLRiders);
		/////Right Hands/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "Sonic_500_Glove_R")
			node->m_Visible = ((SelectHandR != HRRiders) && (SelectHandR != HRLinkGlove) && (SelectHandR != HRGauntlet) && (SelectHandR != HRArkCosmos));
		//Riders Glove
		if (nodeName == "Sonic_530_RiderGlove_R")
			node->m_Visible = (SelectHandR == HRRiders);

		//printf("%s\n", nodeName.c_str());

		if (node->m_Visible)
			model->m_NodeGroupModels.push_back(node);
	}

	model->m_NodeGroupModelNum = model->m_NodeGroupModels.size();
}
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);

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

	//if (IsInMenu)
	//	return;

	printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}
HOOK(void*, __fastcall, CMirageDatabaseWrapperGetModelData, hh::mr::fpCMirageDatabaseWrapperGetModelData, Hedgehog::Mirage::CMirageDatabaseWrapper* This, void* Edx, boost::shared_ptr<Hedgehog::Mirage::CModelData>& out_spModelData, const Hedgehog::Base::CSharedString& in_rName, size_t in_Unknown)
{
	void* result = originalCMirageDatabaseWrapperGetModelData(This, Edx, out_spModelData, in_rName, in_Unknown);

	if (out_spModelData && (strstr(in_rName.c_str(), "chr_Sonic_HD") || strstr(in_rName.c_str(), "chr_SuperSonic_HD")))
	{
		std::lock_guard lock(mutex);

		const auto pair = models.find(out_spModelData);
		if (pair == models.end())
			models[out_spModelData] = {};
	}

	return result;
}

//INI file Handling
void writeINI(FILE* iniFile)
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
void readINI()
{
	INIReader* reader = new INIReader("CustomSelect.ini");
	if (reader->ParseError() != 0)
	{
		FILE* pFile = fopen("CustomSelect.ini", "wb");
		writeINI(pFile);
		fclose(pFile);
		reader = new INIReader("CustomSelect.ini");
	}
	SelectShoes = (SelectShoeType)reader->GetInteger("Select", "SelectShoes", SelectShoes);
	SelectBody = (SelectBodyType)reader->GetInteger("Select", "SelectBody", SelectBody);
	SelectHead = (SelectHeadType)reader->GetInteger("Select", "SelectHead", SelectHead);
	SelectHandL = (SelectHandLType)reader->GetInteger("Select", "SelectHandL", SelectHandL);
	SelectHandR = (SelectHandRType)reader->GetInteger("Select", "SelectHandR", SelectHandR);
}

//Handle UI
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

	if (PressedSL && !IsInMenu && IsModernSonic && !ExternalControl && !Goal && !((getPackedID == pam000) && IsOutOfControl))
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
			readINI();
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
				ShSoapVariant = false;
				Sh06Variant = GemDefault;
				ShSUVariant = SUAirboost;
				break;
			case ShArchie:
				ShDefaultVariant = ShoeArchie;
				ShSoapVariant = false;
				Sh06Variant = GemDefault;
				ShSUVariant = SUAirboost;
				break;
			case ShSoapsLightSpd:
				ShSoapVariant = true;
				ShDefaultVariant = ShoeDefault;
				Sh06Variant = GemDefault;
				ShSUVariant = SUAirboost;
				break;
			case Sh06GemRed:
				Sh06Variant = GemRed;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				ShSUVariant = SUAirboost;
				break;
			case Sh06GemBlue:
				Sh06Variant = GemBlue;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				ShSUVariant = SUAirboost;
				break;
			case Sh06GemGreen:
				Sh06Variant = GemGreen;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				ShSUVariant = SUAirboost;
				break;
			case Sh06GemPurple:
				Sh06Variant = GemPurple;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				ShSUVariant = SUAirboost;
				break;
			case Sh06GemSky:
				Sh06Variant = GemSky;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				ShSUVariant = SUAirboost;
				break;
			case Sh06GemWhite:
				Sh06Variant = GemWhite;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				ShSUVariant = SUAirboost;
				break;
			case Sh06GemYellow:
				Sh06Variant = GemYellow;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				ShSUVariant = SUAirboost;
				break;
			case ShSULightSpd:
				ShSUVariant = SULightSpd;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				Sh06Variant = GemDefault;
				break;
			case ShSUStomp:
				ShSUVariant = SUStomp;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				Sh06Variant = GemDefault;
				break;
			case ShSUWallJmp:
				ShSUVariant = SUWallJmp;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				Sh06Variant = GemDefault;
				break;
			case ShSUWerehog:
				ShSUVariant = SUWerehog;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				Sh06Variant = GemDefault;
				break;
			case ShSUZoney:
				ShSUVariant = SUZoney;
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				Sh06Variant = GemDefault;
				break;
			default:
				ShDefaultVariant = ShoeDefault;
				ShSoapVariant = false;
				Sh06Variant = GemDefault;
				ShSUVariant = SUAirboost;
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
				default:
					scIcon->GetNode("star")->SetPatternIndex(0);
					break;
				}
				break;
			case UIPartHandR:
				switch (ItemOption)
				{
				case 1:
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
					if (ShSoapVariant)
					{
						if (SelectShoes == ShSoapsLightSpd)
							scChara->GetNode("img_ch_2")->SetPatternIndex(12);
						else
							scChara->GetNode("img_ch_2")->SetPatternIndex(11);
					}
					else
					{
						if (SelectShoes == ShSoaps)
							scChara->GetNode("img_ch_2")->SetPatternIndex(2);
						else
							scChara->GetNode("img_ch_2")->SetPatternIndex(1);
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
					//Riders Left Glove
					if (SelectHandL == HLRiders)
						scChara->GetNode("img_ch_3")->SetPatternIndex(8);
					else
						scChara->GetNode("img_ch_3")->SetPatternIndex(7);
					//Empty Placeholders
					scChara->GetNode("img_ch_2")->SetPatternIndex(7);
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
					//Riders Right Glove
					if (SelectHandR == HRRiders)
						scChara->GetNode("img_ch_2")->SetPatternIndex(10);
					else
						scChara->GetNode("img_ch_2")->SetPatternIndex(9);
					//Empty Placeholders
					scChara->GetNode("img_ch_1")->SetPatternIndex(9);
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
						if (!ShSoapVariant)
							SelectShoes = ShSoaps;
						else
							SelectShoes = ShSoapsLightSpd;
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
						Common::PlaySoundStatic(menuSoundHandle, 1000007);
						break;
					case 7:
						Common::PlaySoundStatic(menuSoundHandle, 1000007);
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
						SelectBody = BdDefault;
						break;
					case 1:
						Common::PlaySoundStatic(menuSoundHandle, 1000007);
						break;
					case 2:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectBody = BdWildFire;
						break;
					case 3:
						Common::PlaySoundStatic(menuSoundHandle, 1000007);
						break;
					case 4:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
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
						Common::PlaySoundStatic(menuSoundHandle, 1000007);
						break;
					case 3:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandL = HLRiders;
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
						Common::PlaySoundStatic(menuSoundHandle, 1000007);
						break;
					case 2:
						Common::PlaySoundStatic(menuSoundHandle, 1000005);
						PlayCursorAnim();
						SelectHandR = HRRiders;
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
						if (ShSoapVariant)
						{
							ShSoapVariant = false;
							if (SelectShoes == ShSoapsLightSpd)
								SelectShoes = ShSoaps;
						}
						else
						{
							ShSoapVariant = true;
							if (SelectShoes == ShSoaps)
								SelectShoes = ShSoapsLightSpd;
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
				writeINI(pFile);
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

			//printf("%d", ShSoapVariant);
			//printf(" - ");
			//printf("%d", HRMagicVariant);
			//printf(" - ");
			//printf("%d", ShSUVariant);
			//printf(" - ");
			//printf("%d", Sh06Variant);
			//printf(" - ");
			//printf("%f", scIcon->m_MotionFrame);
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
	if (scChara)
	{
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scChara);
		scChara = nullptr;
	}
	if (scIcon)
	{
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scIcon);
		scIcon = nullptr;
	}
	if (scTextArea)
	{
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scTextArea);
		scTextArea = nullptr;
	}
	if (scLRMove)
	{
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scLRMove);
		scLRMove = nullptr;
	}
	if (scDeco)
	{
		Chao::CSD::CProject::DestroyScene(prFittingScreen.Get(), scDeco);
		scDeco = nullptr;
	}
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
			{ "Gem Shoes", Sh06GemDefault},
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

//OnFrame
EXPORT void OnFrame()
{
	std::lock_guard lock(mutex);

	for (auto it = models.begin(); it != models.end();)
	{
		if (it->first.unique())
		{
			it = models.erase(it);
			printf("SONIC IS DEAD!!!!!!!!!!!!\n");
		}
		else
		{
			meshVisibilityHandler(it->first, it->second);
			++it;
		}
	}
}

//Init
EXPORT void Init()
{
	readINI();
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
	INSTALL_HOOK(CMirageDatabaseWrapperGetModelData);
	WRITE_MEMORY(0x16D4B4C, void*, CSonicRemoveCallback);
	WRITE_MEMORY(0x16A467C, void*, CHudSonicStageRemoveCallback);
	WRITE_MEMORY(0x16A5598, void*, CHudSonicStageRemoveCallback);
	//WRITE_NOP(0x0042AB50, 5); //idk i forgor
	//WRITE_JUMP(0x42A82C, (void*)0x42A9E5); //Jumps over entire state if
	//WRITE_JUMP(0x42A83D, (void*)0x42A843); //Jumps over stage switch
}