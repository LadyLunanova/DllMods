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
inline std::vector<ItemDataHead> s_ItemDataHead;

struct ItemDataBody
{
	std::string name;
	int altcount;
	int altselect;
	int hideflags;
};
inline std::vector<ItemDataBody> s_ItemDataBody;

struct ItemDataShoes
{
	std::string name;
	int altcount;
	int altselect;
};
inline std::vector<ItemDataShoes> s_ItemDataShoes;

struct ItemDataHandR
{
	std::string name;
	int altcount;
	int altselect;
};
inline std::vector<ItemDataHandR> s_ItemDataHandR;

struct ItemDataHandL
{
	std::string name;
	int altcount;
	int altselect;
};
inline std::vector<ItemDataHandL> s_ItemDataHandL;

int SelectHeadData = 0;
int SelectBodyData = 0;
int SelectShoesData = 0;
int SelectHandRData = 0;
int SelectHandLData = 0;

////------Hardcoded Item Setup------////

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

SelectSonicBodyType SelectSonicBody = SelectSonicBodyType::SBSnMaterial;

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

int HyperFrameCycle = 0;
bool isRenderableCreated = false;
bool isLoadModel = true;

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
	auto mapBodyHideFlag = s_ItemDataBody[SelectBodyData].hideflags;
	auto mapChar = s_ItemDataHandL[SelectHandLData].name;
	auto mapAltCount = s_ItemDataHandL[SelectHandLData].altcount;
	auto mapAltSelect = s_ItemDataHandL[SelectHandLData].altselect;

	if (mapBodyHideFlag == 0 || mapBodyHideFlag == 3)
	{
		if (mapAltCount >= 1)
			sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "chr_Sn_%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return result;
}
const char* CModelHandRString(char* result)
{
	auto mapBodyHideFlag = s_ItemDataBody[SelectBodyData].hideflags;
	auto mapChar = s_ItemDataHandR[SelectHandRData].name;
	auto mapAltCount = s_ItemDataHandR[SelectHandRData].altcount;
	auto mapAltSelect = s_ItemDataHandR[SelectHandRData].altselect;

	if (mapBodyHideFlag == 0 || mapBodyHideFlag == 4)
	{
		if (mapAltCount >= 1)
			sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "chr_Sn_%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return result;
}
const char* CModelShoeString(char* result)
{
	auto mapBodyHideFlag = s_ItemDataBody[SelectBodyData].hideflags;
	auto mapChar = s_ItemDataShoes[SelectShoesData].name;
	auto mapAltCount = s_ItemDataShoes[SelectShoesData].altcount;
	auto mapAltSelect = s_ItemDataShoes[SelectShoesData].altselect;

	if (mapBodyHideFlag == 0 || mapBodyHideFlag == 3 || mapBodyHideFlag == 4)
	{
		if (mapAltCount >= 1)
			sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "chr_Sn_%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return result;
}
const char* CModelEyelidString(char* result)
{
	const char* texExtVar = "";
	const char* texExt0 = "BdyEyeDefault";
	const char* texExt1 = "BdyEyeLash";
	const char* texExt2 = "BdyEyeSkin";

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
const char* CModelBaseHeadString(char* result)
{
	auto mapHeadHideFlag = s_ItemDataHead[SelectHeadData].hideflags;
	const char* texExtVis = "BdyHead";
	const char* texExtInv = "";

	if (mapHeadHideFlag != 0)
		sprintf(result, "chr_Sn_%s", texExtInv);
	else
		sprintf(result, "chr_Sn_%s", texExtVis);

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
	auto mapBodyHideFlag = s_ItemDataBody[SelectBodyData].hideflags;
	auto mapChar = s_ItemDataShoes[SelectShoesData].name;
	auto mapAltCount = s_ItemDataShoes[SelectShoesData].altcount;
	auto mapAltSelect = s_ItemDataShoes[SelectShoesData].altselect;

	if (mapBodyHideFlag == 0 || mapBodyHideFlag == 3 || mapBodyHideFlag == 4)
	{
		if (mapAltCount >= 1)
			sprintf(result, "Customize/ctp_Shoes/%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "Customize/ctp_Shoes/%s", mapChar.c_str());
	}
	else
		sprintf(result, "");
	

	return result;
}
const char* ArchiveHandRString(char* result)
{
	auto mapBodyHideFlag = s_ItemDataBody[SelectBodyData].hideflags;
	auto mapChar = s_ItemDataHandR[SelectHandRData].name;
	auto mapAltCount = s_ItemDataHandR[SelectHandRData].altcount;
	auto mapAltSelect = s_ItemDataHandR[SelectHandRData].altselect;

	if (mapBodyHideFlag == 0 || mapBodyHideFlag == 4)
	{
		if (mapAltCount >= 1)
			sprintf(result, "Customize/ctp_Hand_R/%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "Customize/ctp_Hand_R/%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return result;
}
const char* ArchiveHandLString(char* result)
{
	auto mapBodyHideFlag = s_ItemDataBody[SelectBodyData].hideflags;
	auto mapChar = s_ItemDataHandL[SelectHandLData].name;
	auto mapAltCount = s_ItemDataHandL[SelectHandLData].altcount;
	auto mapAltSelect = s_ItemDataHandL[SelectHandLData].altselect;

	if (mapBodyHideFlag == 0 || mapBodyHideFlag == 3)
	{
		if (mapAltCount >= 1)
			sprintf(result, "Customize/ctp_Hand_L/%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "Customize/ctp_Hand_L/%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return result;
}
const char* ArchiveEyelidString(char* result)
{
	const char* texExtVar = "";
	const char* texExt0 = "BdyEyeDefault";
	const char* texExt1 = "BdyEyeLash";
	const char* texExt2 = "BdyEyeSkin";

	if (SelectEyelid == EyelidDefault)
		texExtVar = texExt0;
	else if (SelectEyelid == EyelidLashes)
		texExtVar = texExt1;
	else if (SelectEyelid == EyelidSkin)
		texExtVar = texExt2;


	sprintf(result, "Customize/bdy_Body/%s", texExtVar);
	return result;
}
const char* ArchiveBaseHeadString(char* result)
{
	auto mapHeadHideFlag = s_ItemDataHead[SelectHeadData].hideflags;
	const char* texExtVis = "BdyHead";
	const char* texExtInv = "";

	if (mapHeadHideFlag != 0)
		sprintf(result, "Customize/bdy_Body/%s", texExtInv);
	else
		sprintf(result, "Customize/bdy_Body/%s", texExtVis);

	return result;
}

//////Sonic Renderable
class CustomizeSonicRenderable : public Sonic::CGameObject3D
{
public:
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnHead;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnBaseHead;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnBody;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnShoes;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnHandR;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnHandL;
	boost::shared_ptr<hh::mr::CSingleElement> m_spElementSnEyelid;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArHeadMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArBaseHeadMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArBodyMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArShoesMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArHandRMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArHandLMdlData;
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spArEyelidMdlData;
	boost::shared_ptr<Hedgehog::Database::CDatabase> m_spArDatabase;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArHeadMatData;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArHeadReplaceMatData;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArBaseHeadMatData;
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spArBaseHeadReplaceMatData;
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
		AddRenderable("MBP", m_spElement, false);

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
		arDataStruct arBaseHeadData;
		arDataStruct arBodyData;
		arDataStruct arEyelidData;

		if (isLoadModel)
		{
			arHeadData = loadArchiveDatabase(ArchiveHeadString(HeBuffer), CModelHeadString(HeBuffer), "chr_sn_body_original", CMaterialBodyString(HeBuffer));
			arBaseHeadData = loadArchiveDatabase(ArchiveBaseHeadString(HeBuffer), CModelBaseHeadString(HeBuffer), "chr_sn_body_original", CMaterialBodyString(HeBuffer));
			arBodyData = loadArchiveDatabase(ArchiveBodyString(BdBuffer), CModelBodyString(BdBuffer), "chr_sn_body_original", CMaterialBodyString(BdBuffer));
			arEyelidData = loadArchiveDatabase(ArchiveEyelidString(HeBuffer), CModelEyelidString(HeBuffer), "chr_sn_body_original", CMaterialBodyString(HeBuffer));

			m_spArShoesMdlData = loadArchiveDatabase(ArchiveShoeString(ShBuffer), CModelShoeString(ShBuffer), "", "").m_spArModelData;
			m_spArHandRMdlData = loadArchiveDatabase(ArchiveHandRString(HRBuffer), CModelHandRString(HRBuffer), "", "").m_spArModelData;
			m_spArHandLMdlData = loadArchiveDatabase(ArchiveHandLString(HLBuffer), CModelHandLString(HLBuffer), "", "").m_spArModelData;

			m_spArHeadMdlData = arHeadData.m_spArModelData;
			m_spArHeadMatData = arHeadData.m_spArMaterialData;
			m_spArHeadReplaceMatData = arHeadData.m_spArReplaceMaterialData;

			m_spArBaseHeadMdlData = arBaseHeadData.m_spArModelData;
			m_spArBaseHeadMatData = arBaseHeadData.m_spArMaterialData;
			m_spArBaseHeadReplaceMatData = arBaseHeadData.m_spArReplaceMaterialData;

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
		if (m_spArBaseHeadMdlData != nullptr && m_spArBaseHeadMdlData->IsMadeAll())
		{
			addCustomRenderModel(m_spElementSnBaseHead, m_spArBaseHeadMdlData);
			m_spArBaseHeadMdlData = nullptr;
		}
		if (m_spArHeadMatData != nullptr && m_spArHeadMatData->IsMadeAll() && m_spArHeadReplaceMatData != nullptr && m_spArHeadReplaceMatData->IsMadeAll())
		{
			addCustomRenderMaterial(m_spElementSnHead, m_spArHeadMatData, m_spArHeadReplaceMatData);
			m_spArHeadMatData = nullptr;
			m_spArHeadReplaceMatData = nullptr;
		}
		if (m_spArBaseHeadMatData != nullptr && m_spArBaseHeadMatData->IsMadeAll() && m_spArBaseHeadReplaceMatData != nullptr && m_spArBaseHeadReplaceMatData->IsMadeAll())
		{
			addCustomRenderMaterial(m_spElementSnBaseHead, m_spArBaseHeadMatData, m_spArBaseHeadReplaceMatData);
			m_spArBaseHeadMatData = nullptr;
			m_spArBaseHeadReplaceMatData = nullptr;
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
		if (m_spElementSnBaseHead != nullptr)
		{
			m_spElementSnBaseHead->m_Enabled = !pPlayer_Invisible;
		}

		auto input = Sonic::CInputState::GetInstance()->GetPadState();
		bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
		bool PressedLeft = input.IsTapped(Sonic::eKeyState_DpadLeft);
		bool PressedRight = input.IsTapped(Sonic::eKeyState_DpadRight);
		bool PressedUp = input.IsTapped(Sonic::eKeyState_DpadUp);
		bool PressedDown = input.IsTapped(Sonic::eKeyState_DpadDown);

		loadCustomRenderModel();
	}

	void KillCallback() override
	{
		RemoveRenderables();
		isRenderableCreated = false;
		isLoadModel = true;
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

void KillCustomizeSonicRenderable()
{
	if (isRenderableCreated)
	{
		obj_CustomizeSonicRenderable->SendMessageImm<Sonic::Message::MsgKill>(obj_CustomizeSonicRenderable->m_ActorID);
		obj_CustomizeSonicRenderable = nullptr;
		isRenderableCreated = false;
		isLoadModel = true;
		//printf("KILL RENDERABLE\n");
	}
}

void OnFrameHandler()
{
	//Send Wildfire VFX
	if (s_ItemDataBody[SelectBodyData].name == "BdWildFire")
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
				itemData.altselect = 0;
			}
			break;
		case 3:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHandR.emplace_back();
				itemData.name = obj["name"];
				itemData.altcount = obj["altcount"];
				itemData.altselect = 0;
			}
			break;
		case 4:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHandL.emplace_back();
				itemData.name = obj["name"];
				itemData.altcount = obj["altcount"];
				itemData.altselect = 0;
			}
			break;
		}

	}
}