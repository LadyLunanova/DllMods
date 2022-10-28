#pragma once

//Items
enum SelectShoeType
{
	ShDefault = 0,
	ShClassic = 1,
	ShArchie = 2,
	ShSA1LightSpd = 3,
	ShSoaps = 4,
	ShSoapsLightSpd = 5,
	ShSoapsRacing = 6,
	ShRiders = 7,
	Sh06GemDefault = 8,
	Sh06GemRed = 9,
	Sh06GemBlue = 10,
	Sh06GemGreen = 11,
	Sh06GemPurple = 12,
	Sh06GemSky = 13,
	Sh06GemWhite = 14,
	Sh06GemYellow = 15,
	ShSUAirboost = 16,
	ShSULightSpd = 17,
	ShSUStomp = 18,
	ShSUWallJmp = 19,
	ShSUWerehog = 20,
	ShSUZoney = 21,
	ShSimulator = 22, //TBA
	ShLinkBoots = 22,
	ShBoom = 23,
	Sh2020Running = 24, //TBA
	Sh2020HorseShoe = 25, //TBA
	Sh2020Karate = 26, //TBA
	ShPumas = 27,
	ShKorone = 28, //TBA
	ShMonsterHunter = 29, //TBA
	ShDeluxe = 30, //TBA
};
enum SelectBodyType
{
	BdDefault = 0,
	BdBlueArms = 1,
	BdRaceSuit = 2,
	BdWildFire = 3,
	BdSimulator = 4, //TBA
	BdLinkCos = 5,
	BdScarf = 6,
	BdScarfArms = 7,
	Bd2020SwimVest = 8,
	Bd2020SurfSuit = 9,
	Bd2020HorseShirt = 10,
	Bd2020Running = 11, //TBA
	Bd2020Fence = 12, //TBA
	BdMovieSkin = 13,
	BdMovieSkinArms = 14,
	BdSpeedSlugger = 15,//TBA
	BdSpeedPirate = 16, //TBA
};
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
enum ShDefaultVariantType
{
	ShoeDefault = 0,
	ShoeClassic = 1,
	ShoeArchie = 2,
};
enum ShSA2VariantType
{
	SA2Soap = 0,
	SA2LightSpd = 1,
	SA2Racing = 2,
};
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
enum ShSUVariantType
{
	SUAirboost = 0,
	SULightSpd = 1,
	SUStomp = 2,
	SUWallJmp = 3,
	SUWerehog = 4,
	SUZoney = 5,
};

SelectShoeType SelectShoes = SelectShoeType::ShDefault;
SelectBodyType SelectBody = SelectBodyType::BdDefault;
SelectHeadType SelectHead = SelectHeadType::HeDefault;
SelectHandLType SelectHandL = SelectHandLType::HLDefault;
SelectHandRType SelectHandR = SelectHandRType::HRDefault;
ShDefaultVariantType ShDefaultVariant = ShDefaultVariantType::ShoeDefault;
ShSA2VariantType ShSA2Variant = ShSA2VariantType::SA2Soap;
Sh06VariantType Sh06Variant = Sh06VariantType::GemDefault;
ShSUVariantType ShSUVariant = ShSUVariantType::SUAirboost;

bool EyelidSkin = false;
bool NoBallJump = true;
bool BdBlueArmVariant = false;
bool BdScarfVariant = false;
bool BdMovieVariant = false;
bool HRMagicVariant = false;
bool HRRidersVariant = false;

void WildFireEnable(int Enabled);

void EyelidStatusGet(int Eyelid)
{
	if (Eyelid == 0)
		EyelidSkin = false;
	else
		EyelidSkin = true;
}

void ItemVisibilityHandler(const boost::shared_ptr<hh::mr::CModelData>& model, std::vector<boost::shared_ptr<hh::mr::CNodeGroupModelData>>& nodeGroupModels)
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

	//Send Wildfire VFX
	if (SelectBody == BdWildFire)
		WildFireEnable(1);
	else
		WildFireEnable(0);

	model->m_NodeGroupModelNum = model->m_NodeGroupModels.size();
}

void MaterialSelectionHandler(const boost::shared_ptr<hh::mr::CModelData>& model, std::vector<boost::shared_ptr<hh::mr::CMeshData>>& spMaterial)
{
	if (!model || !(model->m_Flags & hh::db::eDatabaseDataFlags_IsMadeOne))
		return;

	if (spMaterial.empty())
		spMaterial.assign(model->m_OpaqueMeshes.begin(), model->m_OpaqueMeshes.end());

	//model->m_OpaqueMeshes.clear();

	for (auto& mat : spMaterial)
	{
		auto matName = mat->m_MaterialName;

		if (matName == "sonic_gm_body")
			mat->m_MaterialName = "sonic_gm_body00";

		//if (matName != "sonic_gm_body")
		//	model->m_OpaqueMeshes.push_back(mat);
	}

	//model->m_NodeGroupModelNum = model->m_OpaqueMeshes.size();
}