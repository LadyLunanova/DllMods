#pragma once
#include <fstream>
#include <json/json.hpp>
using nlohmann::json;

////------Item Setup------////

struct ItemDataHead
{
	std::string Name;
	int AltCount;
	int AltSelect;
	bool HideHead;
};
inline std::vector<ItemDataHead> s_ItemDataHead;

struct ItemDataBody
{
	std::string Name;
	int AltCount;
	int AltSelect;
	bool HideShoes;
	bool HideHandR;
	bool HideHandL;
};
inline std::vector<ItemDataBody> s_ItemDataBody;
inline ItemDataBody s_ItemDataBodyPrev{};

struct ItemDataShoes
{
	std::string Name;
	int AltCount;
	int AltSelect;
};
inline std::vector<ItemDataShoes> s_ItemDataShoes;

struct ItemDataHandR
{
	std::string Name;
	int AltCount;
	int AltSelect;
};
inline std::vector<ItemDataHandR> s_ItemDataHandR;

struct ItemDataHandL
{
	std::string Name;
	int AltCount;
	int AltSelect;
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
	SBBounceball,
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
	{ SBBounceball,			"SBBounceball" },
	{ SBOverflow06,			"Null" },
	{ SBOverflow07,			"Null" },
	{ SBOverflow08,			"Null" },
};

////------Special Setup------////

enum class SelectCategory : uint32_t
{
	None = 1 << 0,
	Head = 1 << 1,
	Body = 1 << 2,
	Shoes = 1 << 4,
	HandR = 1 << 8,
	HandL = 1 << 16,
	All = Head | Body | Shoes | HandR | HandL
};

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
enum SelectBounceBallType
{
	BounceBallBAP,
	BounceBallDefault,
	BounceBallSWA,
	BounceBallBetaSWA,
	BounceBallLW,
	BounceBallForces,
	BounceBallSA1,
	BounceBallNoVFX,
};

SelectSnSonMatType SelectSnSonMat = SelectSnSonMatType::SnMatOriginal;
SelectSsnSonMatType SelectSsnSonMat = SelectSsnSonMatType::SsnMatOriginal;
SelectSsnHeadType SelectSsnHead = SelectSsnHeadType::SsnFormDefault;
SelectSsnFormType SelectSsnForm = SelectSsnFormType::SsnFormSuper;
SelectEyelidType SelectEyelid = SelectEyelidType::EyelidDefault;
SelectJumpBallType SelectJumpBall = SelectJumpBallType::JumpBallDefault;
SelectBounceBallType SelectBounceBall = SelectBounceBallType::BounceBallBAP;

int HyperFrameCycle = 0;
bool isRenderableCreated = false;
bool isJumpBallHide = false;

void MsgWildFire(int Enabled);
void MsgJumpBall(int BallType);

void MsgJumpModelHide(bool Enabled)
{
	isJumpBallHide = Enabled;
}

Hedgehog::Base::CSharedString CModelHeadString()
{
	char result[512]{};

	auto mapChar = s_ItemDataHead[SelectHeadData].Name;
	auto mapAltCount = s_ItemDataHead[SelectHeadData].AltCount;
	auto mapAltSelect = s_ItemDataHead[SelectHeadData].AltSelect;

	if (mapAltCount >= 1)
		sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "chr_Sn_%s", mapChar.c_str());

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelBodyString()
{
	char result[512]{};

	auto mapChar = s_ItemDataBody[SelectBodyData].Name;
	auto mapAltCount = s_ItemDataBody[SelectBodyData].AltCount;
	auto mapAltSelect = s_ItemDataBody[SelectBodyData].AltSelect;

	if (mapAltCount >= 1)
		sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "chr_Sn_%s", mapChar.c_str());

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelHandLString()
{
	char result[512]{};

	auto mapBodyHideHandL = s_ItemDataBody[SelectBodyData].HideHandL;
	auto mapChar = s_ItemDataHandL[SelectHandLData].Name;
	auto mapAltCount = s_ItemDataHandL[SelectHandLData].AltCount;
	auto mapAltSelect = s_ItemDataHandL[SelectHandLData].AltSelect;

	if (!mapBodyHideHandL)
	{
		if (mapAltCount >= 1)
			sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "chr_Sn_%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelHandRString()
{
	char result[512]{};

	auto mapBodyHideHandR = s_ItemDataBody[SelectBodyData].HideHandR;
	auto mapChar = s_ItemDataHandR[SelectHandRData].Name;
	auto mapAltCount = s_ItemDataHandR[SelectHandRData].AltCount;
	auto mapAltSelect = s_ItemDataHandR[SelectHandRData].AltSelect;

	if (!mapBodyHideHandR)
	{
		if (mapAltCount >= 1)
			sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "chr_Sn_%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelShoeString()
{
	char result[512]{};

	auto mapBodyHideShoes = s_ItemDataBody[SelectBodyData].HideShoes;
	auto mapChar = s_ItemDataShoes[SelectShoesData].Name;
	auto mapAltCount = s_ItemDataShoes[SelectShoesData].AltCount;
	auto mapAltSelect = s_ItemDataShoes[SelectShoesData].AltSelect;

	if (!mapBodyHideShoes)
	{
		if (mapAltCount >= 1)
			sprintf(result, "chr_Sn_%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "chr_Sn_%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelEyelidString()
{
	char result[512]{};

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
	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString CModelBaseHeadString()
{
	char result[512]{};

	auto mapHeadHideHead = s_ItemDataHead[SelectHeadData].HideHead;
	const char* texExtVis = "BdyHead";
	const char* texExtInv = "";

	if (mapHeadHideHead)
		sprintf(result, "chr_Sn_%s", texExtInv);
	else
		sprintf(result, "chr_Sn_%s", texExtVis);

	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString CMaterialBodyString()
{
	char result[512]{};

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
	return Hedgehog::Base::CSharedString(result);
}

Hedgehog::Base::CSharedString ArchiveHeadString()
{
	char result[512]{};

	auto mapChar = s_ItemDataHead[SelectHeadData].Name;
	auto mapAltCount = s_ItemDataHead[SelectHeadData].AltCount;
	auto mapAltSelect = s_ItemDataHead[SelectHeadData].AltSelect;

	if (mapAltCount >= 1)
		sprintf(result, "Customize/ctp_Head/%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "Customize/ctp_Head/%s", mapChar.c_str());

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString ArchiveBodyString()
{
	char result[512]{};

	auto mapChar = s_ItemDataBody[SelectBodyData].Name;
	auto mapAltCount = s_ItemDataBody[SelectBodyData].AltCount;
	auto mapAltSelect = s_ItemDataBody[SelectBodyData].AltSelect;

	if (mapAltCount >= 1)
		sprintf(result, "Customize/ctp_Body/%s_%02d", mapChar.c_str(), mapAltSelect);
	else
		sprintf(result, "Customize/ctp_Body/%s", mapChar.c_str());

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString ArchiveShoeString()
{
	char result[512]{};

	auto mapBodyHideShoes = s_ItemDataBody[SelectBodyData].HideShoes;
	auto mapChar = s_ItemDataShoes[SelectShoesData].Name;
	auto mapAltCount = s_ItemDataShoes[SelectShoesData].AltCount;
	auto mapAltSelect = s_ItemDataShoes[SelectShoesData].AltSelect;

	if (!mapBodyHideShoes)
	{
		if (mapAltCount >= 1)
			sprintf(result, "Customize/ctp_Shoes/%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "Customize/ctp_Shoes/%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString ArchiveHandRString()
{
	char result[512]{};

	auto mapBodyHideHandR = s_ItemDataBody[SelectBodyData].HideHandR;
	auto mapChar = s_ItemDataHandR[SelectHandRData].Name;
	auto mapAltCount = s_ItemDataHandR[SelectHandRData].AltCount;
	auto mapAltSelect = s_ItemDataHandR[SelectHandRData].AltSelect;

	if (!mapBodyHideHandR)
	{
		if (mapAltCount >= 1)
			sprintf(result, "Customize/ctp_Hand_R/%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "Customize/ctp_Hand_R/%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString ArchiveHandLString()
{
	char result[512]{};

	auto mapBodyHideHandL = s_ItemDataBody[SelectBodyData].HideHandL;
	auto mapChar = s_ItemDataHandL[SelectHandLData].Name;
	auto mapAltCount = s_ItemDataHandL[SelectHandLData].AltCount;
	auto mapAltSelect = s_ItemDataHandL[SelectHandLData].AltSelect;

	if (!mapBodyHideHandL)
	{
		if (mapAltCount >= 1)
			sprintf(result, "Customize/ctp_Hand_L/%s_%02d", mapChar.c_str(), mapAltSelect);
		else
			sprintf(result, "Customize/ctp_Hand_L/%s", mapChar.c_str());
	}
	else
		sprintf(result, "");

	return Hedgehog::Base::CSharedString(result);
}
Hedgehog::Base::CSharedString ArchiveEyelidString()
{
	char result[512]{};

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
Hedgehog::Base::CSharedString ArchiveBaseHeadString()
{
	char result[512]{};

	auto mapHeadHideHead = s_ItemDataHead[SelectHeadData].HideHead;
	const char* texExtVis = "BdyHead";
	const char* texExtInv = "";

	if (mapHeadHideHead)
		sprintf(result, "Customize/bdy_Body/%s", texExtInv);
	else
		sprintf(result, "Customize/bdy_Body/%s", texExtVis);

	return Hedgehog::Base::CSharedString(result);
}

class MsgRefreshCustomizeSonic : public Hedgehog::Universe::MessageTypeSet
{
public:
	static constexpr const char* ms_pType = "MsgRefreshCustomizeSonic";

	SelectCategory m_Category{ SelectCategory::All };

	MsgRefreshCustomizeSonic() {}
	MsgRefreshCustomizeSonic(SelectCategory in_category) : m_Category(in_category) {}

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

	boost::shared_ptr<Hedgehog::Mirage::CMatrixNode> m_spMatrixNode{};
	boost::shared_ptr<Hedgehog::Mirage::CPose> m_spPose{};

	bool m_isHeadLoaded{};
	bool m_isCastShadows{ true };

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

	void AddCustomRenderModel(Sonic::CGameObject* in_pGameObject, const char* in_pCategory, boost::shared_ptr<hh::mr::CSingleElement>& in_rspElement, boost::shared_ptr<Hedgehog::Mirage::CModelData> in_spModelData)
	{
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

	void RemoveCustomRenderModel(Sonic::CGameObject* in_pGameObject, const char* in_pCategory, boost::shared_ptr<Hedgehog::Mirage::CSingleElement>& in_spElement)
	{
		in_pGameObject->RemoveRenderable(in_pCategory, in_spElement, m_isCastShadows);
		in_pGameObject->RemoveRenderable("MBP", in_spElement, false);
	}

	void AddCustomRenderMaterial(boost::shared_ptr<hh::mr::CSingleElement>& in_rspElement, boost::shared_ptr<Hedgehog::Mirage::CMaterialData> in_spMaterialData, boost::shared_ptr<Hedgehog::Mirage::CMaterialData> in_spMaterialReplaceData)
	{
		if (!in_rspElement || !in_spMaterialData || !in_spMaterialData->IsMadeAll() || !in_spMaterialReplaceData || !in_spMaterialReplaceData->IsMadeAll())
			return;

		in_rspElement->m_MaterialMap.emplace(in_spMaterialData.get(), in_spMaterialReplaceData);
	}

	void RefreshModels(Sonic::CGameObject* in_pGameObject, const char* in_pRenderCategory, SelectCategory in_selectCategory = SelectCategory::All)
	{
		auto isUpdateAll = in_selectCategory == SelectCategory::All;
		
		if (isUpdateAll || (uint32_t(in_selectCategory) & uint32_t(SelectCategory::Head)) != 0)
		{
			m_isHeadLoaded = false;

			ArDataStruct arHeadData{};
			ArDataStruct arBaseHeadData{};
			ArDataStruct arEyelidData{};

			arHeadData = LoadArchiveDatabase(ArchiveHeadString(), CModelHeadString(), "chr_sn_body_original", CMaterialBodyString());
			arBaseHeadData = LoadArchiveDatabase(ArchiveBaseHeadString(), CModelBaseHeadString(), "chr_sn_body_original", CMaterialBodyString());
			arEyelidData = LoadArchiveDatabase(ArchiveEyelidString(), CModelEyelidString(), "chr_sn_body_original", CMaterialBodyString());

			m_spHeadMdlData = arHeadData.m_spModelData;
			m_spHeadMatData = arHeadData.m_spMaterialData;
			m_spHeadReplaceMatData = arHeadData.m_spReplaceMaterialData;
			m_spBaseHeadMdlData = arBaseHeadData.m_spModelData;
			m_spBaseHeadMatData = arBaseHeadData.m_spMaterialData;
			m_spBaseHeadReplaceMatData = arBaseHeadData.m_spReplaceMaterialData;
			m_spEyelidMdlData = arEyelidData.m_spModelData;
			m_spEyelidMatData = arEyelidData.m_spMaterialData;
			m_spEyelidReplaceMatData = arEyelidData.m_spReplaceMaterialData;

			RemoveCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnHead);
			RemoveCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnBaseHead);
			RemoveCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnEyelid);
		}

		if (isUpdateAll || (uint32_t(in_selectCategory) & uint32_t(SelectCategory::Body)) != 0)
		{
			ArDataStruct arBodyData{};

			arBodyData = LoadArchiveDatabase(ArchiveBodyString(), CModelBodyString(), "chr_sn_body_original", CMaterialBodyString());

			m_spBodyMdlData = arBodyData.m_spModelData;
			m_spBodyMatData = arBodyData.m_spMaterialData;
			m_spBodyReplaceMatData = arBodyData.m_spReplaceMaterialData;

			RemoveCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnBody);
		}

		if (isUpdateAll || (uint32_t(in_selectCategory) & uint32_t(SelectCategory::Shoes)) != 0)
		{
			m_spShoesMdlData = LoadArchiveDatabase(ArchiveShoeString(), CModelShoeString(), "", "").m_spModelData;
			RemoveCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnShoes);
		}

		if (isUpdateAll || (uint32_t(in_selectCategory) & uint32_t(SelectCategory::HandL)) != 0)
		{
			m_spHandLMdlData = LoadArchiveDatabase(ArchiveHandLString(), CModelHandLString(), "", "").m_spModelData;
			RemoveCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnHandL);
		}

		if (isUpdateAll || (uint32_t(in_selectCategory) & uint32_t(SelectCategory::HandR)) != 0)
		{
			m_spHandRMdlData = LoadArchiveDatabase(ArchiveHandRString(), CModelHandRString(), "", "").m_spModelData;
			RemoveCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnHandR);
		}
	}

	void UpdateRenderables(Sonic::CGameObject* in_pGameObject, const char* in_pRenderCategory)
	{
		////Refresh Models
		if (m_spHeadMdlData != nullptr && m_spHeadMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnHead, m_spHeadMdlData);
			m_spHeadMdlData = nullptr;
			m_isHeadLoaded = true;
		}
		if (m_spBaseHeadMdlData != nullptr && m_spBaseHeadMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnBaseHead, m_spBaseHeadMdlData);
			m_spBaseHeadMdlData = nullptr;
			m_isHeadLoaded = true;
		}
		if (m_spEyelidMdlData != nullptr && m_spEyelidMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnEyelid, m_spEyelidMdlData);
			m_spEyelidMdlData = nullptr;
		}
		if (m_spBodyMdlData != nullptr && m_spBodyMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnBody, m_spBodyMdlData);
			m_spBodyMdlData = nullptr;
		}
		if (m_spShoesMdlData != nullptr && m_spShoesMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnShoes, m_spShoesMdlData);
			m_spShoesMdlData = nullptr;
		}
		if (m_spHandLMdlData != nullptr && m_spHandLMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnHandL, m_spHandLMdlData);
			m_spHandLMdlData = nullptr;
		}
		if (m_spHandRMdlData != nullptr && m_spHandRMdlData->IsMadeAll())
		{
			AddCustomRenderModel(in_pGameObject, in_pRenderCategory, m_spSnHandR, m_spHandRMdlData);
			m_spHandRMdlData = nullptr;
		}

		////Refresh Materials
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
		if (m_spEyelidMatData != nullptr && m_spEyelidMatData->IsMadeAll() && m_spEyelidReplaceMatData != nullptr && m_spEyelidReplaceMatData->IsMadeAll())
		{
			AddCustomRenderMaterial(m_spSnEyelid, m_spEyelidMatData, m_spEyelidReplaceMatData);
			m_spEyelidMatData = nullptr;
			m_spEyelidReplaceMatData = nullptr;
		}
		if (m_spBodyMatData != nullptr && m_spBodyMatData->IsMadeAll() && m_spBodyReplaceMatData != nullptr && m_spBodyReplaceMatData->IsMadeAll())
		{
			AddCustomRenderMaterial(m_spSnBody, m_spBodyMatData, m_spBodyReplaceMatData);
			m_spBodyMatData = nullptr;
			m_spBodyReplaceMatData = nullptr;
		}
	}
};

class CustomizeSonicPlayerRenderable : public CustomizeSonicRenderable, public Sonic::CGameObject3D
{
public:
	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder, Sonic::CGameDocument* in_pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		in_pGameDocument->AddUpdateUnit("b", this);

		// Set previous body data to current.
		s_ItemDataBodyPrev = s_ItemDataBody[SelectBodyData];

		// Load initial models.
		MsgRefreshCustomizeSonic msg{ SelectCategory::All };
		ProcessMessage(msg, false);
	}

	void UpdateSerial(const Hedgehog::Universe::SUpdateInfo& in_rUpdateInfo) override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* pContext = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));

		m_spMatrixNode = pContext->m_spModelMatrixNode;
		m_spPose = pPlayer->m_spCharacterModel->m_spInstanceInfo->m_spPose;
		
		auto isModelEnabled = pPlayer->m_spCharacterModel->m_Enabled && !isJumpBallHide;

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
		if (in_rMsg.Is<MsgRefreshCustomizeSonic>())
		{
			auto& msgRefreshCustomizeSonic = static_cast<MsgRefreshCustomizeSonic&>(in_rMsg);
			
			RefreshModels(this, "Player", msgRefreshCustomizeSonic.m_Category);

			printf("REFRESH PLAYER MODELS: %d\n", int(msgRefreshCustomizeSonic.m_Category));
		}

		return true;
	}

	void KillCallback() override
	{
		printf("KILL PLAYER RENDERABLE\n");
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
	if (s_ItemDataBody[SelectBodyData].Name == "BdWildFire")
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
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.HideHead = obj["HideHead"];
				itemData.AltSelect = 0;
			}
			break;
		case 1:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataBody.emplace_back();
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.HideShoes = obj["HideShoes"];
				itemData.HideHandR = obj["HideHandR"];
				itemData.HideHandL = obj["HideHandL"];
				itemData.AltSelect = 0;
			}
			break;
		case 2:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataShoes.emplace_back();
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.AltSelect = 0;
			}
			break;
		case 3:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHandR.emplace_back();
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.AltSelect = 0;
			}
			break;
		case 4:
			for (const auto& obj : json)
			{
				auto& itemData = s_ItemDataHandL.emplace_back();
				itemData.Name = obj["Name"];
				itemData.AltCount = obj["AltCount"];
				itemData.AltSelect = 0;
			}
			break;
		}

	}
}