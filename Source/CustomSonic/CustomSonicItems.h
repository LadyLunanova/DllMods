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
bool isJumpBallHide = false;

void MsgWildFire(int Enabled);
void MsgJumpBall(int BallType);
void MsgJumpModelHide(bool Enabled)
{
	isJumpBallHide = Enabled;
}

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

class MsgUpdateCustomModels : public Hedgehog::Universe::MessageTypeSet
{
public:
	static constexpr const char* ms_pType = "MsgUpdateCustomModels";

    bool IsOfType(const char* in_pType) const override
    {
        return in_pType == ms_pType;
    }
    
    const char* GetType() const override
    {
        return ms_pType;
    }
};

class CustomizeSonicRenderable
{
public:
	boost::shared_ptr<Hedgehog::Database::CDatabase> m_spDatabase{};
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spHeadMdlData{};
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spBaseHeadMdlData{};
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spBodyMdlData{};
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spShoesMdlData{};
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spHandRMdlData{};
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spHandLMdlData{};
	boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spEyelidMdlData{};
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spHeadMatData{};
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spHeadReplaceMatData{};
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spBaseHeadMatData{};
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spBaseHeadReplaceMatData{};
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spBodyMatData{};
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spBodyReplaceMatData{};
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spEyelidMatData{};
	boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spEyelidReplaceMatData{};
	boost::shared_ptr<Hedgehog::Mirage::CSingleElement> m_spSnHead{};
	boost::shared_ptr<Hedgehog::Mirage::CSingleElement> m_spSnBaseHead{};
	boost::shared_ptr<Hedgehog::Mirage::CSingleElement> m_spSnBody{};
	boost::shared_ptr<Hedgehog::Mirage::CSingleElement> m_spSnShoes{};
	boost::shared_ptr<Hedgehog::Mirage::CSingleElement> m_spSnHandR{};
	boost::shared_ptr<Hedgehog::Mirage::CSingleElement> m_spSnHandL{};
	boost::shared_ptr<Hedgehog::Mirage::CSingleElement> m_spSnEyelid{};
	std::vector<boost::shared_ptr<Hedgehog::Mirage::CSingleElement>> m_vspExtraElements{};
	boost::shared_ptr<Hedgehog::Mirage::CMatrixNode> m_spMatrixNode{};
	boost::shared_ptr<Hedgehog::Mirage::CPose> m_spPose{};
	bool m_isCastShadows{ true };
	bool m_isUpdateModels{ true };

	struct ArDataStruct
	{
		boost::shared_ptr<Hedgehog::Mirage::CModelData> m_spModelData{};
		boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spMaterialData{};
		boost::shared_ptr<Hedgehog::Mirage::CMaterialData> m_spReplaceMaterialData{};
	};

	ArDataStruct LoadArchiveDatabase(Hedgehog::Base::CSharedString in_archiveName, Hedgehog::Base::CSharedString in_modelName, Hedgehog::Base::CSharedString in_materialName, Hedgehog::Base::CSharedString in_materialReplaceName)
	{
		ArDataStruct arDataStruct{};

		m_spDatabase = Hedgehog::Database::CDatabase::CreateDatabase();

		auto& loader = Sonic::CApplicationDocument::GetInstance()->m_pMember->m_spDatabaseLoader;

		loader->CreateArchiveList(in_archiveName + ".ar", in_archiveName + ".arl", { 200, 5 });
		loader->LoadArchiveList(m_spDatabase, in_archiveName + ".arl");
		loader->LoadArchive(m_spDatabase, in_archiveName + ".ar", { -10, 5 }, false, false);

		Hedgehog::Mirage::CMirageDatabaseWrapper wrapper(m_spDatabase.get());

		arDataStruct.m_spModelData = wrapper.GetModelData(in_modelName);
		arDataStruct.m_spMaterialData = wrapper.GetMaterialData(in_materialName);
		arDataStruct.m_spReplaceMaterialData = wrapper.GetMaterialData(in_materialReplaceName);

		return arDataStruct;
	}

	//////Custom Funcs
	void AddCustomRenderModel(Sonic::CGameObject* in_pGameObject, const char* in_pCategory, boost::shared_ptr<hh::mr::CSingleElement>& in_rspElement, boost::shared_ptr<Hedgehog::Mirage::CModelData> in_spModelData)
	{
		////Setup Model
		if (!in_spModelData || !in_spModelData->IsMadeAll())
			return;

		in_rspElement = boost::make_shared<hh::mr::CSingleElement>(in_spModelData);

		in_pGameObject->AddRenderable(in_pCategory, in_rspElement, m_isCastShadows);

		if (m_isCastShadows)
			in_pGameObject->AddRenderable("MBP", in_rspElement, false);

		if (m_spMatrixNode)
			in_rspElement->BindMatrixNode(m_spMatrixNode);

		if (m_spPose)
			in_rspElement->BindPose(m_spPose);
	}

	void AddCustomRenderMaterial(boost::shared_ptr<hh::mr::CSingleElement>& in_rspElement, boost::shared_ptr<Hedgehog::Mirage::CMaterialData> in_spMaterialData, boost::shared_ptr<Hedgehog::Mirage::CMaterialData> in_spMaterialReplaceData)
	{
		if (!in_rspElement || !in_spMaterialData || !in_spMaterialData->IsMadeAll() || !in_spMaterialReplaceData || !in_spMaterialReplaceData->IsMadeAll())
			return;

		in_rspElement->m_MaterialMap.emplace(in_spMaterialData.get(), in_spMaterialReplaceData);
	}

	void UpdateRenderables(Sonic::CGameObject* in_pGameObject, const char* m_pCategory)
	{
		char ShBuffer[256]{};
		char BdBuffer[256]{};
		char HeBuffer[256]{};
		char HLBuffer[256]{};
		char HRBuffer[256]{};
		char EyeBuffer[256]{};

		ArDataStruct arHeadData{};
		ArDataStruct arBaseHeadData{};
		ArDataStruct arBodyData{};
		ArDataStruct arEyelidData{};

		if (m_isUpdateModels)
		{
			arHeadData = LoadArchiveDatabase(ArchiveHeadString(HeBuffer), CModelHeadString(HeBuffer), "chr_sn_body_original", CMaterialBodyString(HeBuffer));
			arBaseHeadData = LoadArchiveDatabase(ArchiveBaseHeadString(HeBuffer), CModelBaseHeadString(HeBuffer), "chr_sn_body_original", CMaterialBodyString(HeBuffer));
			arBodyData = LoadArchiveDatabase(ArchiveBodyString(BdBuffer), CModelBodyString(BdBuffer), "chr_sn_body_original", CMaterialBodyString(BdBuffer));
			arEyelidData = LoadArchiveDatabase(ArchiveEyelidString(HeBuffer), CModelEyelidString(HeBuffer), "chr_sn_body_original", CMaterialBodyString(HeBuffer));

			m_spShoesMdlData = LoadArchiveDatabase(ArchiveShoeString(ShBuffer), CModelShoeString(ShBuffer), "", "").m_spModelData;
			m_spHandRMdlData = LoadArchiveDatabase(ArchiveHandRString(HRBuffer), CModelHandRString(HRBuffer), "", "").m_spModelData;
			m_spHandLMdlData = LoadArchiveDatabase(ArchiveHandLString(HLBuffer), CModelHandLString(HLBuffer), "", "").m_spModelData;

			m_spHeadMdlData = arHeadData.m_spModelData;
			m_spHeadMatData = arHeadData.m_spMaterialData;
			m_spHeadReplaceMatData = arHeadData.m_spReplaceMaterialData;

			m_spBaseHeadMdlData = arBaseHeadData.m_spModelData;
			m_spBaseHeadMatData = arBaseHeadData.m_spMaterialData;
			m_spBaseHeadReplaceMatData = arBaseHeadData.m_spReplaceMaterialData;

			m_spBodyMdlData = arBodyData.m_spModelData;
			m_spBodyMatData = arBodyData.m_spMaterialData;
			m_spBodyReplaceMatData = arBodyData.m_spReplaceMaterialData;

			m_spEyelidMdlData = arEyelidData.m_spModelData;
			m_spEyelidMatData = arEyelidData.m_spMaterialData;
			m_spEyelidReplaceMatData = arEyelidData.m_spReplaceMaterialData;

			in_pGameObject->RemoveRenderables();

			for (auto& spElement : m_vspExtraElements)
				in_pGameObject->AddRenderable(m_pCategory, spElement, false);
			
			m_isUpdateModels = false;
		}

		if (m_spHeadMdlData != nullptr && m_spHeadMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, m_pCategory, m_spSnHead, m_spHeadMdlData);
			m_spHeadMdlData = nullptr;
		}

		if (m_spBodyMdlData != nullptr && m_spBodyMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, m_pCategory, m_spSnBody, m_spBodyMdlData);
			m_spBodyMdlData = nullptr;
		}

		if (m_spShoesMdlData != nullptr && m_spShoesMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, m_pCategory, m_spSnShoes, m_spShoesMdlData);
			m_spShoesMdlData = nullptr;
		}

		if (m_spHandRMdlData != nullptr && m_spHandRMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, m_pCategory, m_spSnHandR, m_spHandRMdlData);
			m_spHandRMdlData = nullptr;
		}

		if (m_spHandLMdlData != nullptr && m_spHandLMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, m_pCategory, m_spSnHandL, m_spHandLMdlData);
			m_spHandLMdlData = nullptr;
		}

		if (m_spEyelidMdlData != nullptr && m_spEyelidMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, m_pCategory, m_spSnEyelid, m_spEyelidMdlData);
			m_spEyelidMdlData = nullptr;
		}

		if (m_spBaseHeadMdlData != nullptr && m_spBaseHeadMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, m_pCategory, m_spSnBaseHead, m_spBaseHeadMdlData);
			m_spBaseHeadMdlData = nullptr;
		}

		if (m_spHeadMatData != nullptr && m_spHeadMatData->IsMadeAll() && m_spHeadReplaceMatData != nullptr && m_spHeadReplaceMatData->IsMadeAll())
		{
			AddCustomRenderMaterial(m_spSnHead, m_spHeadMatData, m_spHeadReplaceMatData);
			m_spHeadMatData = nullptr;
			m_spHeadReplaceMatData = nullptr;
		}

		if (m_spBaseHeadMatData != nullptr && m_spBaseHeadMatData->IsMadeAll() && m_spBaseHeadReplaceMatData != nullptr && m_spBaseHeadReplaceMatData->IsMadeAll())
		{
			AddCustomRenderMaterial(m_spSnBaseHead, m_spBaseHeadMatData, m_spBaseHeadReplaceMatData);
			m_spBaseHeadMatData = nullptr;
			m_spBaseHeadReplaceMatData = nullptr;
		}

		if (m_spBodyMatData != nullptr && m_spBodyMatData->IsMadeAll() && m_spBodyReplaceMatData != nullptr && m_spBodyReplaceMatData->IsMadeAll())
		{
			AddCustomRenderMaterial(m_spSnBody, m_spBodyMatData, m_spBodyReplaceMatData);
			m_spBodyMatData = nullptr;
			m_spBodyReplaceMatData = nullptr;
		}

		if (m_spEyelidMatData != nullptr && m_spEyelidMatData->IsMadeAll() && m_spEyelidReplaceMatData != nullptr && m_spEyelidReplaceMatData->IsMadeAll())
		{
			AddCustomRenderMaterial(m_spSnEyelid, m_spEyelidMatData, m_spEyelidReplaceMatData);
			m_spEyelidMatData = nullptr;
			m_spEyelidReplaceMatData = nullptr;
		}
	}
};

//////Sonic Renderable
class CustomizeSonicPlayerRenderable : public CustomizeSonicRenderable, public Sonic::CGameObject3D
{
public:
	//////Renderable Funcs
	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder,
		Sonic::CGameDocument* in_pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		in_pGameDocument->AddUpdateUnit("b", this);
	}

	void UpdateSerial(const Hedgehog::Universe::SUpdateInfo& in_rUpdateInfo) override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* pContext = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));

		m_spMatrixNode = pContext->m_spModelMatrixNode;
		m_spPose = pPlayer->m_spCharacterModel->m_spInstanceInfo->m_spPose;
		
		auto isPlayerInvisible = (pPlayer->m_spCharacterModel->m_spInstanceInfo->m_Flags & Hedgehog::Mirage::eInstanceInfoFlags_Invisible) != 0;
		auto isModelEnabled = !isPlayerInvisible && !isJumpBallHide;

		if (m_spSnHead != nullptr)
			m_spSnHead->m_Enabled = isModelEnabled;

		if (m_spSnBody != nullptr)
			m_spSnBody->m_Enabled = isModelEnabled;

		if (m_spSnShoes != nullptr)
			m_spSnShoes->m_Enabled = isModelEnabled;

		if (m_spSnHandR != nullptr)
			m_spSnHandR->m_Enabled = isModelEnabled;

		if (m_spSnHandL != nullptr)
			m_spSnHandL->m_Enabled = isModelEnabled;

		if (m_spSnEyelid != nullptr)
			m_spSnEyelid->m_Enabled = isModelEnabled;

		if (m_spSnBaseHead != nullptr)
			m_spSnBaseHead->m_Enabled = isModelEnabled;

		UpdateRenderables(this, "Player");
	}

	bool ProcessMessage(Hedgehog::Universe::Message& in_rMsg, bool in_Flag) override
	{
		if (in_rMsg.GetType() == MsgUpdateCustomModels::ms_pType)
			m_isUpdateModels = true;

		return true;
	}

	void KillCallback() override
	{
		printf("KILL PREVIEW RENDERABLE\n");
		RemoveRenderables();
	}
};
boost::shared_ptr<CustomizeSonicPlayerRenderable> obj_CustomizeSonicPlayerRenderable;

void CreateCustomizeSonicPlayerRenderable()
{
	if (!isRenderableCreated)
	{
		obj_CustomizeSonicPlayerRenderable = boost::make_shared<CustomizeSonicPlayerRenderable>();
		Sonic::CGameDocument::GetInstance()->AddGameObject(obj_CustomizeSonicPlayerRenderable);
		isRenderableCreated = true;
	}
}

void KillCustomizeSonicPlayerRenderable()
{
	if (isRenderableCreated)
	{
		obj_CustomizeSonicPlayerRenderable->Kill();
		obj_CustomizeSonicPlayerRenderable = nullptr;
		isRenderableCreated = false;
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