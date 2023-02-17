#pragma once

<<<<<<< HEAD
////------Item Setup------////
enum SelectShoeType
{
	ShDefault, //Has Lightspeed variant
	ShClassic,
	ShSA1,
	ShSA1LightS,
	ShSA2Beta, //Has Lightspeed variant
	ShSA2Soap, //Has Lightspeed and racing variants
	ShRiders,
	Sh06,
	Sh06Gem, //has 7 color alts
	ShArchie,
	ShExCal,  //TBA
	ShSUAirboost,
	ShSULightS,
	ShSUStomp,
	ShSUWallJmp,
	ShSUWerehog,
	ShSUZoney,
	ShSimulator,
	ShLink,
	ShBoom,
	ShSpeedSlugger, //TBA
	ShSpeedPirate, //TBA
	ShSmash, //TBA
	Sh2020Running, //TBA
	Sh2020Horse, //TBA
	Sh2020Karate, //TBA
	ShPumas,
	ShSFDeluxe,
	ShSFKorone,
	ShSFMonsterHunter,
	ShPrmNewYoke, //TBA
	ShPrmBoscageMaze, //TBA
	ShPrmNoPlace, //TBA
	ShOverflow00,
	ShOverflow01,
	ShOverflow02,
};
enum SelectBodyType
{
	BdDefault, //Blue Arms
	BdSA2Racing,
	BdWildFire, //Blue Arms
	BdExCal,  //TBA
	BdSimulator,
	BdLink,
	BdScarf, //Blue Arms
	BdSpeedSlugger, //TBA
	BdSpeedPirate, //TBA
	Bd2020SwimVest,
	Bd2020SurfSuit,
	Bd2020Horse,
	Bd2020Running, //TBA
	Bd2020Fence, //TBA
	BdMovieSkin, //Blue Arms
	BdSFMonsterHunter,
	BdSFMonsterHunterF,
	BdSFHoliday,
	BdOverflow00,
	BdOverflow01,
	BdOverflow02,
};
enum SelectHeadType
{
	HeDefault,
	HeSA1Sunglass,
	HeRiders,
	HeHeadphones,
	HeExCal,  //TBA
	HeSimulator,
	HeLink,
	HeBoom,
	HeSpeedSlugger, //TBA
	HeSpeedPirate, //TBA
	HeSFMonsterHunter,
	HeSFMonsterHunterF,
	HeSFHoliday,
	HeOverflow00,
	HeOverflow01,
	HeOverflow02,
};
enum SelectHandLType
{
	HLDefault,
	HLSA1Crystal,
	HLSA2FlameRing,
	HLSA2Racing,
	HLRiders,
	HL06Bounce,
	HLExCal,  //TBA
	HLChip,
	HLSimulator,
	HLLink,
	HLBoom,
	HLSpeedSlugger, //TBA
	HLSpeedPirate, //TBA
	HLSmash, //TBA
	HL2020Karate, //TBA
	HLSFDeluxe,
	HLSFKorone,
	HLSFMonsterHunter,
	HLPrmNewYoke, //TBA
	HLPrmBoscageMaze, //TBA
	HLPrmNoPlace, //TBA
	HLOverflow00,
	HLOverflow01,
	HLOverflow02,
};
enum SelectHandRType
{
	HRDefault,
	HRSA2Bounce,
	HRSA2Racing,
	HRRiders,
	HRRidersArkCosmos,
	HR06Homing,
	HRSecretRing,
	HRGauntlet,
	HRExCal,  //TBA
	HRSimulator,
	HRLink,
	HRBoom,
	HRSpeedSlugger, //TBA
	HRSpeedPirate, //TBA
	HRSmash, //TBA
	HR2020Karate, //TBA
	HRSFDeluxe,
	HRSFKorone,
	HRSFMonsterHunter,
	HRPrmNewYoke, //TBA
	HRPrmBoscageMaze, //TBA
	HRPrmNoPlace, //TBA
	HROverflow00,
	HROverflow01,
	HROverflow02,
};
enum SelectSonicBodyType
{
	SBSnMaterial,
	SBSsnMaterial,
	SBEyelids,
	SBSuperHead,
	SBOverflow04,
	SBOverflow05,
	SBOverflow06,
	SBOverflow07,
	SBOverflow08,
=======
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
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
};

SelectShoeType SelectShoes = SelectShoeType::ShDefault;
SelectBodyType SelectBody = SelectBodyType::BdDefault;
SelectHeadType SelectHead = SelectHeadType::HeDefault;
SelectHandLType SelectHandL = SelectHandLType::HLDefault;
SelectHandRType SelectHandR = SelectHandRType::HRDefault;
<<<<<<< HEAD
SelectSonicBodyType SelectSonicBody = SelectSonicBodyType::SBSnMaterial;

static std::map<int, const char*> MAP_THUMB_SHOE =
{
	{ ShDefault,			"ShDefault"},
	{ ShClassic,			"ShClassic" },
	{ ShSA1,				"ShSA1" },
	{ ShSA1LightS,			"ShSA1LightS" },
	{ ShSA2Beta,			"ShSA2Beta" },
	{ ShSA2Soap,			"ShSA2Soap" },
	{ ShRiders,				"ShRiders" },
	{ Sh06,					"Sh06" },
	{ Sh06Gem,				"Sh06Gem" },
	{ ShArchie,				"ShArchie" },
	{ ShExCal,				"ShExCal" },
	{ ShSUAirboost,			"ShSUAirboost" },
	{ ShSULightS,			"ShSULightS" },
	{ ShSUStomp,			"ShSUStomp" },
	{ ShSUWallJmp,			"ShSUWallJmp" },
	{ ShSUWerehog,			"ShSUWerehog" },
	{ ShSUZoney,			"ShSUZoney" },
	{ ShSimulator,			"ShSimulator" },
	{ ShLink,				"ShLink" },
	{ ShBoom,				"ShBoom" },
	{ ShSpeedSlugger,		"ShSpeedSlugger" },
	{ ShSpeedPirate,		"ShSpeedPirate" },
	{ ShSmash,				"ShSmash" },
	{ Sh2020Running,		"Sh2020Running" },
	{ Sh2020Horse,			"Sh2020Horse" },
	{ Sh2020Karate,			"Sh2020Karate" },
	{ ShPumas,				"ShPumas" },
	{ ShSFDeluxe,			"ShSFDeluxe" },
	{ ShSFKorone,			"ShSFKorone" },
	{ ShSFMonsterHunter,	"ShSFMonsterHunter" },
	{ ShPrmNewYoke,			"ShPrmNewYoke" },
	{ ShPrmBoscageMaze,		"ShPrmBoscageMaze" },
	{ ShPrmNoPlace,			"ShPrmNoPlace" },
	{ ShOverflow00,			"Null" },
	{ ShOverflow01,			"Null" },
	{ ShOverflow02,			"Null" },
};
static std::map<int, const char*> MAP_THUMB_BODY =
{
	{ BdDefault,			"BdDefault"},
	{ BdSA2Racing,			"BdSA2Racing" },
	{ BdWildFire,			"BdWildFire" },
	{ BdExCal,				"BdExCal" },
	{ BdSimulator,			"BdSimulator" },
	{ BdLink,				"BdLink" },
	{ BdScarf,				"BdScarf" },
	{ BdSpeedSlugger,		"BdSpeedSlugger" },
	{ BdSpeedPirate,		"BdSpeedPirate" },
	{ Bd2020SwimVest,		"Bd2020SwimVest" },
	{ Bd2020SurfSuit,		"Bd2020SurfSuit" },
	{ Bd2020Horse,			"Bd2020Horse" },
	{ Bd2020Running,		"Bd2020Running" },
	{ Bd2020Fence,			"Bd2020Fence" },
	{ BdMovieSkin,			"BdMovieSkin" },
	{ BdSFMonsterHunter,	"BdSFMonsterHunter" },
	{ BdSFMonsterHunterF,	"BdSFMonsterHunterF" },
	{ BdSFHoliday,			"BdSFHoliday" },
	{ BdOverflow00,			"Null" },
	{ BdOverflow01,			"Null" },
	{ BdOverflow02,			"Null" },
};
static std::map<int, const char*> MAP_THUMB_HEAD =
{
	{ HeDefault,			"HeDefault"},
	{ HeSA1Sunglass,		"HeSA1Sunglass" },
	{ HeRiders,				"HeRiders" },
	{ HeHeadphones,			"HeHeadphones" },
	{ HeExCal,				"HeExCal" },
	{ HeSimulator,			"HeSimulator" },
	{ HeLink,				"HeLink" },
	{ HeBoom,				"HeBoom" },
	{ HeSpeedSlugger,		"HeSpeedSlugger" },
	{ HeSpeedPirate,		"HeSpeedPirate" },
	{ HeSFMonsterHunter,	"HeSFMonsterHunter" },
	{ HeSFMonsterHunterF,	"HeSFMonsterHunterF" },
	{ HeSFHoliday,			"HeSFHoliday" },
	{ HeOverflow00,			"Null" },
	{ HeOverflow01,			"Null" },
	{ HeOverflow02,			"Null" },
};
static std::map<int, const char*> MAP_THUMB_HANDL =
{
	{ HLDefault,			"HLDefault"},
	{ HLSA1Crystal,			"HLSA1Crystal"},
	{ HLSA2FlameRing,		"HLSA2FlameRing"},
	{ HLSA2Racing,			"HLSA2Racing"},
	{ HLRiders,				"HLRiders"},
	{ HL06Bounce,			"HL06Bounce"},
	{ HLExCal,				"HLExCal"},
	{ HLChip,				"HLChip"},
	{ HLSimulator, 			"HLSimulator"},
	{ HLLink,				"HLLink"},
	{ HLBoom,				"HLBoom"},
	{ HLSpeedSlugger, 		"HLSpeedSlugger"},
	{ HLSpeedPirate, 		"HLSpeedPirate"},
	{ HLSmash, 				"HLSmash"},
	{ HL2020Karate, 		"HL2020Karate"},
	{ HLSFDeluxe, 			"HLSFDeluxe"},
	{ HLSFKorone,			"HLSFKorone"},
	{ HLSFMonsterHunter, 	"HLSFMonsterHunter"},
	{ HLPrmNewYoke, 		"HLPrmNewYoke"},
	{ HLPrmBoscageMaze, 	"HLPrmBoscageMaze"},
	{ HLPrmNoPlace, 		"HLPrmNoPlace"},
	{ HLOverflow00,			"Null" },
	{ HLOverflow01,			"Null" },
	{ HLOverflow02,			"Null" },
};
static std::map<int, const char*> MAP_THUMB_HANDR =
{
	{ HRDefault,			"HRDefault"},
	{ HRSA2Bounce,			"HRSA2Bounce"},
	{ HRSA2Racing,			"HRSA2Racing"},
	{ HRRiders,				"HRRiders"},
	{ HRRidersArkCosmos,	"HRRidersArkCosmos"},
	{ HR06Homing,			"HR06Homing"},
	{ HRSecretRing,			"HRSecretRing"},
	{ HRGauntlet,			"HRGauntlet"},
	{ HRExCal,				"HRExCal"},
	{ HRSimulator, 			"HRSimulator"},
	{ HRLink,				"HRLink"},
	{ HRBoom,				"HRBoom"},
	{ HRSpeedSlugger, 		"HRSpeedSlugger"},
	{ HRSpeedPirate, 		"HRSpeedPirate"},
	{ HRSmash, 				"HRSmash"},
	{ HR2020Karate, 		"HR2020Karate"},
	{ HRSFDeluxe, 			"HRSFDeluxe"},
	{ HRSFKorone,			"HRSFKorone"},
	{ HRSFMonsterHunter, 	"HRSFMonsterHunter"},
	{ HRPrmNewYoke, 		"HRPrmNewYoke"},
	{ HRPrmBoscageMaze, 	"HRPrmBoscageMaze"},
	{ HRPrmNoPlace, 		"HRPrmNoPlace"},
	{ HROverflow00,			"Null" },
	{ HROverflow01,			"Null" },
	{ HROverflow02,			"Null" },
};
static std::map<int, const char*> MAP_THUMB_SONICBODY =
{
	{ SBSnMaterial,			"Null" },
	{ SBSsnMaterial,		"Null" },
	{ SBEyelids,			"Null" },
	{ SBSuperHead,			"Null" },
	{ SBOverflow04,			"Null" },
	{ SBOverflow05,			"Null" },
	{ SBOverflow06,			"Null" },
	{ SBOverflow07,			"Null" },
	{ SBOverflow08,			"Null" },
};

////------Variant Setup------////

enum ShDefaultVariantType
{
	DefaultDefault,
	DefaultLightS,
};
enum ShSA2BetaVariantType
{
	SA2Beta,
	SA2BetaLightS,
};
enum ShSA2SoapVariantType
{
	SA2Soap,
	SA2SoapLightS,
	SA2SoapRacing,
};
enum Sh06GemVariantType
{
	GemDefault,
	GemRed,
	GemBlue,
	GemGreen,
	GemPurple,
	GemSky,
	GemWhite,
	GemYellow,
};

ShDefaultVariantType ShDefaultVariant = ShDefaultVariantType::DefaultDefault;
ShSA2BetaVariantType ShSA2BetaVariant = ShSA2BetaVariantType::SA2Beta;
ShSA2SoapVariantType ShSA2SoapVariant = ShSA2SoapVariantType::SA2Soap;
Sh06GemVariantType Sh06GemVariant = Sh06GemVariantType::GemDefault;

bool BdDefaultVariant = false;
bool BdWildFireVariant = false;
bool BdScarfVariant = false;
bool BdMovieVariant = false;
bool HeSimulatorVariant = false;
bool HRSA2BounceVariant = false;

////------Special Setup------////

enum SelectSnSonMatType
{
	SnMatDefault,
	SnMatOriginal,
	SnMatS4E2,
	SnMatRed,
	SnMatGreen,
	SnMatPink,
	SnMatBlack,
};
enum SelectSsnSonMatType
{
	SsnMatDefault,
	SsnMatOriginal,
	SsnMatRed,
	SsnMatGreen,
	SsnMatPink,
	SsnMatBlack,
	SsnMatPurple,
};
enum SelectSsnFormType
{
	SsnFormSuper,
	SsnFormHyper,
	SsnFormFleetway,
	SsnFormUltra,
	SsnFormDark,
	SsnFormDarkspine,
	SsnFormFire,
	SsnFormExcalibur,
	SsnFormCyber,
};
enum SelectEyelidType
{
	EyelidDefault,
	EyelidLashes,
	EyelidSkin,
};
enum SelectEffJumpType
{
	EffJmpDefault,
	EffJmpNoBall,
	EffJmpNoVFX,
	EffJmpBAP,
	EffJmpForces,
	EffJmpBetaSWA,
	EffJmpSWA,
	EffJmpSA1,
	EffJmpLW,
};

SelectSnSonMatType SelectSnSonMat = SelectSnSonMatType::SnMatDefault;
SelectSsnSonMatType SelectSsnSonMat = SelectSsnSonMatType::SsnMatDefault;
SelectSsnFormType SelectSsnForm = SelectSsnFormType::SsnFormSuper;
SelectEyelidType SelectEyelid = SelectEyelidType::EyelidDefault;
SelectEffJumpType SelectEffJump = SelectEffJumpType::EffJmpDefault;
bool NoBallJump = true;

int HyperFrameCycle = 0;

void MsgWildFire(int Enabled);
void MsgJumpBall(int BallType);
=======
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
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9

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
<<<<<<< HEAD
		if (nodeName == "ShDefault_00")
			node->m_Visible = (SelectShoes == ShDefault && (ShDefaultVariant == DefaultDefault) && SelectBody != BdSFMonsterHunter);
		if (nodeName == "ShDefault_01")
			node->m_Visible = (SelectShoes == ShDefault && (ShDefaultVariant == DefaultLightS) && SelectBody != BdSFMonsterHunter);

		//Classic Shoes
		if (nodeName == "ShClassic")
			node->m_Visible = (SelectShoes == ShClassic && SelectBody != BdSFMonsterHunter);

		//SA1 Lightspeed Shoes
		if (nodeName == "ShSA1LightS")
			node->m_Visible = (SelectShoes == ShSA1LightS && SelectBody != BdSFMonsterHunter);

		//SA2 Soap Shoes
		if (nodeName == "ShSA2Soap_00")
			node->m_Visible = (SelectShoes == ShSA2Soap && (ShSA2SoapVariant != SA2SoapRacing) && SelectBody != BdSFMonsterHunter);
		if (nodeName == "ShSA2Soap_01")
			node->m_Visible = (SelectShoes == ShSA2Soap && (ShSA2SoapVariant == SA2SoapLightS) && SelectBody != BdSFMonsterHunter);
		if (nodeName == "ShSA2Soap_02")
			node->m_Visible = (SelectShoes == ShSA2Soap && (ShSA2SoapVariant == SA2SoapRacing) && SelectBody != BdSFMonsterHunter);

		//Riders Shoes
		if (nodeName == "ShRiders")
			node->m_Visible = (SelectShoes == ShRiders && SelectBody != BdSFMonsterHunter);

		//06 Gem Shoes
		if (nodeName == "Sh06Gem_Default")
			node->m_Visible = (SelectShoes == Sh06Gem && Sh06GemVariant == GemDefault && SelectBody != BdSFMonsterHunter);
		if (nodeName == "Sh06Gem_Red")
			node->m_Visible = (SelectShoes == Sh06Gem && Sh06GemVariant == GemRed && SelectBody != BdSFMonsterHunter);
		if (nodeName == "Sh06Gem_Blue")
			node->m_Visible = (SelectShoes == Sh06Gem && Sh06GemVariant == GemBlue && SelectBody != BdSFMonsterHunter);
		if (nodeName == "Sh06Gem_Green")
			node->m_Visible = (SelectShoes == Sh06Gem && Sh06GemVariant == GemGreen && SelectBody != BdSFMonsterHunter);
		if (nodeName == "Sh06Gem_Purple")
			node->m_Visible = (SelectShoes == Sh06Gem && Sh06GemVariant == GemPurple && SelectBody != BdSFMonsterHunter);
		if (nodeName == "Sh06Gem_Sky")
			node->m_Visible = (SelectShoes == Sh06Gem && Sh06GemVariant == GemSky && SelectBody != BdSFMonsterHunter);
		if (nodeName == "Sh06Gem_White")
			node->m_Visible = (SelectShoes == Sh06Gem && Sh06GemVariant == GemWhite && SelectBody != BdSFMonsterHunter);
		if (nodeName == "Sh06Gem_Yellow")
			node->m_Visible = (SelectShoes == Sh06Gem && Sh06GemVariant == GemYellow && SelectBody != BdSFMonsterHunter);

		//Archie Shoes
		if (nodeName == "ShArchie")
			node->m_Visible = (SelectShoes == ShArchie && SelectBody != BdSFMonsterHunter);

		//Unleashed Shoes
		if (nodeName == "ShSUAirboost")
			node->m_Visible = (SelectShoes == ShSUAirboost && SelectBody != BdSFMonsterHunter);
		if (nodeName == "ShSULightS")
			node->m_Visible = (SelectShoes == ShSULightS && SelectBody != BdSFMonsterHunter);
		if (nodeName == "ShSUStomp")
			node->m_Visible = (SelectShoes == ShSUStomp && SelectBody != BdSFMonsterHunter);
		if (nodeName == "ShSUWallJmp")
			node->m_Visible = (SelectShoes == ShSUWallJmp && SelectBody != BdSFMonsterHunter);
		if (nodeName == "ShSUWerehog")
			node->m_Visible = (SelectShoes == ShSUWerehog && SelectBody != BdSFMonsterHunter);
		if (nodeName == "ShSUZoney")
			node->m_Visible = (SelectShoes == ShSUZoney && SelectBody != BdSFMonsterHunter);

		//Simulator Shoes
		if (nodeName == "ShSimulator")
			node->m_Visible = (SelectShoes == ShSimulator && SelectBody != BdSFMonsterHunter);

		//Link Boots
		if (nodeName == "ShLink")
			node->m_Visible = (SelectShoes == ShLink && SelectBody != BdSFMonsterHunter);

		//Boom Shoes
		if (nodeName == "ShBoom")
			node->m_Visible = (SelectShoes == ShBoom && SelectBody != BdSFMonsterHunter);

		//Puma Shoes
		if (nodeName == "ShPumas")
			node->m_Visible = (SelectShoes == ShPumas && SelectBody != BdSFMonsterHunter);

		//Prime New Yoke Shoes
		if (nodeName == "ShPrmNewYoke")
			node->m_Visible = (SelectShoes == ShPrmNewYoke && SelectBody != BdSFMonsterHunter);

		//Frontiers Deluxe Shoes
		if (nodeName == "ShSFDeluxe")
			node->m_Visible = (SelectShoes == ShSFDeluxe && SelectBody != BdSFMonsterHunter);

		//Frontiers Korone Shoes
		if (nodeName == "ShSFKorone")
			node->m_Visible = (SelectShoes == ShSFKorone && SelectBody != BdSFMonsterHunter);

		//Frontiers Monster Hunter
		if (nodeName == "ShSFMonsterHunter")
			node->m_Visible = (SelectShoes == ShSFMonsterHunter || SelectBody == BdSFMonsterHunter);

		/////Bodies//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Base Body
		if (nodeName == "BdDefault_00")
			node->m_Visible = ((SelectBody == BdDefault && BdDefaultVariant == false) ||
				(SelectBody == BdWildFire && BdWildFireVariant == false) ||
				(SelectBody == BdScarf && BdScarfVariant == false) ||
				(SelectBody == BdMovieSkin && BdMovieVariant == false));
		if (nodeName == "BdDefault_01")
			node->m_Visible = ((SelectBody == BdDefault && BdDefaultVariant == true) ||
				(SelectBody == BdWildFire && BdWildFireVariant == true) ||
				(SelectBody == BdScarf && BdScarfVariant == true) ||
				(SelectBody == BdMovieSkin && BdMovieVariant == true));

		//Racing Suit
		if (nodeName == "BdSA2Racing")
			node->m_Visible = (SelectBody == BdSA2Racing);

		//Simulator
		if (nodeName == "BdSimulator")
			node->m_Visible = (SelectBody == BdSimulator);

		//Link Tunic
		if (nodeName == "BdLink")
			node->m_Visible = (SelectBody == BdLink);

		//Scarf
		if (nodeName == "BdScarf")
			node->m_Visible = (SelectBody == BdScarf);

		//2020 Swim Vest
		if (nodeName == "Bd2020SwimVest")
			node->m_Visible = (SelectBody == Bd2020SwimVest);

		//2020 Surf Suit
		if (nodeName == "Bd2020SurfSuit")
			node->m_Visible = (SelectBody == Bd2020SurfSuit);

		//2020 Horse Shirt
		if (nodeName == "Bd2020Horse")
			node->m_Visible = (SelectBody == Bd2020Horse);

		//Movie Quills
		if (nodeName == "BdMovieSkin")
			node->m_Visible = (SelectBody == BdMovieSkin);
		if (nodeName == "HeDefault_Movie")
			node->m_Visible = ((SelectBody == BdMovieSkin) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF));
		if (nodeName == "HeDefault_00_Movie")
			node->m_Visible = ((SelectBody == BdMovieSkin) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF));
		if (nodeName == "HeDefault_01_Movie")
			node->m_Visible = false;
		if (nodeName == "HeBoom_Movie")
			node->m_Visible = ((SelectBody == BdMovieSkin) && (SelectHead == HeBoom));
		if (nodeName == "HeLink_Movie")
			node->m_Visible = ((SelectBody == BdMovieSkin) && (SelectHead == HeLink));
		if (nodeName == "HeSFMonsterHunterF_Movie")
			node->m_Visible = ((SelectBody == BdMovieSkin) && (SelectHead == HeSFMonsterHunterF));

		//Frontiers Monster Hunter
		if (nodeName == "BdSFMonsterHunter")
			node->m_Visible = (SelectBody == BdSFMonsterHunter);

		//Frontiers Monster Hunter Felyne
		if (nodeName == "BdSFMonsterHunterF")
			node->m_Visible = (SelectBody == BdSFMonsterHunterF);

		//Frontiers Holiday Cheer Suit
		if (nodeName == "BdSFHoliday")
			node->m_Visible = (SelectBody == BdSFHoliday);

		/////Heads//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Base Head
		if (nodeName == "HeDefault")
			node->m_Visible = (!(SelectHead == HeSimulator && HeSimulatorVariant == false) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF) && (SelectHead != HeSFMonsterHunter));
		
		//Super Heads
		if (nodeName == "HeDefault_00")
			node->m_Visible = (!(SelectHead == HeSimulator && HeSimulatorVariant == false) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF) && (SelectHead != HeSFMonsterHunter));
		if (nodeName == "HeDefault_01")
			node->m_Visible = false;

		//SA1 Sunglasses
		if (nodeName == "HeSA1Sunglass")
			node->m_Visible = (SelectHead == HeSA1Sunglass);

		//Riders Glasses
		if (nodeName == "HeRiders")
			node->m_Visible = (SelectHead == HeRiders);

		//Headphones
		if (nodeName == "HeHeadphones")
			node->m_Visible = (SelectHead == HeHeadphones);

		//Simulator
		if (nodeName == "HeSimulator_00")
			node->m_Visible = (SelectHead == HeSimulator && HeSimulatorVariant == false);
		if (nodeName == "HeSimulator_01")
			node->m_Visible = (SelectHead == HeSimulator && HeSimulatorVariant == true);

		//Link Hat
		if (nodeName == "HeLink")
			node->m_Visible = (SelectHead == HeLink);

		//Boom Spikes
		if (nodeName == "HeBoom")
			node->m_Visible = (SelectHead == HeBoom);

		//Frontiers Monster Hunter
		if (nodeName == "HeSFMonsterHunter")
			node->m_Visible = (SelectHead == HeSFMonsterHunter);

		//Frontiers Monster Hunter Felyne
		if (nodeName == "HeSFMonsterHunterF")
			node->m_Visible = (SelectHead == HeSFMonsterHunterF);

		//Frontiers Holiday Cheer Suit
		if (nodeName == "HeSFHoliday")
			node->m_Visible = (SelectHead == HeSFHoliday);

		/////Left Hands//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "HLDefault")
			node->m_Visible = (((SelectHandL == HLDefault) ||
				(SelectHandL == HLSA1Crystal) ||
				(SelectHandL == HLSA2FlameRing) ||
				(SelectHandL == HL06Bounce) ||
				(SelectHandL == HLChip)) &&
				(SelectBody != BdSFMonsterHunter));

		//SA1 Crystal Ring
		if (nodeName == "HLSA1Crystal")
			node->m_Visible = (SelectHandL == HLSA1Crystal && SelectBody != BdSFMonsterHunter);

		//SA2 Flame Ring
		if (nodeName == "HLSA2FlameRing")
			node->m_Visible = (SelectHandL == HLSA2FlameRing && SelectBody != BdSFMonsterHunter);

		//SA2 Racing Glove
		if (nodeName == "HLSA2Racing")
			node->m_Visible = (SelectHandL == HLSA2Racing && SelectBody != BdSFMonsterHunter);

		//Riders Glove
		if (nodeName == "HLRiders")
			node->m_Visible = (SelectHandL == HLRiders && SelectBody != BdSFMonsterHunter);

		//06 Bounce Bracelet
		if (nodeName == "HL06Bounce")
			node->m_Visible = (SelectHandL == HL06Bounce && SelectBody != BdSFMonsterHunter);

		//Chip Ring
		if (nodeName == "HLChip")
			node->m_Visible = (SelectHandL == HLChip && SelectBody != BdSFMonsterHunter);

		//Simulator
		if (nodeName == "HLSimulator")
			node->m_Visible = (SelectHandL == HLSimulator && SelectBody != BdSFMonsterHunter);

		//Link Glove
		if (nodeName == "HLLink")
			node->m_Visible = (SelectHandL == HLLink && SelectBody != BdSFMonsterHunter);

		//Boom Glove
		if (nodeName == "HLBoom")
			node->m_Visible = (SelectHandL == HLBoom && SelectBody != BdSFMonsterHunter);

		//Frontiers Deluxe Glove
		if (nodeName == "HLSFDeluxe")
			node->m_Visible = (SelectHandL == HLSFDeluxe && SelectBody != BdSFMonsterHunter);

		//Frontiers Korone Glove
		if (nodeName == "HLSFKorone")
			node->m_Visible = (SelectHandL == HLSFKorone && SelectBody != BdSFMonsterHunter);

		//Frontiers Monster Hunter
		if (nodeName == "HLSFMonsterHunter")
			node->m_Visible = (SelectHandL == HLSFMonsterHunter || SelectBody == BdSFMonsterHunter);

		/////Right Hands/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "HRDefault")
			node->m_Visible = (((SelectHandR == HRDefault) ||
				(SelectHandR == HRSA2Bounce) ||
				(SelectHandR == HR06Homing) ||
				(SelectHandR == HRSecretRing)) && 
				(SelectBody != BdSFMonsterHunter));

		//SA2 Bounce Bracelet
		if (nodeName == "HRSA2Bounce_00")
			node->m_Visible = (SelectHandR == HRSA2Bounce) && (SelectBody != BdSFMonsterHunter);
		if (nodeName == "HRSA2Bounce_01")
			node->m_Visible = ((SelectHandR == HRSA2Bounce && HRSA2BounceVariant == true) && (SelectBody != BdSFMonsterHunter));

		//SA2 Racing Glove
		if (nodeName == "HRSA2Racing")
			node->m_Visible = (SelectHandR == HRSA2Racing && SelectBody != BdSFMonsterHunter);

		//Riders Glove
		if (nodeName == "HRRiders")
			node->m_Visible = ((SelectHandR == HRRiders || SelectHandR == HRRidersArkCosmos) && SelectBody != BdSFMonsterHunter);
		if (nodeName == "HRRidersArkCosmos")
			node->m_Visible = (SelectHandR == HRRidersArkCosmos && SelectBody != BdSFMonsterHunter);

		//06 Homing Smash Bracelet
		if (nodeName == "HR06Homing")
			node->m_Visible = (SelectHandR == HR06Homing && SelectBody != BdSFMonsterHunter);

		//Secret Ring
		if (nodeName == "HRSecretRing")
			node->m_Visible = (SelectHandR == HRSecretRing && SelectBody != BdSFMonsterHunter);

		//Gauntlet
		if (nodeName == "HRGauntlet")
			node->m_Visible = (SelectHandR == HRGauntlet && SelectBody != BdSFMonsterHunter);

		//Simulator
		if (nodeName == "HRSimulator")
			node->m_Visible = (SelectHandR == HRSimulator && SelectBody != BdSFMonsterHunter);

		//Link Glove
		if (nodeName == "HRLink")
			node->m_Visible = (SelectHandR == HRLink && SelectBody != BdSFMonsterHunter);

		//Boom Glove
		if (nodeName == "HRBoom")
			node->m_Visible = (SelectHandR == HRBoom && SelectBody != BdSFMonsterHunter);

		//Frontiers Deluxe Glove
		if (nodeName == "HRSFDeluxe")
			node->m_Visible = (SelectHandR == HRSFDeluxe && SelectBody != BdSFMonsterHunter);

		//Frontiers Korone Glove
		if (nodeName == "HRSFKorone")
			node->m_Visible = (SelectHandR == HRSFKorone && SelectBody != BdSFMonsterHunter);

		//Frontiers Monster Hunter
		if (nodeName == "HRSFMonsterHunter")
			node->m_Visible = (SelectHandR == HRSFMonsterHunter || SelectBody == BdSFMonsterHunter);

		/////Misc/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (nodeName == "EyeDefault")
			node->m_Visible = (SelectEyelid == EyelidDefault);
		if (nodeName == "EyeLash")
			node->m_Visible = (SelectEyelid == EyelidLashes);
		if (nodeName == "EyeSkin")
			node->m_Visible = (SelectEyelid == EyelidSkin);
=======
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
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9

		//printf("%s\n", nodeName.c_str());

		if (node->m_Visible)
			model->m_NodeGroupModels.push_back(node);
	}

	//Send Wildfire VFX
	if (SelectBody == BdWildFire)
<<<<<<< HEAD
		MsgWildFire(1);
	else
		MsgWildFire(0);

	MsgJumpBall(SelectEffJump);

	model->m_NodeGroupModelNum = model->m_NodeGroupModels.size();

	if (HyperFrameCycle >= 47)
		HyperFrameCycle = 0;
	else
		HyperFrameCycle++;

	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedUp = input.IsTapped(Sonic::eKeyState_DpadUp);
	bool PressedDown = input.IsTapped(Sonic::eKeyState_DpadDown);
	bool PressedLeft = input.IsTapped(Sonic::eKeyState_DpadLeft);
	bool PressedRight = input.IsTapped(Sonic::eKeyState_DpadRight);
	if (PressedUp)
	{
		if (SelectSnSonMat != SnMatBlack)
			SelectSnSonMat = (SelectSnSonMatType)(SelectSnSonMat + 1);
		else
			SelectSnSonMat = SnMatDefault;
	}
	if (PressedDown)
	{
		if (SelectSnSonMat != SnMatDefault)
			SelectSnSonMat = (SelectSnSonMatType)(SelectSnSonMat - 1);
		else
			SelectSnSonMat = SnMatBlack;
	}
	if (PressedRight)
	{
		if (SelectSsnSonMat != SsnMatBlack)
			SelectSsnSonMat = (SelectSsnSonMatType)(SelectSsnSonMat + 1);
		else
			SelectSsnSonMat = SsnMatDefault;
	}
	if (PressedLeft)
	{
		if (SelectSsnSonMat != SsnMatDefault)
			SelectSsnSonMat = (SelectSsnSonMatType)(SelectSsnSonMat - 1);
		else
			SelectSsnSonMat = SsnMatBlack;
	}
}

void traverse(hh::mr::CRenderable* renderable)
{
	if (auto element = dynamic_cast<hh::mr::CSingleElement*>(renderable))
	{
		////------Materials------////

		auto sonic_gm_body_custom = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("sonic_gm_body"); //Custom Fur
		auto sonic_gm_body_original = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("sonic_gm_body00"); //Original Fur
		auto sonic_gm_body_S4E2 = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("sonic_gm_body01"); //S4E2 Fur
		auto sonic_gm_body_red = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("sonic_gm_body02"); //Red Fur
		auto sonic_gm_body_green = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("sonic_gm_body03"); //Green Fur
		auto sonic_gm_body_pink = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("sonic_gm_body04"); //Pink Fur
		auto sonic_gm_body_black = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("sonic_gm_body05"); //Black Fur
		auto super_sonic_gm_body = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("super_sonic_gm_body");
		auto hyper_sonic_gm_body00 = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("hyper_sonic_gm_body00");
		auto hyper_sonic_gm_body01 = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("hyper_sonic_gm_body01");
		auto hyper_sonic_gm_body02 = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("hyper_sonic_gm_body02");
		auto hyper_sonic_gm_body03 = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("hyper_sonic_gm_body03");
		auto hyper_sonic_gm_body04 = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("hyper_sonic_gm_body04");
		auto hyper_sonic_gm_body05 = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("hyper_sonic_gm_body05");
		auto hyper_sonic_gm_body06 = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("hyper_sonic_gm_body06");
		auto chr_Sonic_spin_HD = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("chr_Sonic_spin_HD");
		auto chr_Sonic_spin02_HD = hh::mr::CMirageDatabaseWrapper(Sonic::CGameDocument::GetInstance()->m_pMember->m_spDatabase.get()).GetMaterialData("chr_Sonic_spin02_HD");

		if (strstr(element->m_spModel->m_TypeAndName.c_str(), "chr_Sonic_HD"))
		{
			element->m_MaterialMap.clear();
			switch (SelectSnSonMat)
			{
			case SnMatDefault:
				element->m_MaterialMap.emplace(sonic_gm_body_custom.get(), sonic_gm_body_custom);
				return;
				break;
			case SnMatOriginal:
				element->m_MaterialMap.emplace(sonic_gm_body_custom.get(), sonic_gm_body_original);
				return;						  
				break;
			case SnMatS4E2:
				element->m_MaterialMap.emplace(sonic_gm_body_custom.get(), sonic_gm_body_S4E2);
				return;
				break;
			case SnMatRed:
				element->m_MaterialMap.emplace(sonic_gm_body_custom.get(), sonic_gm_body_red);
				return;
				break;
			case SnMatGreen:
				element->m_MaterialMap.emplace(sonic_gm_body_custom.get(), sonic_gm_body_green);
				return;
				break;
			case SnMatPink:
				element->m_MaterialMap.emplace(sonic_gm_body_custom.get(), sonic_gm_body_pink);
				return;
				break;
			case SnMatBlack:
				element->m_MaterialMap.emplace(sonic_gm_body_custom.get(), sonic_gm_body_black);
				return;
				break;
			}
		}

		if (strstr(element->m_spModel->m_TypeAndName.c_str(), "chr_SuperSonic_HD"))
		{
			element->m_MaterialMap.clear();
			switch (SelectSsnSonMat)
			{
			case SsnMatDefault:
				element->m_MaterialMap.emplace(super_sonic_gm_body.get(), sonic_gm_body_custom);
				return;
				break;
			case SsnMatOriginal:
				element->m_MaterialMap.emplace(super_sonic_gm_body.get(), sonic_gm_body_original);
				return;
				break;
			case SsnMatRed:
				element->m_MaterialMap.emplace(super_sonic_gm_body.get(), sonic_gm_body_red);
				return;
				break;
			case SsnMatGreen:
				element->m_MaterialMap.emplace(super_sonic_gm_body.get(), sonic_gm_body_green);
				return;
				break;
			case SsnMatPink:
				element->m_MaterialMap.emplace(super_sonic_gm_body.get(), sonic_gm_body_pink);
				return;
				break;
			case SsnMatBlack:
				element->m_MaterialMap.emplace(super_sonic_gm_body.get(), sonic_gm_body_black);
				return;
				break;
			//case SsnMatHyper:
			//	switch (HyperFrameCycle)
			//	{
			//	case 0:
			//	case 1:
			//	case 2:
			//	case 3:
			//		element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body00);
			//		return;
			//		break;
			//	case 8:
			//	case 9:
			//	case 10:
			//	case 11:
			//		element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body01);
			//		return;
			//		break;
			//	case 16:
			//	case 17:
			//	case 18:
			//	case 19:
			//		element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body02);
			//		return;
			//		break;
			//	case 24:
			//	case 25:
			//	case 26:
			//	case 27:
			//		element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body03);
			//		return;
			//		break;
			//	case 32:
			//	case 33:
			//	case 34:
			//	case 35:
			//		element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body04);
			//		return;
			//		break;
			//	case 40:
			//	case 41:
			//	case 42:
			//	case 43:
			//		element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body05);
			//		return;
			//		break;
			//	default:
			//		element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body06);
			//		return;
			//		break;
			//	}
			//	break;
			}
		}

		if (strstr(element->m_spModel->m_TypeAndName.c_str(), "chr_Sonic_spin_HD"))
		{
			element->m_MaterialMap.clear();
			element->m_MaterialMap.emplace(chr_Sonic_spin_HD.get(), chr_Sonic_spin02_HD);
		}
	}
	else if (auto bundle = dynamic_cast<hh::mr::CBundle*>(renderable))
	{
		for (const auto& it : bundle->m_RenderableList)
			traverse(it.get());
	}
	else if (auto optimalBundle = dynamic_cast<hh::mr::COptimalBundle*>(renderable))
	{
		for (const auto it : optimalBundle->m_RenderableList)
			traverse(it);
	}
}

//void MaterialSelectionHandler(const boost::shared_ptr<hh::mr::CModelData>& model, std::vector<boost::shared_ptr<hh::mr::CMeshData>>& spMaterial)
//{
//	if (!model || !(model->m_Flags & hh::db::eDatabaseDataFlags_IsMadeOne))
//		return;
//
//	if (spMaterial.empty())
//		spMaterial.assign(model->m_OpaqueMeshes.begin(), model->m_OpaqueMeshes.end());
//
//	model->m_OpaqueMeshes.clear();
//
//	for (auto& mat : spMaterial)
//	{
//		auto matName = mat->m_MaterialName;
//
//		if (matName == "sonic_gm_body")
//		{
//			mat->m_MaterialName = "sonic_gm_body00";
//			printf("sonic_gm_body FOUND\n");
//		}
//			
//
//		if (matName != "sonic_gm_body")
//			model->m_OpaqueMeshes.push_back(mat);
//	}
//
//	model->m_NodeNum = model->m_OpaqueMeshes.size();
//}
=======
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
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
