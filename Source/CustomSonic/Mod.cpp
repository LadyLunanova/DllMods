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
	ShClassic = 001,
	ShArchie = 02,
	ShSA1LightSpd = 10,
	ShSoaps = 20,
	ShSoapsLightSpd = 21,
	ShSoapsRacing = 22,
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
	ShSULightSpd = 51,
	ShSUStomp = 52,
	ShSUWallJmp = 53,
	ShSUWerehog = 54,
	ShSUZoney = 55,
	ShLinkBoots = 60,
	ShBoom = 70,
	ShPumas = 80,
	/*
	
	ShSimulator = xx,
	Sh2020Running = xx,
	Sh2020HorseShoe = xx,
	
	*/
};
SelectShoeType SelectShoes = SelectShoeType::ShDefault;
enum SelectBodyType
{
	BdDefault = 00,
	BdBlueArms = 01,
	BdRaceSuit = 10,
	BdWildFire = 20,
	BdLinkCos = 30,
	BdScarf = 40,
	BdScarfArms = 41,
	Bd2020SwimVest = 50,
	Bd2020SurfSuit = 60,
	Bd2020HorseShirt = 70,
	BdMovieSkin = 80,
	BdMovieSkinArms = 81,
	/*
	
	BdSimulator = xx,
	Bd2020Running = xx,
	BdSpeedSlugger = xx,
	BdSpeedPirate = xx,
	
	*/
};
SelectBodyType SelectBody = SelectBodyType::BdDefault;
enum SelectHeadType
{
	HeDefault = 00,
	HeSA1Sunglass = 10,
	HeRiders = 20,
	HeHeadphones = 30,
	HeLinkHat = 40,
	HeBoomSpikes = 50,
	/*
	
	HeSimulator = xx,
	HeSpeedSlugger = xx,
	HeSpeedPirate = xx,
	
	*/
};
SelectHeadType SelectHead = SelectHeadType::HeDefault;
enum SelectHandLType
{
	HLDefault = 00,
	HLCrystalRing = 10,
	HLFlameRing = 20,
	HLRacingGlove = 30,
	HLRiders = 40,
	HL06Bounce = 50,
	HLChipBracelet = 60,
	HLLinkGlove = 70,
	HLBoomGlove = 80,
};
SelectHandLType SelectHandL = SelectHandLType::HLDefault;
enum SelectHandRType
{
	HRDefault = 00,
	HRBounceBracelet = 10,
	HRMagicHands = 11,
	HRRacingGlove = 20,
	HRRiders = 30,
	HRArkCosmos = 31,
	HR06HomingSmash = 40,
	HRSecretRing = 50,
	HRGauntlet = 60,
	HRLinkGlove = 70,
	HRBoomGlove = 80,
};
SelectHandRType SelectHandR = SelectHandRType::HRDefault;
bool EyelidSkin = false;
bool NoBallJump = true;
bool BdBlueArmVariant = false;
bool BdScarfVariant = false;
bool BdMovieVariant = false;
bool HRMagicVariant = false;
bool HRRidersVariant = false;
enum ShDefaultVariantType
{
	ShoeDefault = 0,
	ShoeClassic = 1,
	ShoeArchie = 2,
};
ShDefaultVariantType ShDefaultVariant = ShDefaultVariantType::ShoeDefault;
enum ShSA2VariantType
{
	SA2Soap = 0,
	SA2LightSpd = 1,
	SA2Racing = 2,
};
ShSA2VariantType ShSA2Variant = ShSA2VariantType::SA2Soap;
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
enum ShSUVariantType
{
	SUAirboost = 0,
	SULightSpd = 1,
	SUStomp = 2,
	SUWallJmp = 3,
	SUWerehog = 4,
	SUZoney = 5,
};
ShSUVariantType ShSUVariant = ShSUVariantType::SUAirboost;

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

//Dynamic Omnis
// Original code by Skyth: https://github.com/blueskythlikesclouds
hh::base::CRefPtr<Sonic::CLocalLight> genericPLight;
hh::base::CRefPtr<Sonic::CLocalLight> superPLight;
hh::base::CRefPtr<Sonic::CLocalLight> grindPLight;
hh::base::CRefPtr<Sonic::CLocalLight> wildfirePLight;
void CustomParticleLight(float ColorR, float ColorG, float ColorB, float ColorA, float LightRange, Sonic::Player::CPlayer* player)
{
	auto gameDocument = Sonic::CGameDocument::GetInstance();
	if (!gameDocument.get().get() || !gameDocument->m_pMember->m_spLightManager)
		return;

	auto playerContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!playerContext)
		return;

	const auto& stateName = playerContext->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	const auto& animName = playerContext->GetCurrentAnimationName();

	auto FootNode = player->m_spCharacterModel->GetNode("Toe_R"); //Set up Chest bone matrix for VFX
	Eigen::Affine3f affineFootNode;
	affineFootNode = FootNode->m_WorldMatrix;
	hh::math::CVector FootVector;
	FootVector = affineFootNode.translation();

	if ((animName == "SpinAttack" && stateName == "Jump") || stateName == "Stomping" || playerContext->StateFlag(eStateFlag_Boost))
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		//if (stateName == "Sliding" || playerContext->StateFlag(eStateFlag_Boost))
		//	position += fwd * 0.5f + up * 0.25f;

		if (!genericPLight)
			if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
				genericPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
			else
				genericPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { ColorR, ColorG, ColorB, ColorA }, { 0.0f, 0.0f, 0.0f, LightRange });
		else
			genericPLight->SetPosition(position);
	}
	else if (stateName == "SquatKick" || stateName == "Sliding")
	{
		if (!genericPLight)
			if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
				genericPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(FootVector, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
			else
				genericPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(FootVector, { ColorR, ColorG * 1.23f, ColorB * 1.5f, ColorA }, { 0.0f, 0.0f, 0.0f, 2.2f });
		else
			genericPLight->SetPosition(FootVector);
	}
	else
		genericPLight = nullptr;

	if (stateName == "Grind" || stateName == "GrindSquat" || stateName == "GrindLandJumpShort")
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		position += up * 0.06f;

		if (!grindPLight)
			grindPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 2.0f, 0.666f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.5f });
		else
			grindPLight->SetPosition(position);
	}
	else
		grindPLight = nullptr;

	if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		if (stateName == "Sliding" || playerContext->StateFlag(eStateFlag_Boost))
			position += fwd * 0.5f + up * 0.25f;

		if (!superPLight)
			superPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
		else
			superPLight->SetPosition(position);
	}
	else
		superPLight = nullptr;
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
		if (nodeName == "Sonic_101_Classic")
			node->m_Visible = (SelectShoes == ShClassic);
		if (nodeName == "Sonic_102_Archie")
			node->m_Visible = (SelectShoes == ShArchie);
		//SA1 Lightspeed Shoes
		if (nodeName == "Sonic_110_SA1LightSpd")
			node->m_Visible = (SelectShoes == ShSA1LightSpd);
		//Soap Shoes
		if (nodeName == "Sonic_120_Soaps")
			node->m_Visible = ((SelectShoes == ShSoaps) || (SelectShoes == ShSoapsLightSpd));
		if (nodeName == "Sonic_121_SoapLightSpd")
			node->m_Visible = (SelectShoes == ShSoapsLightSpd);
		if (nodeName == "Sonic_122_SoapRacing")
			node->m_Visible = (SelectShoes == ShSoapsRacing);
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
		//Link Boots
		if (nodeName == "Sonic_160_LinkBoots")
			node->m_Visible = (SelectShoes == ShLinkBoots);
		//Boom Shoes
		if (nodeName == "Sonic_170_BoomShoes")
			node->m_Visible = (SelectShoes == ShBoom);
		//Puma Shoes
		if (nodeName == "Sonic_180_Pumas")
			node->m_Visible = (SelectShoes == ShPumas);

		/////Bodies//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Base Body
		if (nodeName == "Sonic_000_Body")
			node->m_Visible = ((SelectBody == BdDefault) || (SelectBody == BdMovieSkin) || (SelectBody == BdScarf) || (SelectBody == BdWildFire));
		//Racing Suit
		if (nodeName == "Sonic_210_Racing")
			node->m_Visible = (SelectBody == BdRaceSuit);
		//Link Tunic
		if (nodeName == "Sonic_230_LinkCos")
			node->m_Visible = (SelectBody == BdLinkCos);
		//Scarf
		if (nodeName == "Sonic_240_Scarf")
			node->m_Visible = ((SelectBody == BdScarf) || (SelectBody == BdScarfArms));
		if (nodeName == "Sonic_241_ScarfArms")
			node->m_Visible = (SelectBody == BdScarfArms) || (SelectBody == BdBlueArms) || (SelectBody == BdMovieSkinArms);
		//Swim Vest
		if (nodeName == "Sonic_250_Swim")
			node->m_Visible = (SelectBody == Bd2020SwimVest);
		//Surf Suit
		if (nodeName == "Sonic_260_Surf")
			node->m_Visible = (SelectBody == Bd2020SurfSuit);
		//Horse Shirt
		if (nodeName == "Sonic_270_Horse")
			node->m_Visible = (SelectBody == Bd2020HorseShirt);
		//Movie Quills
		if (nodeName == "Sonic_280_Movie_Body")
			node->m_Visible = (SelectBody == BdMovieSkin) || (SelectBody == BdMovieSkinArms);
		if (nodeName == "Sonic_280_Movie_Head")
			node->m_Visible = (((SelectBody == BdMovieSkin) || (SelectBody == BdMovieSkinArms)) && (SelectHead != HeLinkHat) && (SelectHead != HeBoomSpikes));
		if (nodeName == "Sonic_280_Movie_BoomHead")
			node->m_Visible = (((SelectBody == BdMovieSkin) || (SelectBody == BdMovieSkinArms)) && (SelectHead == HeBoomSpikes));
		if (nodeName == "Sonic_280_Movie_LinkHead")
			node->m_Visible = (((SelectBody == BdMovieSkin) || (SelectBody == BdMovieSkinArms)) && (SelectHead == HeLinkHat));

		/////Heads//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Base Head
		if (nodeName == "Sonic_001_Head")
			node->m_Visible = ((SelectHead != HeLinkHat) && (SelectHead != HeBoomSpikes));
		//SA1 Sunglasses
		if (nodeName == "Sonic_310_SA1SunGlass")
			node->m_Visible = (SelectHead == HeSA1Sunglass);
		//Riders Glasses
		if (nodeName == "Sonic_320_RidersGlass")
			node->m_Visible = (SelectHead == HeRiders);
		//Headphones
		if (nodeName == "Sonic_330_Headphone")
			node->m_Visible = (SelectHead == HeHeadphones);
		//Link Hat
		if (nodeName == "Sonic_340_LinkHat")
			node->m_Visible = (SelectHead == HeLinkHat);
		//Boom Spikes
		if (nodeName == "Sonic_350_BoomSpikes")
			node->m_Visible = (SelectHead == HeBoomSpikes);

		/////Left Hands//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "Sonic_400_Glove_L")
			node->m_Visible = ((SelectHandL != HLRiders) && (SelectHandL != HLLinkGlove) && (SelectHandL != HLRacingGlove) && (SelectHandL != HLBoomGlove));
		//Crystal Ring
		if (nodeName == "Sonic_410_CrystalRing")
			node->m_Visible = (SelectHandL == HLCrystalRing);
		//Flame Ring
		if (nodeName == "Sonic_420_FlameRing")
			node->m_Visible = (SelectHandL == HLFlameRing);
		//Racing Glove
		if (nodeName == "Sonic_430_RaceGlove_L")
			node->m_Visible = (SelectHandL == HLRacingGlove);
		//Riders Glove
		if (nodeName == "Sonic_440_RiderGlove_L")
			node->m_Visible = (SelectHandL == HLRiders);
		//06 Bounce Bracelet
		if (nodeName == "Sonic_450_06Bounce")
			node->m_Visible = (SelectHandL == HL06Bounce);
		//Chip Ring
		if (nodeName == "Sonic_460_ChipRing")
			node->m_Visible = (SelectHandL == HLChipBracelet);
		//Link Glove
		if (nodeName == "Sonic_470_LinkGlove_L")
			node->m_Visible = (SelectHandL == HLLinkGlove);
		//Boom Glove
		if (nodeName == "Sonic_480_BoomGlove_L")
			node->m_Visible = (SelectHandL == HLBoomGlove);

		/////Right Hands/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "Sonic_500_Glove_R")
			node->m_Visible = ((SelectHandR != HRRiders) && (SelectHandR != HRLinkGlove) && (SelectHandR != HRRacingGlove) && (SelectHandR != HRGauntlet) && (SelectHandR != HRArkCosmos) && (SelectHandR != HRBoomGlove));
		//Bounce Bracelet
		if (nodeName == "Sonic_510_Bounce")
			node->m_Visible = ((SelectHandR == HRBounceBracelet) || (SelectHandR == HRMagicHands));
		if (nodeName == "Sonic_511_Magic")
			node->m_Visible = (SelectHandR == HRMagicHands);
		//Racing Glove
		if (nodeName == "Sonic_520_RaceGlove_R")
			node->m_Visible = (SelectHandR == HRRacingGlove);
		//Riders Glove
		if (nodeName == "Sonic_530_RiderGlove_R")
			node->m_Visible = ((SelectHandR == HRRiders) || (SelectHandR == HRArkCosmos));
		if (nodeName == "Sonic_531_RiderArk")
			node->m_Visible = (SelectHandR == HRArkCosmos);
		//06 Homing Smash Bracelet
		if (nodeName == "Sonic_540_06Smash")
			node->m_Visible = (SelectHandR == HR06HomingSmash);
		//Secret Ring
		if (nodeName == "Sonic_550_SecretRing")
			node->m_Visible = (SelectHandR == HRSecretRing);
		//Gauntlet
		if (nodeName == "Sonic_560_Gauntlet_R")
			node->m_Visible = (SelectHandR == HRGauntlet);
		//Link Glove
		if (nodeName == "Sonic_570_LinkGlove_R")
			node->m_Visible = (SelectHandR == HRLinkGlove);
		//Boom Glove
		if (nodeName == "Sonic_580_BoomGlove_R")
			node->m_Visible = (SelectHandR == HRBoomGlove);

		/////Eyelids/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (nodeName == "Sonic_002_Eyelid_Skin")
			node->m_Visible = EyelidSkin;
		if (nodeName == "Sonic_003_Eyelid_Blue")
			node->m_Visible = !EyelidSkin;

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
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);

	if (IsModernSonic)
	{
		if (SelectBody == BdWildFire)
		{
			SpawnFireParticle(This);
			//sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_JumpShortReleaseTime] = 256.0f;
		}
		if (SelectBody != BdWildFire)
		{
			KillFireParticle(This);
			//sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_JumpShortReleaseTime);
		}

		//if (SelectShoes == ShSUWallJmp)
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_JumpPower] = 22.0f;
		//else if (SelectShoes == ShSUStomp)
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_JumpPower] = 12.3f;
		//else
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_JumpPower);

		//if (SelectShoes == ShSUAirboost)
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostTime] = 1000.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_BoostInAirKeepTime] = 1000.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostGravityRate] = 0.5f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostLevitationTime] = 0.25f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostFirstDecrease] = 0.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostEnableChaosEnergy] = 0.0f;
		//}
		//else
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostTime);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_BoostInAirKeepTime);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostGravityRate);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostLevitationTime);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostFirstDecrease);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostEnableChaosEnergy);
		//}

		//if (SelectShoes == ShSA1LightSpd || SelectShoes == ShSoaps || SelectShoes == ShSoapsLightSpd || SelectShoes == ShSoapsRacing)
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationSpeedMax] = 5000.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationSpeedMaxShoesSliding] = 5000.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationForce0] = 100.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationForce1] = 100.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationForce2] = 100.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_VertVelocityBallToFall] = -1000.0f;
		//}
		//else
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationSpeedMax);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationSpeedMaxShoesSliding);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationForce0);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationForce1);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationForce2);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_VertVelocityBallToFall);
		//}

		//if (SelectShoes == Sh06GemDefault ||
		//	SelectShoes == Sh06GemRed ||
		//	SelectShoes == Sh06GemYellow ||
		//	SelectShoes == Sh06GemWhite ||
		//	SelectShoes == Sh06GemGreen ||
		//	SelectShoes == Sh06GemPurple ||
		//	SelectShoes == Sh06GemSky ||
		//	SelectShoes == Sh06GemBlue)
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity] = 30.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicParameter_MotionRunToDash] = 48.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicParameter_MotionDashToJet] = 84.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicParameter_MotionJetToBoost] = 100.0f;
		//	
		//}
		//else
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicParameter_MotionRunToDash);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicParameter_MotionDashToJet);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicParameter_MotionJetToBoost);
		//}
	}

	//printf(sonic->GetCurrentAnimationName().c_str());
	//printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	//printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);

	CustomParticleLight(0.0, 0.4, 2.0, 1.0, 4.2, This);
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
	writeINI(pFile);
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
	writeINI(pFile);
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
	writeINI(pFile);
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

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationParams, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CParameterCategory* cat_Bounce = parameterGroup->CreateParameterCategory("Customizable Sonic", "Parameters for customizable Sonic");

	//cat_Bounce->CreateParamInt(&someInt, "Integer");
	//cat_Bounce->CreateParamBool(&someBool, "Bool");
	//cat_Bounce->CreateParamFloat(&someFloat, "Float");

	cat_Bounce->CreateParamBool(&ConfigDecoEnable, "Show the fitting room UI decorations.");
	cat_Bounce->CreateParamBool(&EyelidSkin, "Use skin eyelids.");

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
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicWildFire</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam_cmn</Name>\n";
		stream << "    <Archive>SystemCommon</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam000</Name>\n";
		stream << "    <Archive>SystemCommon</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam001</Name>\n";
		stream << "    <Archive>SystemCommon</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>SonicSp</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam000</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Title</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev021</Name>\n";
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
		stream << "    <Name>ev051</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev053</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev061</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev062</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev071</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev081</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev082</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev091</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev121</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev210</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev220</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev221</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev230</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev240</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev241</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev250</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev251</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev260</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev261</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev270</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev271</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev280</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev281</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev290</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev291</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev310</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev311</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev320</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev321</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev330</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev331</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev340</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev341</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev410</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev411</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev420</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev430</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>evt150</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
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
	////Model Visibility////
	std::lock_guard lock(mutex);

	for (auto it = models.begin(); it != models.end();)
	{
		if (it->first.unique())
		{
			it = models.erase(it);
			printf("SONIC MODEL DESTROYED\n");
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
	INIReader reader("Config.ini");
	ConfigDecoEnable = reader.GetBoolean("Mod", "ConfigDecoEnable", ConfigDecoEnable);
	EyelidSkin = reader.GetBoolean("Mod", "EyelidSkin", EyelidSkin);
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(MsgLookAtStart);
	INSTALL_HOOK(MsgLookAtEnd);
	INSTALL_HOOK(CHudSonicStageUpdate);
	INSTALL_HOOK(CHudPlayableMenuUpdate);
	INSTALL_HOOK(CHUDPauseUpdate);
	INSTALL_HOOK(CHudResultStart);
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
}