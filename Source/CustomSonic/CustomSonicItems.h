#pragma once
#include <fstream>
#include <json/json.hpp>
using nlohmann::json;

////------Item Setup------////

struct ItemDataHead
{
	std::string name;
	int altcount;
	int altselect;
	int hideflags;
};
static std::vector<ItemDataHead> s_ItemDataHead;

struct ItemDataBody
{
	std::string name;
	int altcount;
	int altselect;
	int hideflags;
};
static std::vector<ItemDataBody> s_ItemDataBody;

struct ItemDataShoes
{
	std::string name;
	int altcount;
	int altselect;
	int hideflags;
};
static std::vector<ItemDataShoes> s_ItemDataShoes;

struct ItemDataHandR
{
	std::string name;
	int altcount;
	int altselect;
	int hideflags;
};
static std::vector<ItemDataHandR> s_ItemDataHandR;

struct ItemDataHandL
{
	std::string name;
	int altcount;
	int altselect;
	int hideflags;
};
static std::vector<ItemDataHandL> s_ItemDataHandL;

int SelectHeadData = 0;
int SelectBodyData = 0;
int SelectShoesData = 0;
int SelectHandRData = 0;
int SelectHandLData = 0;

////------Hardcoded Item Setup------////

//enum SelectShoeType
//{
//	ShDefault, //Has Lightspeed variant
//	ShClassic,
//	ShSA1,
//	ShSA1LightS,
//	ShSA2Beta, //Has Lightspeed variant
//	ShSA2Soap, //Has Lightspeed and racing variants
//	ShRiders,
//	Sh06,
//	Sh06Gem, //has 7 color alts
//	ShArchie,
//	ShExCal,  //TBA
//	ShSUAirboost,
//	ShSULightS,
//	ShSUStomp,
//	ShSUWallJmp,
//	ShSUWerehog,
//	ShSUZoney,
//	ShSimulator,
//	ShLink,
//	ShBoom,
//	ShSpeedSlugger, //TBA
//	ShSpeedPirate, //TBA
//	ShSmash, //TBA
//	Sh2020Running, //TBA
//	Sh2020Horse, //TBA
//	Sh2020Karate, //TBA
//	ShPumas,
//	ShSFDeluxe,
//	ShSFKorone,
//	ShSFMonsterHunter,
//	ShPrmNewYoke, //TBA
//	ShPrmBoscageMaze, //TBA
//	ShPrmNoPlace, //TBA
//	ShOverflow00,
//	ShOverflow01,
//	ShOverflow02,
//};
//enum SelectBodyType
//{
//	BdDefault, //Blue Arms
//	BdSA2Racing,
//	BdWildFire, //Blue Arms
//	BdExCal,  //TBA
//	BdSimulator,
//	BdLink,
//	BdScarf, //Blue Arms
//	BdSpeedSlugger, //TBA
//	BdSpeedPirate, //TBA
//	Bd2020SwimVest,
//	Bd2020SurfSuit,
//	Bd2020Horse,
//	Bd2020Running, //TBA
//	Bd2020Fence, //TBA
//	BdMovieSkin, //Blue Arms
//	BdSFMonsterHunter,
//	BdSFMonsterHunterF,
//	BdSFHoliday,
//	BdOverflow00,
//	BdOverflow01,
//	BdOverflow02,
//};
//enum SelectHeadType
//{
//	HeDefault,
//	HeSA1Sunglass,
//	HeRiders,
//	HeHeadphones,
//	HeExCal,  //TBA
//	HeSimulator,
//	HeLink,
//	HeBoom,
//	HeSpeedSlugger, //TBA
//	HeSpeedPirate, //TBA
//	HeSFMonsterHunter,
//	HeSFMonsterHunterF,
//	HeSFHoliday,
//	HeOverflow00,
//	HeOverflow01,
//	HeOverflow02,
//};
//enum SelectHandLType
//{
//	HLDefault,
//	HLSA1Crystal,
//	HLSA2FlameRing,
//	HLSA2Racing,
//	HLRiders,
//	HL06Bounce,
//	HLExCal,  //TBA
//	HLChip,
//	HLSimulator,
//	HLLink,
//	HLBoom,
//	HLSpeedSlugger, //TBA
//	HLSpeedPirate, //TBA
//	HLSmash, //TBA
//	HL2020Karate, //TBA
//	HLSFDeluxe,
//	HLSFKorone,
//	HLSFMonsterHunter,
//	HLPrmNewYoke, //TBA
//	HLPrmBoscageMaze, //TBA
//	HLPrmNoPlace, //TBA
//	HLOverflow00,
//	HLOverflow01,
//	HLOverflow02,
//};
//enum SelectHandRType
//{
//	HRDefault,
//	HRSA2Bounce,
//	HRSA2Racing,
//	HRRiders,
//	HRRidersArkCosmos,
//	HR06Homing,
//	HRSecretRing,
//	HRGauntlet,
//	HRExCal,  //TBA
//	HRSimulator,
//	HRLink,
//	HRBoom,
//	HRSpeedSlugger, //TBA
//	HRSpeedPirate, //TBA
//	HRSmash, //TBA
//	HR2020Karate, //TBA
//	HRSFDeluxe,
//	HRSFKorone,
//	HRSFMonsterHunter,
//	HRPrmNewYoke, //TBA
//	HRPrmBoscageMaze, //TBA
//	HRPrmNoPlace, //TBA
//	HROverflow00,
//	HROverflow01,
//	HROverflow02,
//};
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

//SelectShoeType SelectShoes = SelectShoeType::ShDefault;
//SelectBodyType SelectBody = SelectBodyType::BdDefault;
//SelectHeadType SelectHead = SelectHeadType::HeDefault;
//SelectHandLType SelectHandL = SelectHandLType::HLDefault;
//SelectHandRType SelectHandR = SelectHandRType::HRDefault;
SelectSonicBodyType SelectSonicBody = SelectSonicBodyType::SBSnMaterial;

//static std::map<int, const char*> MAP_FILE_SHOE =
//{
//	{ ShDefault,			"ShDefault"},
//	{ ShClassic,			"ShClassic" },
//	{ ShSA1,				"ShSA1" },
//	{ ShSA1LightS,			"ShSA1LightS" },
//	{ ShSA2Beta,			"ShSA2Beta" },
//	{ ShSA2Soap,			"ShSA2Soap" },
//	{ ShRiders,				"ShRiders" },
//	{ Sh06,					"Sh06" },
//	{ Sh06Gem,				"Sh06Gem" },
//	{ ShArchie,				"ShArchie" },
//	{ ShExCal,				"ShExCal" },
//	{ ShSUAirboost,			"ShSUAirboost" },
//	{ ShSULightS,			"ShSULightS" },
//	{ ShSUStomp,			"ShSUStomp" },
//	{ ShSUWallJmp,			"ShSUWallJmp" },
//	{ ShSUWerehog,			"ShSUWerehog" },
//	{ ShSUZoney,			"ShSUZoney" },
//	{ ShSimulator,			"ShSimulator" },
//	{ ShLink,				"ShLink" },
//	{ ShBoom,				"ShBoom" },
//	{ ShSpeedSlugger,		"ShSpeedSlugger" },
//	{ ShSpeedPirate,		"ShSpeedPirate" },
//	{ ShSmash,				"ShSmash" },
//	{ Sh2020Running,		"Sh2020Running" },
//	{ Sh2020Horse,			"Sh2020Horse" },
//	{ Sh2020Karate,			"Sh2020Karate" },
//	{ ShPumas,				"ShPumas" },
//	{ ShSFDeluxe,			"ShSFDeluxe" },
//	{ ShSFKorone,			"ShSFKorone" },
//	{ ShSFMonsterHunter,	"ShSFMonsterHunter" },
//	{ ShPrmNewYoke,			"ShPrmNewYoke" },
//	{ ShPrmBoscageMaze,		"ShPrmBoscageMaze" },
//	{ ShPrmNoPlace,			"ShPrmNoPlace" },
//	{ ShOverflow00,			"Null" },
//	{ ShOverflow01,			"Null" },
//	{ ShOverflow02,			"Null" },
//};
//static std::map<int, const char*> MAP_FILE_BODY =
//{
//	{ BdDefault,			"BdDefault"},
//	{ BdSA2Racing,			"BdSA2Racing" },
//	{ BdWildFire,			"BdWildFire" },
//	{ BdExCal,				"BdExCal" },
//	{ BdSimulator,			"BdSimulator" },
//	{ BdLink,				"BdLink" },
//	{ BdScarf,				"BdScarf" },
//	{ BdSpeedSlugger,		"BdSpeedSlugger" },
//	{ BdSpeedPirate,		"BdSpeedPirate" },
//	{ Bd2020SwimVest,		"Bd2020SwimVest" },
//	{ Bd2020SurfSuit,		"Bd2020SurfSuit" },
//	{ Bd2020Horse,			"Bd2020Horse" },
//	{ Bd2020Running,		"Bd2020Running" },
//	{ Bd2020Fence,			"Bd2020Fence" },
//	{ BdMovieSkin,			"BdMovieSkin" },
//	{ BdSFMonsterHunter,	"BdSFMonsterHunter" },
//	{ BdSFMonsterHunterF,	"BdSFMonsterHunterF" },
//	{ BdSFHoliday,			"BdSFHoliday" },
//	{ BdOverflow00,			"Null" },
//	{ BdOverflow01,			"Null" },
//	{ BdOverflow02,			"Null" },
//};
//static std::map<int, const char*> MAP_FILE_HEAD =
//{
//	{ HeDefault,			"HeDefault"},
//	{ HeSA1Sunglass,		"HeSA1Sunglass" },
//	{ HeRiders,				"HeRiders" },
//	{ HeHeadphones,			"HeHeadphones" },
//	{ HeExCal,				"HeExCal" },
//	{ HeSimulator,			"HeSimulator" },
//	{ HeLink,				"HeLink" },
//	{ HeBoom,				"HeBoom" },
//	{ HeSpeedSlugger,		"HeSpeedSlugger" },
//	{ HeSpeedPirate,		"HeSpeedPirate" },
//	{ HeSFMonsterHunter,	"HeSFMonsterHunter" },
//	{ HeSFMonsterHunterF,	"HeSFMonsterHunterF" },
//	{ HeSFHoliday,			"HeSFHoliday" },
//	{ HeOverflow00,			"Null" },
//	{ HeOverflow01,			"Null" },
//	{ HeOverflow02,			"Null" },
//};
//static std::map<int, const char*> MAP_FILE_HANDL =
//{
//	{ HLDefault,			"HLDefault"},
//	{ HLSA1Crystal,			"HLSA1Crystal"},
//	{ HLSA2FlameRing,		"HLSA2FlameRing"},
//	{ HLSA2Racing,			"HLSA2Racing"},
//	{ HLRiders,				"HLRiders"},
//	{ HL06Bounce,			"HL06Bounce"},
//	{ HLExCal,				"HLExCal"},
//	{ HLChip,				"HLChip"},
//	{ HLSimulator, 			"HLSimulator"},
//	{ HLLink,				"HLLink"},
//	{ HLBoom,				"HLBoom"},
//	{ HLSpeedSlugger, 		"HLSpeedSlugger"},
//	{ HLSpeedPirate, 		"HLSpeedPirate"},
//	{ HLSmash, 				"HLSmash"},
//	{ HL2020Karate, 		"HL2020Karate"},
//	{ HLSFDeluxe, 			"HLSFDeluxe"},
//	{ HLSFKorone,			"HLSFKorone"},
//	{ HLSFMonsterHunter, 	"HLSFMonsterHunter"},
//	{ HLPrmNewYoke, 		"HLPrmNewYoke"},
//	{ HLPrmBoscageMaze, 	"HLPrmBoscageMaze"},
//	{ HLPrmNoPlace, 		"HLPrmNoPlace"},
//	{ HLOverflow00,			"Null" },
//	{ HLOverflow01,			"Null" },
//	{ HLOverflow02,			"Null" },
//};
//static std::map<int, const char*> MAP_FILE_HANDR =
//{
//	{ HRDefault,			"HRDefault"},
//	{ HRSA2Bounce,			"HRSA2Bounce"},
//	{ HRSA2Racing,			"HRSA2Racing"},
//	{ HRRiders,				"HRRiders"},
//	{ HRRidersArkCosmos,	"HRRidersArkCosmos"},
//	{ HR06Homing,			"HR06Homing"},
//	{ HRSecretRing,			"HRSecretRing"},
//	{ HRGauntlet,			"HRGauntlet"},
//	{ HRExCal,				"HRExCal"},
//	{ HRSimulator, 			"HRSimulator"},
//	{ HRLink,				"HRLink"},
//	{ HRBoom,				"HRBoom"},
//	{ HRSpeedSlugger, 		"HRSpeedSlugger"},
//	{ HRSpeedPirate, 		"HRSpeedPirate"},
//	{ HRSmash, 				"HRSmash"},
//	{ HR2020Karate, 		"HR2020Karate"},
//	{ HRSFDeluxe, 			"HRSFDeluxe"},
//	{ HRSFKorone,			"HRSFKorone"},
//	{ HRSFMonsterHunter, 	"HRSFMonsterHunter"},
//	{ HRPrmNewYoke, 		"HRPrmNewYoke"},
//	{ HRPrmBoscageMaze, 	"HRPrmBoscageMaze"},
//	{ HRPrmNoPlace, 		"HRPrmNoPlace"},
//	{ HROverflow00,			"Null" },
//	{ HROverflow01,			"Null" },
//	{ HROverflow02,			"Null" },
//};
static std::map<int, const char*> MAP_FILE_SONICBODY =
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
	JumpBallSA2,
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
bool isRenderableCreated = false;
bool isLoadModel = true;

bool m_IsModelHide = false;
void MsgModelHide(bool Enabled)
{
	m_IsModelHide = Enabled;
}

void MsgWildFire(int Enabled);
void MsgJumpBall(int BallType);

const char* CModelHeadString(char* result)
{
	auto mapChar = s_ItemDataHead[SelectHeadData].name;
	auto mapAltCount = s_ItemDataHead[SelectHeadData].altcount;
	auto mapAltSelect = s_ItemDataHead[SelectHeadData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "chr_Sn_%s", mapChar.c_str());

	return result;
}
const char* CModelBodyString(char* result)
{
	auto mapChar = s_ItemDataBody[SelectBodyData].name;
	auto mapAltCount = s_ItemDataBody[SelectBodyData].altcount;
	auto mapAltSelect = s_ItemDataBody[SelectBodyData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "chr_Sn_%s", mapChar.c_str());

	return result;
}
const char* CModelHandLString(char* result)
{
	auto mapChar = s_ItemDataHandL[SelectHandLData].name;
	auto mapAltCount = s_ItemDataHandL[SelectHandLData].altcount;
	auto mapAltSelect = s_ItemDataHandL[SelectHandLData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "chr_Sn_%s", mapChar.c_str());

	return result;
}
const char* CModelHandRString(char* result)
{
	auto mapChar = s_ItemDataHandR[SelectHandRData].name;
	auto mapAltCount = s_ItemDataHandR[SelectHandRData].altcount;
	auto mapAltSelect = s_ItemDataHandR[SelectHandRData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "chr_Sn_%s", mapChar.c_str());

	return result;
}
const char* CModelShoeString(char* result)
{
	auto mapChar = s_ItemDataShoes[SelectShoesData].name;
	auto mapAltCount = s_ItemDataShoes[SelectShoesData].altcount;
	auto mapAltSelect = s_ItemDataShoes[SelectShoesData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "chr_Sn_%s", mapChar.c_str());

	return result;
}
const char* CModelEyelidString(char* result)
{
	const char* texExtVar = "";
	const char* texExt0 = "EyeDefault";
	const char* texExt1 = "EyeLash";
	const char* texExt2 = "EyeSkin";

	if (SelectEyelid == EyelidDefault)
		texExtVar = texExt0;
	else if (SelectEyelid == EyelidLashes)
		texExtVar = texExt1;
	else if (SelectEyelid == EyelidSkin)
		texExtVar = texExt2;

	sprintf(result, "chr_Sn_%s", texExtVar);
	//printf("chr_Sn_%s%s\n", texExtVar);
	return result;
}

const char* CMaterialBodyString(char* result)
{
	const char* texExtVar = "chr_sn_body_original";
	const char* texExtOG = "chr_sn_body_original"; //OG
	const char* texExtCustom = "chr_sn_body_custom"; //Custom
	const char* texExtS4E2 = "chr_sn_body_s4e2"; //S4E2
	const char* texExtRed = "chr_sn_body_red"; //Red
	const char* texExtGreen = "chr_sn_body_green"; //Green
	const char* texExtPink = "chr_sn_body_pink"; //Pink
	const char* texExtBlack = "chr_sn_body_black"; //Black

	switch (SelectSnSonMat)
	{
	case SnMatOriginal:
		texExtVar = texExtOG;
		break;
	case SnMatCustom:
		texExtVar = texExtCustom;
		break;
	case SnMatS4E2:
		texExtVar = texExtS4E2;
		break;
	case SnMatRed:
		texExtVar = texExtRed;
		break;
	case SnMatGreen:
		texExtVar = texExtGreen;
		break;
	case SnMatPink:
		texExtVar = texExtPink;
		break;
	case SnMatBlack:
		texExtVar = texExtBlack;
		break;
	}

	sprintf(result, "%s", texExtVar);
	//printf("%s%s\n", mapChar, texExtVar);
	return result;
}

const char* ArchiveHeadString(char* result)
{
	auto mapChar = s_ItemDataHead[SelectHeadData].name;
	auto mapAltCount = s_ItemDataHead[SelectHeadData].altcount;
	auto mapAltSelect = s_ItemDataHead[SelectHeadData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "Customize/ctp_Head/%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "Customize/ctp_Head/%s", mapChar.c_str());

	return result;
}
const char* ArchiveBodyString(char* result)
{
	auto mapChar = s_ItemDataBody[SelectBodyData].name;
	auto mapAltCount = s_ItemDataBody[SelectBodyData].altcount;
	auto mapAltSelect = s_ItemDataBody[SelectBodyData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "Customize/ctp_Body/%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "Customize/ctp_Body/%s", mapChar.c_str());

	return result;
}
const char* ArchiveShoeString(char* result)
{
	auto mapChar = s_ItemDataShoes[SelectShoesData].name;
	auto mapAltCount = s_ItemDataShoes[SelectShoesData].altcount;
	auto mapAltSelect = s_ItemDataShoes[SelectShoesData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "Customize/ctp_Shoes/%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "Customize/ctp_Shoes/%s", mapChar.c_str());

	return result;
}
const char* ArchiveHandRString(char* result)
{
	auto mapChar = s_ItemDataHandR[SelectHandRData].name;
	auto mapAltCount = s_ItemDataHandR[SelectHandRData].altcount;
	auto mapAltSelect = s_ItemDataHandR[SelectHandRData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "Customize/ctp_Hand_R/%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "Customize/ctp_Hand_R/%s", mapChar.c_str());

	return result;
}
const char* ArchiveHandLString(char* result)
{
	auto mapChar = s_ItemDataHandL[SelectHandLData].name;
	auto mapAltCount = s_ItemDataHandL[SelectHandLData].altcount;
	auto mapAltSelect = s_ItemDataHandL[SelectHandLData].altselect;

	if (mapAltCount >= 1)
		sprintf(result, "Customize/ctp_Hand_L/%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "Customize/ctp_Hand_L/%s", mapChar.c_str());

	return result;
}
const char* ArchiveEyelidString(char* result)
{
	const char* texExtVar = "";
	const char* texExt0 = "EyeDefault";
	const char* texExt1 = "EyeLash";
	const char* texExt2 = "EyeSkin";

	if (SelectEyelid == EyelidDefault)
		texExtVar = texExt0;
	else if (SelectEyelid == EyelidLashes)
		texExtVar = texExt1;
	else if (SelectEyelid == EyelidSkin)
		texExtVar = texExt2;

	sprintf(result, "Customize/bdy_Eyelid/%s", texExtVar);
	//printf("%s%s\n", mapChar, texExtVar);
	return result;
}

//////Sonic Renderable
class CustomizeSonicRenderable : public Sonic::CGameObject3D
{
public:
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnHead;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnBody;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnShoes;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnHandR;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnHandL;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnEyelid;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArHeadMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArBodyMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArShoesMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArHandRMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArHandLMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArEyelidMdlData;
	boost::shared_ptr<Hedgehog::Database::CDatabase> m_spArDatabase;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArHeadMatData;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArHeadReplaceMatData;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArBodyMatData;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArBodyReplaceMatData;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArEyelidMatData;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArEyelidReplaceMatData;

	struct arDataStruct
	{
		boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArModelData;
		boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArMaterialData;
		boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArReplaceMaterialData;
	};

	arDataStruct loadArchiveDatabase(Hedgehog::Base::CSharedString archiveName, Hedgehog::Base::CSharedString modelName, Hedgehog::Base::CSharedString materialName, Hedgehog::Base::CSharedString materialReplaceName)
	{
		arDataStruct arDataStruct;

		m_spArDatabase = Hedgehog::Database::CDatabase::CreateDatabase();
		auto& loader = Sonic::CApplicationDocument::GetInstance()->m_pMember->m_spDatabaseLoader;

		loader->CreateArchiveList(
			archiveName + ".ar",
			archiveName + ".arl",
			{ 200, 5 });

		loader->LoadArchiveList(m_spArDatabase, archiveName + ".arl");

		loader->LoadArchive(m_spArDatabase, archiveName + ".ar", { -10, 5 }, false, false);

		Hedgehog::Mirage::CMirageDatabaseWrapper wrapper(m_spArDatabase.get());

		arDataStruct.m_spArModelData = wrapper.GetModelData(modelName);
		arDataStruct.m_spArMaterialData = wrapper.GetMaterialData(materialName);
		arDataStruct.m_spArReplaceMaterialData = wrapper.GetMaterialData(materialReplaceName);

		return arDataStruct;
	}

	//////Custom Funcs
	void addCustomRenderModel(boost::shared_ptr<hh::mr::CSingleElement>& m_spElement, boost::shared_ptr<Hedgehog::Mirage::CModelData> modelData)
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));

		////Setup Model
		if (!modelData || !modelData->IsMadeAll())
			return;

		m_spElement = boost::make_shared<hh::mr::CSingleElement>(modelData);
		AddRenderable("Player", m_spElement, true);

		////Attach renderable to Sonic
		m_spElement->BindMatrixNode(context->m_spMatrixNode);
		m_spElement->BindPose(pPlayer->m_spCharacterModel->m_spInstanceInfo->m_spPose);
	}

	void addCustomRenderMaterial(boost::shared_ptr<hh::mr::CSingleElement>& m_spElement, boost::shared_ptr<Hedgehog::Mirage::CMaterialData> materialData, boost::shared_ptr<Hedgehog::Mirage::CMaterialData> materialReplaceData)
	{
		if (!materialData || !materialData->IsMadeAll() || !materialReplaceData || !materialReplaceData->IsMadeAll())
			return;

		m_spElement->m_MaterialMap.emplace(materialData.get(), materialReplaceData);
	}

	void loadCustomRenderModel()
	{
		char ShBuffer[256];
		char BdBuffer[256];
		char HeBuffer[256];
		char HLBuffer[256];
		char HRBuffer[256];
		char EyeBuffer[256];
		arDataStruct arHeadData;
		arDataStruct arBodyData;
		arDataStruct arEyelidData;

		if (isLoadModel)
		{
			arHeadData = loadArchiveDatabase(ArchiveHeadString(HeBuffer), CModelHeadString(HeBuffer), "chr_sn_body_original", CMaterialBodyString(HeBuffer));
			arBodyData = loadArchiveDatabase(ArchiveBodyString(BdBuffer), CModelBodyString(BdBuffer), "chr_sn_body_original", CMaterialBodyString(BdBuffer));
			arEyelidData = loadArchiveDatabase(ArchiveEyelidString(HeBuffer), CModelEyelidString(HeBuffer), "chr_sn_body_original", CMaterialBodyString(HeBuffer));

			m_spArShoesMdlData = loadArchiveDatabase(ArchiveShoeString(ShBuffer), CModelShoeString(ShBuffer), "", "").m_spArModelData;
			m_spArHandRMdlData = loadArchiveDatabase(ArchiveHandRString(HRBuffer), CModelHandRString(HRBuffer), "", "").m_spArModelData;
			m_spArHandLMdlData = loadArchiveDatabase(ArchiveHandLString(HLBuffer), CModelHandLString(HLBuffer), "", "").m_spArModelData;

			m_spArHeadMdlData = arHeadData.m_spArModelData;
			m_spArHeadMatData = arHeadData.m_spArMaterialData;
			m_spArHeadReplaceMatData = arHeadData.m_spArReplaceMaterialData;

			m_spArBodyMdlData = arBodyData.m_spArModelData;
			m_spArBodyMatData = arBodyData.m_spArMaterialData;
			m_spArBodyReplaceMatData = arBodyData.m_spArReplaceMaterialData;

			m_spArEyelidMdlData = arEyelidData.m_spArModelData;
			m_spArEyelidMatData = arEyelidData.m_spArMaterialData;
			m_spArEyelidReplaceMatData = arEyelidData.m_spArReplaceMaterialData;

			RemoveRenderables();
			isLoadModel = false;
		}

		if (m_spArHeadMdlData != nullptr && m_spArHeadMdlData->IsMadeAll())
		{
			addCustomRenderModel(m_spElementSnHead, m_spArHeadMdlData);
			m_spArHeadMdlData = nullptr;
		}
		if (m_spArBodyMdlData != nullptr && m_spArBodyMdlData->IsMadeAll())
		{
			addCustomRenderModel(m_spElementSnBody, m_spArBodyMdlData);
			m_spArBodyMdlData = nullptr;
		}
		if (m_spArShoesMdlData != nullptr && m_spArShoesMdlData->IsMadeAll())
		{
			addCustomRenderModel(m_spElementSnShoes, m_spArShoesMdlData);
			m_spArShoesMdlData = nullptr;
		}
		if (m_spArHandRMdlData != nullptr && m_spArHandRMdlData->IsMadeAll())
		{
			addCustomRenderModel(m_spElementSnHandR, m_spArHandRMdlData);
			m_spArHandRMdlData = nullptr;
		}
		if (m_spArHandLMdlData != nullptr && m_spArHandLMdlData->IsMadeAll())
		{
			addCustomRenderModel(m_spElementSnHandL, m_spArHandLMdlData);
			m_spArHandLMdlData = nullptr;
		}
		if (m_spArEyelidMdlData != nullptr && m_spArEyelidMdlData->IsMadeAll())
		{
			addCustomRenderModel(m_spElementSnEyelid, m_spArEyelidMdlData);
			m_spArEyelidMdlData = nullptr;
		}
		if (m_spArHeadMatData != nullptr && m_spArHeadMatData->IsMadeAll() && m_spArHeadReplaceMatData != nullptr && m_spArHeadReplaceMatData->IsMadeAll())
		{
			addCustomRenderMaterial(m_spElementSnHead, m_spArHeadMatData, m_spArHeadReplaceMatData);
			m_spArHeadMatData = nullptr;
			m_spArHeadReplaceMatData = nullptr;
		}
		if (m_spArBodyMatData != nullptr && m_spArBodyMatData->IsMadeAll() && m_spArBodyReplaceMatData != nullptr && m_spArBodyReplaceMatData->IsMadeAll())
		{
			addCustomRenderMaterial(m_spElementSnBody, m_spArBodyMatData, m_spArBodyReplaceMatData);
			m_spArBodyMatData = nullptr;
			m_spArBodyReplaceMatData = nullptr;
		}
		if (m_spArEyelidMatData != nullptr && m_spArEyelidMatData->IsMadeAll() && m_spArEyelidReplaceMatData != nullptr && m_spArEyelidReplaceMatData->IsMadeAll())
		{
			addCustomRenderMaterial(m_spElementSnEyelid, m_spArEyelidMatData, m_spArEyelidReplaceMatData);
			m_spArEyelidMatData = nullptr;
			m_spArEyelidReplaceMatData = nullptr;
		}

	}

	//////Renderable Funcs
	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder,
		Sonic::CGameDocument* in_pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		in_pGameDocument->AddUpdateUnit("b", this);
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& in_rUpdateInfo) override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
		auto pPlayer_Invisible = (pPlayer->m_spCharacterModel->m_spInstanceInfo->m_Flags & Hedgehog::Mirage::eInstanceInfoFlags_Invisible) != 0;
		auto in_spDatabase = GetGameDocument()->m_pMember->m_spDatabase;

		if (m_spElementSnHead != nullptr)
		{
			m_spElementSnHead->m_Enabled = !pPlayer_Invisible;
		}
		if (m_spElementSnBody != nullptr)
		{
			m_spElementSnBody->m_Enabled = !pPlayer_Invisible;
		}
		if (m_spElementSnShoes != nullptr)
		{
			m_spElementSnShoes->m_Enabled = !pPlayer_Invisible;
		}
		if (m_spElementSnHandR != nullptr)
		{
			m_spElementSnHandR->m_Enabled = !pPlayer_Invisible;
		}
		if (m_spElementSnHandL != nullptr)
		{
			m_spElementSnHandL->m_Enabled = !pPlayer_Invisible;
		}
		if (m_spElementSnEyelid != nullptr)
		{
			m_spElementSnEyelid->m_Enabled = !pPlayer_Invisible;
		}

		auto input = Sonic::CInputState::GetInstance()->GetPadState();
		bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
		bool PressedLeft = input.IsTapped(Sonic::eKeyState_DpadLeft);
		bool PressedRight = input.IsTapped(Sonic::eKeyState_DpadRight);
		bool PressedUp = input.IsTapped(Sonic::eKeyState_DpadUp);
		bool PressedDown = input.IsTapped(Sonic::eKeyState_DpadDown);

		loadCustomRenderModel();

		//if (PressedY)
		//{
		//	RemoveRenderables();
		//	addCustomRenderModel(CModelHeadString(HeBuffer), m_spElementSnHead);
		//	addCustomRenderModel(CModelBodyString(BdBuffer), m_spElementSnBody);
		//	addCustomRenderModel(CModelHandRString(HRBuffer), m_spElementSnHandR);
		//	addCustomRenderModel(CModelHandLString(HLBuffer), m_spElementSnHandL);
		//	addCustomRenderModel(CModelShoeString(ShBuffer), m_spElementSnShoes);
		//	addCustomRenderModel(CModelEyelidString(EyeBuffer), m_spElementSnEyelid);
		//}
	}

	void KillCallback() override
	{
		isRenderableCreated = false;

		//printf("KILL CALLBACK\n");
	}
};
boost::shared_ptr<CustomizeSonicRenderable> obj_CustomizeSonicRenderable;

void CreateCustomizeSonicRenderable()
{
	if (!isRenderableCreated)
	{
		obj_CustomizeSonicRenderable = boost::make_shared<CustomizeSonicRenderable>();
		Sonic::CGameDocument::GetInstance()->AddGameObject(obj_CustomizeSonicRenderable);
		isRenderableCreated = true;
	}
}

void ItemVisibilityHandler()
{
	/*
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
			node->m_Visible = ((SelectShoes == ShDefault && (ShDefaultVariant == DefaultDefault) && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "ShDefault_01")
			node->m_Visible = ((SelectShoes == ShDefault && (ShDefaultVariant == DefaultLightS) && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Classic Shoes
		if (nodeName == "ShClassic")
			node->m_Visible = ((SelectShoes == ShClassic && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//SA1 Lightspeed Shoes
		if (nodeName == "ShSA1LightS")
			node->m_Visible = ((SelectShoes == ShSA1LightS && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//SA2 Soap Shoes
		if (nodeName == "ShSA2Soap_00")
			node->m_Visible = ((SelectShoes == ShSA2Soap && (ShSA2SoapVariant != SA2SoapRacing) && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "ShSA2Soap_01")
			node->m_Visible = ((SelectShoes == ShSA2Soap && (ShSA2SoapVariant == SA2SoapLightS) && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "ShSA2Soap_02")
			node->m_Visible = ((SelectShoes == ShSA2Soap && (ShSA2SoapVariant == SA2SoapRacing) && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Riders Shoes
		if (nodeName == "ShRiders")
			node->m_Visible = ((SelectShoes == ShRiders && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//06 Gem Shoes
		if (nodeName == "Sh06Gem_Default")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemDefault && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "Sh06Gem_Red")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemRed && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "Sh06Gem_Blue")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemBlue && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "Sh06Gem_Green")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemGreen && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "Sh06Gem_Purple")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemPurple && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "Sh06Gem_Sky")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemSky && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "Sh06Gem_White")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemWhite && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "Sh06Gem_Yellow")
			node->m_Visible = ((SelectShoes == Sh06Gem && Sh06GemVariant == GemYellow && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Archie Shoes
		if (nodeName == "ShArchie")
			node->m_Visible = ((SelectShoes == ShArchie && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Unleashed Shoes
		if (nodeName == "ShSUAirboost")
			node->m_Visible = ((SelectShoes == ShSUAirboost && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "ShSULightS")
			node->m_Visible = ((SelectShoes == ShSULightS && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "ShSUStomp")
			node->m_Visible = ((SelectShoes == ShSUStomp && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "ShSUWallJmp")
			node->m_Visible = ((SelectShoes == ShSUWallJmp && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "ShSUWerehog")
			node->m_Visible = ((SelectShoes == ShSUWerehog && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "ShSUZoney")
			node->m_Visible = ((SelectShoes == ShSUZoney && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Simulator Shoes
		if (nodeName == "ShSimulator")
			node->m_Visible = ((SelectShoes == ShSimulator && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Link Boots
		if (nodeName == "ShLink")
			node->m_Visible = ((SelectShoes == ShLink && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Boom Shoes
		if (nodeName == "ShBoom")
			node->m_Visible = ((SelectShoes == ShBoom && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Puma Shoes
		if (nodeName == "ShPumas")
			node->m_Visible = ((SelectShoes == ShPumas && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Prime New Yoke Shoes
		if (nodeName == "ShPrmNewYoke")
			node->m_Visible = ((SelectShoes == ShPrmNewYoke && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Deluxe Shoes
		if (nodeName == "ShSFDeluxe")
			node->m_Visible = ((SelectShoes == ShSFDeluxe && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Korone Shoes
		if (nodeName == "ShSFKorone")
			node->m_Visible = ((SelectShoes == ShSFKorone && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "ShSFMonsterHunter")
			node->m_Visible = ((SelectShoes == ShSFMonsterHunter || SelectBody == BdSFMonsterHunter) && !m_IsModelHide);

		/////Bodies//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Base Body
		if (nodeName == "BdDefault_00")
			node->m_Visible = (((SelectBody == BdDefault && BdDefaultVariant == false) ||
				(SelectBody == BdWildFire && BdWildFireVariant == false) ||
				(SelectBody == BdScarf && BdScarfVariant == false) ||
				(SelectBody == BdMovieSkin && BdMovieVariant == false)) && !m_IsModelHide);
		if (nodeName == "BdDefault_01")
			node->m_Visible = (((SelectBody == BdDefault && BdDefaultVariant == true) ||
				(SelectBody == BdWildFire && BdWildFireVariant == true) ||
				(SelectBody == BdScarf && BdScarfVariant == true) ||
				(SelectBody == BdMovieSkin && BdMovieVariant == true)) && !m_IsModelHide);

		//Racing Suit
		if (nodeName == "BdSA2Racing")
			node->m_Visible = ((SelectBody == BdSA2Racing) && !m_IsModelHide);

		//Simulator
		if (nodeName == "BdSimulator")
			node->m_Visible = ((SelectBody == BdSimulator) && !m_IsModelHide);

		//Link Tunic
		if (nodeName == "BdLink")
			node->m_Visible = ((SelectBody == BdLink) && !m_IsModelHide);

		//Scarf
		if (nodeName == "BdScarf")
			node->m_Visible = ((SelectBody == BdScarf) && !m_IsModelHide);

		//2020 Swim Vest
		if (nodeName == "Bd2020SwimVest")
			node->m_Visible = ((SelectBody == Bd2020SwimVest) && !m_IsModelHide);

		//2020 Surf Suit
		if (nodeName == "Bd2020SurfSuit")
			node->m_Visible = ((SelectBody == Bd2020SurfSuit) && !m_IsModelHide);

		//2020 Horse Shirt
		if (nodeName == "Bd2020Horse")
			node->m_Visible = ((SelectBody == Bd2020Horse) && !m_IsModelHide);

		//Movie Quills
		if (nodeName == "BdMovieSkin")
			node->m_Visible = ((SelectBody == BdMovieSkin) && !m_IsModelHide);
		if (nodeName == "HeDefault_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF)) && !m_IsModelHide);
		if (nodeName == "HeDefault_00_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF)) && !m_IsModelHide);
		if (nodeName == "HeDefault_01_Movie")
			node->m_Visible = false;
		if (nodeName == "HeBoom_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead == HeBoom)) && !m_IsModelHide);
		if (nodeName == "HeLink_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead == HeLink)) && !m_IsModelHide);
		if (nodeName == "HeSFMonsterHunterF_Movie")
			node->m_Visible = (((SelectBody == BdMovieSkin) && (SelectHead == HeSFMonsterHunterF)) && !m_IsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "BdSFMonsterHunter")
			node->m_Visible = ((SelectBody == BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Monster Hunter Felyne
		if (nodeName == "BdSFMonsterHunterF")
			node->m_Visible = ((SelectBody == BdSFMonsterHunterF) && !m_IsModelHide);

		//Frontiers Holiday Cheer Suit
		if (nodeName == "BdSFHoliday")
			node->m_Visible = ((SelectBody == BdSFHoliday) && !m_IsModelHide);

		/////Heads//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Base Head
		if (nodeName == "HeDefault")
			node->m_Visible = ((!(SelectHead == HeSimulator && HeSimulatorVariant == false) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF) && (SelectHead != HeSFMonsterHunter)) && !m_IsModelHide);
		
		//Super Heads
		if (nodeName == "HeDefault_00")
			node->m_Visible = ((!(SelectHead == HeSimulator && HeSimulatorVariant == false) && (SelectHead != HeLink) && (SelectHead != HeBoom) && (SelectHead != HeSFMonsterHunterF) && (SelectHead != HeSFMonsterHunter)) && !m_IsModelHide);
		if (nodeName == "HeDefault_01")
			node->m_Visible = false;

		//SA1 Sunglasses
		if (nodeName == "HeSA1Sunglass")
			node->m_Visible = ((SelectHead == HeSA1Sunglass) && !m_IsModelHide);

		//Riders Glasses
		if (nodeName == "HeRiders")
			node->m_Visible = ((SelectHead == HeRiders) && !m_IsModelHide);

		//Headphones
		if (nodeName == "HeHeadphones")
			node->m_Visible = ((SelectHead == HeHeadphones) && !m_IsModelHide);

		//Simulator
		if (nodeName == "HeSimulator_00")
			node->m_Visible = ((SelectHead == HeSimulator && HeSimulatorVariant == false) && !m_IsModelHide);
		if (nodeName == "HeSimulator_01")
			node->m_Visible = ((SelectHead == HeSimulator && HeSimulatorVariant == true) && !m_IsModelHide);

		//Link Hat
		if (nodeName == "HeLink")
			node->m_Visible = ((SelectHead == HeLink) && !m_IsModelHide);

		//Boom Spikes
		if (nodeName == "HeBoom")
			node->m_Visible = ((SelectHead == HeBoom) && !m_IsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "HeSFMonsterHunter")
			node->m_Visible = ((SelectHead == HeSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Monster Hunter Felyne
		if (nodeName == "HeSFMonsterHunterF")
			node->m_Visible = ((SelectHead == HeSFMonsterHunterF) && !m_IsModelHide);

		//Frontiers Holiday Cheer Suit
		if (nodeName == "HeSFHoliday")
			node->m_Visible = ((SelectHead == HeSFHoliday) && !m_IsModelHide);

		/////Left Hands//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "HLDefault")
			node->m_Visible = ((((SelectHandL == HLDefault) ||
				(SelectHandL == HLSA1Crystal) ||
				(SelectHandL == HLSA2FlameRing) ||
				(SelectHandL == HL06Bounce) ||
				(SelectHandL == HLChip)) &&
				(SelectBody != BdSFMonsterHunter)) && !m_IsModelHide);

		//SA1 Crystal Ring
		if (nodeName == "HLSA1Crystal")
			node->m_Visible = ((SelectHandL == HLSA1Crystal && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//SA2 Flame Ring
		if (nodeName == "HLSA2FlameRing")
			node->m_Visible = ((SelectHandL == HLSA2FlameRing && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//SA2 Racing Glove
		if (nodeName == "HLSA2Racing")
			node->m_Visible = ((SelectHandL == HLSA2Racing && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Riders Glove
		if (nodeName == "HLRiders")
			node->m_Visible = ((SelectHandL == HLRiders && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//06 Bounce Bracelet
		if (nodeName == "HL06Bounce")
			node->m_Visible = ((SelectHandL == HL06Bounce && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Chip Ring
		if (nodeName == "HLChip")
			node->m_Visible = ((SelectHandL == HLChip && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Simulator
		if (nodeName == "HLSimulator")
			node->m_Visible = ((SelectHandL == HLSimulator && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Link Glove
		if (nodeName == "HLLink")
			node->m_Visible = ((SelectHandL == HLLink && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Boom Glove
		if (nodeName == "HLBoom")
			node->m_Visible = ((SelectHandL == HLBoom && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Deluxe Glove
		if (nodeName == "HLSFDeluxe")
			node->m_Visible = ((SelectHandL == HLSFDeluxe && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Korone Glove
		if (nodeName == "HLSFKorone")
			node->m_Visible = ((SelectHandL == HLSFKorone && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "HLSFMonsterHunter")
			node->m_Visible = ((SelectHandL == HLSFMonsterHunter || SelectBody == BdSFMonsterHunter) && !m_IsModelHide);

		/////Right Hands/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Default Glove
		if (nodeName == "HRDefault")
			node->m_Visible = ((((SelectHandR == HRDefault) ||
				(SelectHandR == HRSA2Bounce) ||
				(SelectHandR == HR06Homing) ||
				(SelectHandR == HRSecretRing)) && 
				(SelectBody != BdSFMonsterHunter)) && !m_IsModelHide);

		//SA2 Bounce Bracelet
		if (nodeName == "HRSA2Bounce_00")
			node->m_Visible = ((SelectHandR == HRSA2Bounce) && (SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "HRSA2Bounce_01")
			node->m_Visible = (((SelectHandR == HRSA2Bounce && HRSA2BounceVariant == true) && (SelectBody != BdSFMonsterHunter)) && !m_IsModelHide);

		//SA2 Racing Glove
		if (nodeName == "HRSA2Racing")
			node->m_Visible = ((SelectHandR == HRSA2Racing && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Riders Glove
		if (nodeName == "HRRiders")
			node->m_Visible = (((SelectHandR == HRRiders || SelectHandR == HRRidersArkCosmos) && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);
		if (nodeName == "HRRidersArkCosmos")
			node->m_Visible = ((SelectHandR == HRRidersArkCosmos && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//06 Homing Smash Bracelet
		if (nodeName == "HR06Homing")
			node->m_Visible = ((SelectHandR == HR06Homing && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Secret Ring
		if (nodeName == "HRSecretRing")
			node->m_Visible = ((SelectHandR == HRSecretRing && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Gauntlet
		if (nodeName == "HRGauntlet")
			node->m_Visible = ((SelectHandR == HRGauntlet && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Simulator
		if (nodeName == "HRSimulator")
			node->m_Visible = ((SelectHandR == HRSimulator && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Link Glove
		if (nodeName == "HRLink")
			node->m_Visible = ((SelectHandR == HRLink && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Boom Glove
		if (nodeName == "HRBoom")
			node->m_Visible = ((SelectHandR == HRBoom && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Deluxe Glove
		if (nodeName == "HRSFDeluxe")
			node->m_Visible = ((SelectHandR == HRSFDeluxe && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Korone Glove
		if (nodeName == "HRSFKorone")
			node->m_Visible = ((SelectHandR == HRSFKorone && SelectBody != BdSFMonsterHunter) && !m_IsModelHide);

		//Frontiers Monster Hunter
		if (nodeName == "HRSFMonsterHunter")
			node->m_Visible = ((SelectHandR == HRSFMonsterHunter || SelectBody == BdSFMonsterHunter) && !m_IsModelHide);

		/////Misc/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (nodeName == "EyeDefault")
			node->m_Visible = ((SelectEyelid == EyelidDefault) && !m_IsModelHide);
		if (nodeName == "EyeLash")
			node->m_Visible = ((SelectEyelid == EyelidLashes) && !m_IsModelHide);
		if (nodeName == "EyeSkin")
			node->m_Visible = ((SelectEyelid == EyelidSkin) && !m_IsModelHide);

		//printf("%s\n", nodeName.c_str());

		if (node->m_Visible)
			model->m_NodeGroupModels.push_back(node);
	}

	model->m_NodeGroupModelNum = model->m_NodeGroupModels.size();
	*/

	//Send Wildfire VFX
	if (SelectBodyData == 1)
		MsgWildFire(1);
	else
		MsgWildFire(0);

	MsgJumpBall(SelectJumpBall);

	if (HyperFrameCycle >= 59)
		HyperFrameCycle = 0;
	else
		HyperFrameCycle++;

}

void ReadJson(std::string jsonFilePath, int cCat)
{
	std::ifstream stream(jsonFilePath);
	if (stream.is_open())
	{
		json json;
		stream >> json;

		switch (cCat)
		{
		case 0:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHead.emplace_back();
				itemData.name = obj["name"];
				itemData.altcount = obj["altcount"];
				itemData.hideflags = obj["hideflags"];
				itemData.altselect = 0;
			}
			break;
		case 1:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataBody.emplace_back();
				itemData.name = obj["name"];
				itemData.altcount = obj["altcount"];
				itemData.hideflags = obj["hideflags"];
				itemData.altselect = 0;
			}
			break;
		case 2:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataShoes.emplace_back();
				itemData.name = obj["name"];
				itemData.altcount = obj["altcount"];
				itemData.hideflags = obj["hideflags"];
				itemData.altselect = 0;
			}
			break;
		case 3:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHandR.emplace_back();
				itemData.name = obj["name"];
				itemData.altcount = obj["altcount"];
				itemData.hideflags = obj["hideflags"];
				itemData.altselect = 0;
			}
			break;
		case 4:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHandL.emplace_back();
				itemData.name = obj["name"];
				itemData.altcount = obj["altcount"];
				itemData.hideflags = obj["hideflags"];
				itemData.altselect = 0;
			}
			break;
		}

	}
}