#pragma once

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
	SBSuperForm,
	SBJumpball,
	SBOverflow06,
	SBOverflow07,
	SBOverflow08,
};

SelectShoeType SelectShoes = SelectShoeType::ShDefault;
SelectBodyType SelectBody = SelectBodyType::BdDefault;
SelectHeadType SelectHead = SelectHeadType::HeDefault;
SelectHandLType SelectHandL = SelectHandLType::HLDefault;
SelectHandRType SelectHandR = SelectHandRType::HRDefault;
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
	{ SBSnMaterial,			"SBSnMaterial" },
	{ SBSsnMaterial,		"SBSsnMaterial" },
	{ SBEyelids,			"SBEyelids" },
	{ SBSuperHead,			"SBSuperHead" },
	{ SBSuperForm,			"SBSuperForm" },
	{ SBJumpball,			"SBJumpball" },
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
	SnMatOriginal,
	SnMatCustom,
	SnMatS4E2,
	SnMatRed,
	SnMatGreen,
	SnMatPink,
	SnMatBlack,
};
enum SelectSsnSonMatType
{
	SsnMatOriginal,
	SsnMatCustom,
	SsnMatHyper,
	SsnMatRed,
	SsnMatGreen,
	SsnMatPink,
	SsnMatBlack,
	SsnMatPurple,
};
enum SelectSsnHeadType
{
	SsnFormDefault,
	SsnFormShadow,
	SsnFormUpward,
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
enum SelectJumpBallType
{
	JumpBallDefault,
	JumpBallSWA,
	JumpBallBetaSWA,
	JumpBallBAP,
	JumpBallLW,
	JumpBallForces,
	JumpBallSA1,
	JumpBallNoVFX,
	JumpBallNoBall,
};

SelectSnSonMatType SelectSnSonMat = SelectSnSonMatType::SnMatOriginal;
SelectSsnSonMatType SelectSsnSonMat = SelectSsnSonMatType::SsnMatOriginal;
SelectSsnHeadType SelectSsnHead = SelectSsnHeadType::SsnFormDefault;
SelectSsnFormType SelectSsnForm = SelectSsnFormType::SsnFormSuper;
SelectEyelidType SelectEyelid = SelectEyelidType::EyelidDefault;
SelectJumpBallType SelectJumpBall = SelectJumpBallType::JumpBallDefault;
bool NoBallJump = true;

int HyperFrameCycle = 0;
int SA1JumpballFrameCycle = 0;

bool m_spIsModelHide = false;
void MsgModelHide(int Enabled)
{
	if (Enabled == 1)
		m_spIsModelHide = true;
	else
		m_spIsModelHide = false;
}

void MsgWildFire(int Enabled);
void MsgJumpBall(int BallType);

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
		if (nodeName == "ShDefault_00")
			node->m_Visible = ((SelectShoes == ShDefault && (ShDefaultVariant == DefaultDefault) && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "ShDefault_01")
			node->m_Visible = ((SelectShoes == ShDefault && (ShDefaultVariant == DefaultLightS) && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Classic Shoes
		if (nodeName == "ShClassic")
			node->m_Visible = ((SelectShoes == ShClassic && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//SA1 Lightspeed Shoes
		if (nodeName == "ShSA1LightS")
			node->m_Visible = ((SelectShoes == ShSA1LightS && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//SA2 Soap Shoes
		if (nodeName == "ShSA2Soap_00")
			node->m_Visible = ((SelectShoes == ShSA2Soap && (ShSA2SoapVariant != SA2SoapRacing) && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "ShSA2Soap_01")
			node->m_Visible = ((SelectShoes == ShSA2Soap && (ShSA2SoapVariant == SA2SoapLightS) && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "ShSA2Soap_02")
			node->m_Visible = ((SelectShoes == ShSA2Soap && (ShSA2SoapVariant == SA2SoapRacing) && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Riders Shoes
		if (nodeName == "ShRiders")
			node->m_Visible = ((SelectShoes == ShRiders && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//06 Gem Shoes
		if (nodeName == "Sh06Gem_Default")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemDefault && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "Sh06Gem_Red")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemRed && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "Sh06Gem_Blue")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemBlue && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "Sh06Gem_Green")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemGreen && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "Sh06Gem_Purple")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemPurple && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "Sh06Gem_Sky")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemSky && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "Sh06Gem_White")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemWhite && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "Sh06Gem_Yellow")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemYellow && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Archie Shoes
		if (nodeName == "ShArchie")
			node->m_Visible = ((SelectShoes == ShArchie && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Unleashed Shoes
		if (nodeName == "ShSUAirboost")
			node->m_Visible = ((SelectShoes == ShSUAirboost && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "ShSULightS")
			node->m_Visible = ((SelectShoes == ShSULightS && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "ShSUStomp")
			node->m_Visible = ((SelectShoes == ShSUStomp && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "ShSUWallJmp")
			node->m_Visible = ((SelectShoes == ShSUWallJmp && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "ShSUWerehog")
			node->m_Visible = ((SelectShoes == ShSUWerehog && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "ShSUZoney")
			node->m_Visible = ((SelectShoes == ShSUZoney && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Simulator Shoes
		if (nodeName == "ShSimulator")
			node->m_Visible = ((SelectShoes == ShSimulator && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Link Boots
		if (nodeName == "ShLink")
			node->m_Visible = ((SelectShoes == ShLink && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Boom Shoes
		if (nodeName == "ShBoom")
			node->m_Visible = ((SelectShoes == ShBoom && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Puma Shoes
		if (nodeName == "ShPumas")
			node->m_Visible = ((SelectShoes == ShPumas && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Prime New Yoke Shoes
		if (nodeName == "ShPrmNewYoke")
			node->m_Visible = ((SelectShoes == ShPrmNewYoke && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Deluxe Shoes
		if (nodeName == "ShSFDeluxe")
			node->m_Visible = ((SelectShoes == ShSFDeluxe && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Korone Shoes
		if (nodeName == "ShSFKorone")
			node->m_Visible = ((SelectShoes == ShSFKorone && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "ShSFMonsterHunter")
			node->m_Visible = ((SelectShoes == ShSFMonsterHunter || SelectBody == BdSFMonsterHunter) && !m_spIsModelHide);

		/////Bodies//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Base Body
		if (nodeName == "BdDefault_00")
			node->m_Visible = (((SelectBody == BdDefault && BdDefaultVariant == false) ||
				(SelectBody == BdWildFire && BdWildFireVariant == false) ||
				(SelectBody == BdScarf && BdScarfVariant == false) ||
				(SelectBody == BdMovieSkin && BdMovieVariant == false)) && !m_spIsModelHide);
		if (nodeName == "BdDefault_01")
			node->m_Visible = (((SelectBody == BdDefault && BdDefaultVariant == true) ||
				(SelectBody == BdWildFire && BdWildFireVariant == true) ||
				(SelectBody == BdScarf && BdScarfVariant == true) ||
				(SelectBody == BdMovieSkin && BdMovieVariant == true)) && !m_spIsModelHide);

		//Racing Suit
		if (nodeName == "BdSA2Racing")
			node->m_Visible = ((SelectBody == BdSA2Racing) && !m_spIsModelHide);

		//Simulator
		if (nodeName == "BdSimulator")
			node->m_Visible = ((SelectBody == BdSimulator) && !m_spIsModelHide);

		//Link Tunic
		if (nodeName == "BdLink")
			node->m_Visible = ((SelectBody == BdLink) && !m_spIsModelHide);

		//Scarf
		if (nodeName == "BdScarf")
			node->m_Visible = ((SelectBody == BdScarf) && !m_spIsModelHide);

		//2020 Swim Vest
		if (nodeName == "Bd2020SwimVest")
			node->m_Visible = ((SelectBody == Bd2020SwimVest) && !m_spIsModelHide);

		//2020 Surf Suit
		if (nodeName == "Bd2020SurfSuit")
			node->m_Visible = ((SelectBody == Bd2020SurfSuit) && !m_spIsModelHide);

		//2020 Horse Shirt
		if (nodeName == "Bd2020Horse")
			node->m_Visible = ((SelectBody == Bd2020Horse) && !m_spIsModelHide);

		//Movie Quills
		if (nodeName == "BdMovieSkin")
			node->m_Visible = ((SelectBody == BdMovieSkin) && !m_spIsModelHide);
		if (nodeName == "HeDefault_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF)) && !m_spIsModelHide);
		if (nodeName == "HeDefault_00_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF)) && !m_spIsModelHide);
		if (nodeName == "HeDefault_01_Movie")
			node->m_Visible = false;
		if (nodeName == "HeBoom_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead == HeBoom)) && !m_spIsModelHide);
		if (nodeName == "HeLink_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead == HeLink)) && !m_spIsModelHide);
		if (nodeName == "HeSFMonsterHunterF_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead == HeSFMonsterHunterF)) && !m_spIsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "BdSFMonsterHunter")
			node->m_Visible = ((SelectBody == BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Monster Hunter Felyne
		if (nodeName == "BdSFMonsterHunterF")
			node->m_Visible = ((SelectBody == BdSFMonsterHunterF) && !m_spIsModelHide);

		//Frontiers Holiday Cheer Suit
		if (nodeName == "BdSFHoliday")
			node->m_Visible = ((SelectBody == BdSFHoliday) && !m_spIsModelHide);

		/////Heads//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Base Head
		if (nodeName == "HeDefault")
			node->m_Visible = ((!(SelectHead == HeSimulator && HeSimulatorVariant == false) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF) && (SelectHead != HeSFMonsterHunter)) && !m_spIsModelHide);
		
		//Super Heads
		if (nodeName == "HeDefault_00")
			node->m_Visible = ((!(SelectHead == HeSimulator && HeSimulatorVariant == false) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF) && (SelectHead != HeSFMonsterHunter)) && !m_spIsModelHide);
		if (nodeName == "HeDefault_01")
			node->m_Visible = false;

		//SA1 Sunglasses
		if (nodeName == "HeSA1Sunglass")
			node->m_Visible = ((SelectHead == HeSA1Sunglass) && !m_spIsModelHide);

		//Riders Glasses
		if (nodeName == "HeRiders")
			node->m_Visible = ((SelectHead == HeRiders) && !m_spIsModelHide);

		//Headphones
		if (nodeName == "HeHeadphones")
			node->m_Visible = ((SelectHead == HeHeadphones) && !m_spIsModelHide);

		//Simulator
		if (nodeName == "HeSimulator_00")
			node->m_Visible = ((SelectHead == HeSimulator && HeSimulatorVariant == false) && !m_spIsModelHide);
		if (nodeName == "HeSimulator_01")
			node->m_Visible = ((SelectHead == HeSimulator && HeSimulatorVariant == true) && !m_spIsModelHide);

		//Link Hat
		if (nodeName == "HeLink")
			node->m_Visible = ((SelectHead == HeLink) && !m_spIsModelHide);

		//Boom Spikes
		if (nodeName == "HeBoom")
			node->m_Visible = ((SelectHead == HeBoom) && !m_spIsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "HeSFMonsterHunter")
			node->m_Visible = ((SelectHead == HeSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Monster Hunter Felyne
		if (nodeName == "HeSFMonsterHunterF")
			node->m_Visible = ((SelectHead == HeSFMonsterHunterF) && !m_spIsModelHide);

		//Frontiers Holiday Cheer Suit
		if (nodeName == "HeSFHoliday")
			node->m_Visible = ((SelectHead == HeSFHoliday) && !m_spIsModelHide);

		/////Left Hands//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "HLDefault")
			node->m_Visible = ((((SelectHandL == HLDefault) ||
				(SelectHandL == HLSA1Crystal) ||
				(SelectHandL == HLSA2FlameRing) ||
				(SelectHandL == HL06Bounce) ||
				(SelectHandL == HLChip)) &&
				(SelectBody != BdSFMonsterHunter)) && !m_spIsModelHide);

		//SA1 Crystal Ring
		if (nodeName == "HLSA1Crystal")
			node->m_Visible = ((SelectHandL == HLSA1Crystal && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//SA2 Flame Ring
		if (nodeName == "HLSA2FlameRing")
			node->m_Visible = ((SelectHandL == HLSA2FlameRing && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//SA2 Racing Glove
		if (nodeName == "HLSA2Racing")
			node->m_Visible = ((SelectHandL == HLSA2Racing && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Riders Glove
		if (nodeName == "HLRiders")
			node->m_Visible = ((SelectHandL == HLRiders && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//06 Bounce Bracelet
		if (nodeName == "HL06Bounce")
			node->m_Visible = ((SelectHandL == HL06Bounce && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Chip Ring
		if (nodeName == "HLChip")
			node->m_Visible = ((SelectHandL == HLChip && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Simulator
		if (nodeName == "HLSimulator")
			node->m_Visible = ((SelectHandL == HLSimulator && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Link Glove
		if (nodeName == "HLLink")
			node->m_Visible = ((SelectHandL == HLLink && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Boom Glove
		if (nodeName == "HLBoom")
			node->m_Visible = ((SelectHandL == HLBoom && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Deluxe Glove
		if (nodeName == "HLSFDeluxe")
			node->m_Visible = ((SelectHandL == HLSFDeluxe && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Korone Glove
		if (nodeName == "HLSFKorone")
			node->m_Visible = ((SelectHandL == HLSFKorone && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "HLSFMonsterHunter")
			node->m_Visible = ((SelectHandL == HLSFMonsterHunter || SelectBody == BdSFMonsterHunter) && !m_spIsModelHide);

		/////Right Hands/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "HRDefault")
			node->m_Visible = ((((SelectHandR == HRDefault) ||
				(SelectHandR == HRSA2Bounce) ||
				(SelectHandR == HR06Homing) ||
				(SelectHandR == HRSecretRing)) && 
				(SelectBody != BdSFMonsterHunter)) && !m_spIsModelHide);

		//SA2 Bounce Bracelet
		if (nodeName == "HRSA2Bounce_00")
			node->m_Visible = ((SelectHandR == HRSA2Bounce) && (SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "HRSA2Bounce_01")
			node->m_Visible = (((SelectHandR == HRSA2Bounce && HRSA2BounceVariant == true) && (SelectBody != BdSFMonsterHunter)) && !m_spIsModelHide);

		//SA2 Racing Glove
		if (nodeName == "HRSA2Racing")
			node->m_Visible = ((SelectHandR == HRSA2Racing && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Riders Glove
		if (nodeName == "HRRiders")
			node->m_Visible = (((SelectHandR == HRRiders || SelectHandR == HRRidersArkCosmos) && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);
		if (nodeName == "HRRidersArkCosmos")
			node->m_Visible = ((SelectHandR == HRRidersArkCosmos && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//06 Homing Smash Bracelet
		if (nodeName == "HR06Homing")
			node->m_Visible = ((SelectHandR == HR06Homing && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Secret Ring
		if (nodeName == "HRSecretRing")
			node->m_Visible = ((SelectHandR == HRSecretRing && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Gauntlet
		if (nodeName == "HRGauntlet")
			node->m_Visible = ((SelectHandR == HRGauntlet && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Simulator
		if (nodeName == "HRSimulator")
			node->m_Visible = ((SelectHandR == HRSimulator && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Link Glove
		if (nodeName == "HRLink")
			node->m_Visible = ((SelectHandR == HRLink && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Boom Glove
		if (nodeName == "HRBoom")
			node->m_Visible = ((SelectHandR == HRBoom && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Deluxe Glove
		if (nodeName == "HRSFDeluxe")
			node->m_Visible = ((SelectHandR == HRSFDeluxe && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Korone Glove
		if (nodeName == "HRSFKorone")
			node->m_Visible = ((SelectHandR == HRSFKorone && SelectBody != BdSFMonsterHunter) && !m_spIsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "HRSFMonsterHunter")
			node->m_Visible = ((SelectHandR == HRSFMonsterHunter || SelectBody == BdSFMonsterHunter) && !m_spIsModelHide);

		/////Misc/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (nodeName == "EyeDefault")
			node->m_Visible = ((SelectEyelid == EyelidDefault) && !m_spIsModelHide);
		if (nodeName == "EyeLash")
			node->m_Visible = ((SelectEyelid == EyelidLashes) && !m_spIsModelHide);
		if (nodeName == "EyeSkin")
			node->m_Visible = ((SelectEyelid == EyelidSkin) && !m_spIsModelHide);

		//printf("%s\n", nodeName.c_str());

		if (node->m_Visible)
			model->m_NodeGroupModels.push_back(node);
	}

	//Send Wildfire VFX
	if (SelectBody == BdWildFire)
		MsgWildFire(1);
	else
		MsgWildFire(0);

	MsgJumpBall(SelectJumpBall);

	model->m_NodeGroupModelNum = model->m_NodeGroupModels.size();

	if (HyperFrameCycle >= 59)
		HyperFrameCycle = 0;
	else
		HyperFrameCycle++;

	if (SA1JumpballFrameCycle >= 29)
		SA1JumpballFrameCycle = 0;
	else
		SA1JumpballFrameCycle++;

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

		if (strstr(element->m_spModel->m_TypeAndName.c_str(), "chr_Sonic_HD"))
		{
			element->m_MaterialMap.clear();
			switch (SelectSnSonMat)
			{
			case SnMatOriginal:
				element->m_MaterialMap.emplace(sonic_gm_body_custom.get(), sonic_gm_body_original);
				return;						  
				break;
			case SnMatCustom:
				element->m_MaterialMap.emplace(sonic_gm_body_custom.get(), sonic_gm_body_custom);
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
			case SsnMatOriginal:
				element->m_MaterialMap.emplace(super_sonic_gm_body.get(), sonic_gm_body_original);
				return;
				break;
			case SsnMatCustom:
				element->m_MaterialMap.emplace(super_sonic_gm_body.get(), sonic_gm_body_custom);
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
			case SsnMatHyper:
				switch (HyperFrameCycle)
				{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
					element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body00);
					return;
					break;
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
					element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body01);
					return;
					break;
				case 20:
				case 21:
				case 22:
				case 23:
				case 24:
					element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body02);
					return;
					break;
				case 30:
				case 31:
				case 32:
				case 33:
				case 34:
					element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body03);
					return;
					break;
				case 40:
				case 41:
				case 42:
				case 43:
				case 44:
					element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body04);
					return;
					break;
				case 50:
				case 51:
				case 52:
				case 53:
				case 54:
					element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body05);
					return;
					break;
				default:
					element->m_MaterialMap.emplace(super_sonic_gm_body.get(), hyper_sonic_gm_body06);
					return;
					break;
				}
				break;
			}
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