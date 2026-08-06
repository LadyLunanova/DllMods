#pragma once

#include <fstream>
#include <json/json.hpp>
#include "CustomSonicAPI.h"

using namespace CustomSonicAPI;
using nlohmann::json;

////------Item Setup------////

struct ItemDataHead
{
	std::string Name;
	int AltCount;
	int AltSelect;
	bool HideHead;
};
extern std::vector<ItemDataHead> s_ItemDataHead;

struct ItemDataBody
{
	std::string Name;
	int AltCount;
	int AltSelect;
	bool HideShoes;
	bool HideHandR;
	bool HideHandL;
};
extern std::vector<ItemDataBody> s_ItemDataBody;
extern ItemDataBody s_ItemDataBodyPrev;

struct ItemDataShoes
{
	std::string Name;
	int AltCount;
	int AltSelect;
};
extern std::vector<ItemDataShoes> s_ItemDataShoes;

struct ItemDataHandR
{
	std::string Name;
	int AltCount;
	int AltSelect;
};
extern std::vector<ItemDataHandR> s_ItemDataHandR;

struct ItemDataHandL
{
	std::string Name;
	int AltCount;
	int AltSelect;
};
extern std::vector<ItemDataHandL> s_ItemDataHandL;

extern int SelectHeadData;
extern int SelectBodyData;
extern int SelectShoesData;
extern int SelectHandRData;
extern int SelectHandLData;

////------Hardcoded Item Setup------////

enum class SelectOptionType
{
	SnMaterial,
	Eyelids,
	SuperHead,
	SsnMaterial,
	JumpBall,
	BounceBall,
	Overflow01,
	Overflow02,
	Overflow03
};

extern SelectOptionType SelectOption;
extern std::map<SelectOptionType, const char*> SelectOptionNameMap;

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

extern SelectSnMaterialType SelectSnMaterial;
extern SelectEyelidType SelectEyelid;
extern SelectSsnHeadType SelectSsnHead;
extern SelectSsnMaterialType SelectSsnMaterial;
extern SelectJumpBallType SelectJumpBall;
extern SelectBounceBallType SelectBounceBall;

extern int HyperFrameCycle;
extern bool isRenderableCreated;
extern bool isJumpBallHide;

void MsgJumpModelHide(bool Enabled);

Hedgehog::Base::CSharedString CModelHeadString(bool in_isSuper);
Hedgehog::Base::CSharedString CModelBodyString();
Hedgehog::Base::CSharedString CModelHandLString();
Hedgehog::Base::CSharedString CModelHandRString();
Hedgehog::Base::CSharedString CModelShoeString();
Hedgehog::Base::CSharedString CModelEyelidString();
Hedgehog::Base::CSharedString CModelBaseHeadString(bool in_isSuper);
Hedgehog::Base::CSharedString CMaterialBodyString(bool in_isSuper);

Hedgehog::Base::CSharedString ArchiveHeadString();
Hedgehog::Base::CSharedString ArchiveBodyString();
Hedgehog::Base::CSharedString ArchiveShoeString();
Hedgehog::Base::CSharedString ArchiveHandRString();
Hedgehog::Base::CSharedString ArchiveHandLString();
Hedgehog::Base::CSharedString ArchiveEyelidString();
Hedgehog::Base::CSharedString ArchiveBaseHeadString();

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

class CObjCustomSonicBase
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
	bool m_isSuper{ false };

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

			arHeadData = LoadArchiveDatabase(ArchiveHeadString(), CModelHeadString(m_isSuper), "chr_sn_body_original", CMaterialBodyString(m_isSuper));
			arBaseHeadData = LoadArchiveDatabase(ArchiveBaseHeadString(), CModelBaseHeadString(m_isSuper), "chr_sn_body_original", CMaterialBodyString(m_isSuper));
			arEyelidData = LoadArchiveDatabase(ArchiveEyelidString(), CModelEyelidString(), "chr_sn_body_original", CMaterialBodyString(m_isSuper));

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

			arBodyData = LoadArchiveDatabase(ArchiveBodyString(), CModelBodyString(), "chr_sn_body_original", CMaterialBodyString(m_isSuper));

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

class CObjCustomSonicPlayer : public CObjCustomSonicBase, public Sonic::CGameObject3D
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
		auto playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		auto pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
		if (pPlayer == nullptr)
			return RemoveRenderables();
		auto pContext = pPlayer->GetContext();

		if (m_isSuper)
		{
			if (auto pSpRenderable = static_cast<Sonic::Player::CSonicSpRenderableSsn*>(m_pMessageManager->GetMessageActor(pContext->m_SuperRenderableActorID)))
			{
				m_spMatrixNode = pSpRenderable->m_spField160;

				if (auto pAnimationPose = pSpRenderable->GetAnimationPose())
					m_spPose = pAnimationPose->m_pInstanceInfo->m_spPose;
			}
		}
		else
		{
			m_spMatrixNode = pContext->m_spModelMatrixNode;
			m_spPose = pPlayer->m_spCharacterModel->m_spInstanceInfo->m_spPose;
		}
		
		bool isSonicSpinVisible{};

		if (auto pSonicSpin = static_cast<Sonic::Player::CSonicSpin*>(m_pMessageManager->GetMessageActor(pContext->m_SonicSpinActorID)))
			pSonicSpin->SendMessageSelfImm(Sonic::Message::MsgIsVisible(&isSonicSpinVisible));

		auto isModelEnabled = pPlayer->m_spCharacterModel->m_Enabled && !isSonicSpinVisible && !isJumpBallHide;

		if (m_spSnHead)
			m_spSnHead->m_Enabled = isModelEnabled; // (m_isSuper ? false : isModelEnabled);
		if (m_spSnBody)
			m_spSnBody->m_Enabled = isModelEnabled;
		if (m_spSnShoes)
			m_spSnShoes->m_Enabled = isModelEnabled;
		if (m_spSnHandR)
			m_spSnHandR->m_Enabled = isModelEnabled;
		if (m_spSnHandL)
			m_spSnHandL->m_Enabled = isModelEnabled;
		if (m_spSnEyelid)
			m_spSnEyelid->m_Enabled = isModelEnabled;
		if (m_spSnBaseHead)
			m_spSnBaseHead->m_Enabled = isModelEnabled;
		
		// printf("%s\n", isModelEnabled ? "SHOW MODEL" : "HIDE MODEL");
		// printf("%s\n", m_isSuper ? "SUPER SONIC TRUE" : "SUPER SONIC FALSE");

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

	void SetSuper(bool in_isSuper)
	{
		m_isSuper = in_isSuper;
	}

	void KillCallback() override
	{
		printf("KILL PLAYER RENDERABLE\n");
		RemoveRenderables();
	}
};
extern boost::shared_ptr<CObjCustomSonicPlayer> obj_CObjCustomSonicPlayer;

void CreateCustomizeSonicPlayerRenderable();
void KillCustomizeSonicPlayerRenderable();
void OnFrameHandler();
void ReadJson(std::string jsonFilePath, int cCat);
