#include <Common.h>
#include "UpdateDirector.h"
#include "CustomSonicItems.h"
#include "InstallCustomUI.h"
#include "InstallSetup.h"
#include "CustomUI.h"
#include "NPCAnim.h"
#include "CamMath.h"
#include "CustomSonicAPI.h"

//Debug Display setup
Chao::CSD::RCPtr<Chao::CSD::CProject> prDebugTxtScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugTxtVisSel;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugTxtScroll;
Chao::CSD::RCPtr<Chao::CSD::CScene> scDebugTxtTrueSel;
boost::shared_ptr<Sonic::CGameObjectCSD> obDebugTxtUI;
bool DebugUIExists = false;
bool DebugOpen = false;
int DebugSelect = 0;
int DebugCount = 9;
int DebugScrollCount = 0;
int DebugOptionSelect = 0;

void SetDebugDispText(int Select, Chao::CSD::RCPtr<Chao::CSD::CScene>& Scene, int Prefix)
{
	char value0[256]; sprintf(value0, "%d - Visual Selection", Select);
	char value1[256]; sprintf(value1, "%d - Scroll Height", Select);
	char value2[256]; sprintf(value2, "%d - True Selection", Select);

	switch (Prefix)
	{
	case 0:
		Scene->GetNode("Text_sdw")->SetText(value0);
		Scene->GetNode("Text_yellow")->SetText(value0);
		Scene->GetNode("Text_blue")->SetText(value0);
		break;
	case 1:
		Scene->GetNode("Text_sdw")->SetText(value1);
		Scene->GetNode("Text_yellow")->SetText(value1);
		Scene->GetNode("Text_blue")->SetText(value1);
		break;
	case 2:
		Scene->GetNode("Text_sdw")->SetText(value2);
		Scene->GetNode("Text_yellow")->SetText(value2);
		Scene->GetNode("Text_blue")->SetText(value2);
		break;
	}
	
}
void SetDebugText(const char* Text, Chao::CSD::RCPtr<Chao::CSD::CScene>& Scene)
{
	Scene->GetNode("Text_sdw")->SetText(Text);
	Scene->GetNode("Text_yellow")->SetText(Text);
	Scene->GetNode("Text_blue")->SetText(Text);
}

void CreateDebugTxtUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	if (obDebugTxtUI == nullptr)
	{
		auto spTxtCsdProject = wrapper.GetCsdProject("ui_fittingtext_bb");
		prDebugTxtScreen = spTxtCsdProject->m_rcProject;
		obDebugTxtUI = boost::make_shared<Sonic::CGameObjectCSD>(prDebugTxtScreen, 0.5f, "HUD_Pause", true);
		Sonic::CGameDocument::GetInstance()->AddGameObject(obDebugTxtUI, "main", This);
	}
}
void KillDebugTxtUI()
{
	if (obDebugTxtUI)
	{
		obDebugTxtUI->SendMessage(obDebugTxtUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obDebugTxtUI = nullptr;
	}
	DebugUIExists = false;
	DebugOpen = false;
}

//Declare Funcs
void ReadINI(std::string saveFilePath);
void WriteINI(FILE* iniFile);

//////Preview Renderable//////
static uint32_t pCAnimationStateMachineSetBlend = 0xCE0720;
static uint32_t pCNPCAnimationCtor = 0xB67750;

static void* fCAnimationStateMachineSetBlend(Sonic::CAnimationStateMachine* This,
	const Hedgehog::Base::CSharedString& in_rSourceState, const Hedgehog::Base::CSharedString& in_rDestinationState, float in_BlendTime)
{
	void* result = nullptr;

	__asm
	{
		push in_BlendTime
		mov ecx, This
		mov eax, in_rSourceState
		mov edi, in_rDestinationState
		call[pCAnimationStateMachineSetBlend]
		mov result, eax
	}

	return result;
}

class CustomizeSonicPreviewRenderable : public CustomizeSonicRenderable, public Sonic::CGameObject
{
public:
	boost::shared_ptr<Hedgehog::Mirage::CSingleElement> m_spSnEyes{};
	boost::shared_ptr<Hedgehog::Mirage::CSingleElement> m_spSsnEyes{};
	boost::shared_ptr<Sonic::CNPCAnimation> m_spNPCAnimation{};
	hh::math::CVector m_Position{};
	hh::math::CVector4 m_ScreenPosition{};
	hh::math::CQuaternion m_Rotation{};
	hh::math::CMatrix44 m_Transform{};
	bool m_isEyesLoaded{};

	////Animation List
	static inline hh::anim::SMotionInfo m_sAnimList[9]
	{
		{ "CAT_SHOES", "sn_fitting_cat_shoes_loop", 1.0f, 0 },
		{ "CAT_BODY", "sn_fitting_cat_body_loop", 1.0f, 0 },
		{ "CAT_HEAD", "sn_fitting_cat_head_loop", 1.0f, 0 },
		{ "CAT_HANDR", "sn_fitting_cat_hand_r_loop", 1.0f, 0 },
		{ "CAT_HANDL", "sn_fitting_cat_hand_l_loop", 1.0f, 0 },
		{ "CAT_MISC", "sn_fitting_cat_misc_loop", 1.0f, 0 },
		{ "FITTING", "sn_fitting_base_loop", 1.0f, 0 },
		{ "IDLE", "sn_fitting_idle_loop", 1.0f, 0 },
		{ "RUN", "sn_fitting_run_loop", 1.6f, 0 }
	};

	void SetAnimStateTransition(const char* in_pStartState, const char* in_pEndState, float in_TransitionSpeed)
	{
		auto* state = m_spNPCAnimation->m_spAnimationStateMachine->GetAnimationState(in_pStartState).get();
		state->m_TransitionState = in_pEndState;
		state->m_Field90 = true;
		state->m_Field8C = -1.0f;
		fCAnimationStateMachineSetBlend(m_spNPCAnimation->m_spAnimationStateMachine.get(), in_pEndState, in_pStartState, in_TransitionSpeed);
	}

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder, Sonic::CGameDocument* pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		pGameDocument->AddUpdateUnit("b", this);

		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* pContext = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		m_isSuper = pContext->m_pStateFlag->m_Flags[pContext->eStateFlag_InvokeSuperSonic];
		//printf("%s\n", m_isSuper ? "SUPER TRUE" : "SUPER FALSE");
		const char* strEyeModel = "chr_Sonic_HD";
		const char* strEyeSnModel = "chr_Sonic_HD";
		const char* strEyeSsnModel = "chr_SuperSonic_HD";

		if (m_isSuper)
			strEyeModel = strEyeSsnModel;
		else
			strEyeModel = strEyeSnModel;

		m_isCastShadows = false;

		// Load initial models.
		MsgRefreshCustomizeSonic msg{ SelectCategory::All };
		ProcessMessage(msg, false);

		////Setup model for Sonic's eyes
		hh::mr::CMirageDatabaseWrapper wrapper(in_spDatabase.get());
		boost::shared_ptr<hh::mr::CModelData> spModelData = wrapper.GetModelData(strEyeModel, 0);
		if (!spModelData)
			return;
		m_spSnEyes = boost::make_shared<hh::mr::CSingleElement>(spModelData);

		////Construct animator
		auto npcAnimation = reinterpret_cast<Sonic::CNPCAnimation*>(__HH_ALLOC(0x30));
		fCNPCAnimationCtor(npcAnimation);
		m_spNPCAnimation = boost::shared_ptr<Sonic::CNPCAnimation>(npcAnimation);

		//////Initialize Skeleton
		m_spNPCAnimation->Initialize(in_spDatabase, "chr_Sonic_HD");
		m_spNPCAnimation->NPC_ADD_ANIM_LIST(m_sAnimList);
		m_spNPCAnimation->m_spAnimationPose->Update(0.0f);
		
		//////Animation transitions
		SetAnimStateTransition("CAT_SHOES", "CAT_BODY", 0.07f);
		SetAnimStateTransition("CAT_SHOES", "CAT_MISC", 0.07f);
		SetAnimStateTransition("CAT_SHOES", "FITTING", 0.07f);
		SetAnimStateTransition("CAT_BODY", "CAT_HEAD", 0.07f);
		SetAnimStateTransition("CAT_BODY", "CAT_SHOES", 0.07f);
		SetAnimStateTransition("CAT_BODY", "FITTING", 0.07f);
		SetAnimStateTransition("CAT_HEAD", "CAT_HANDL", 0.07f);
		SetAnimStateTransition("CAT_HEAD", "CAT_BODY", 0.07f);
		SetAnimStateTransition("CAT_HEAD", "FITTING", 0.07f);
		SetAnimStateTransition("CAT_HANDL", "CAT_HANDR", 0.07f);
		SetAnimStateTransition("CAT_HANDL", "CAT_HEAD", 0.07f);
		SetAnimStateTransition("CAT_HANDL", "FITTING", 0.07f);
		SetAnimStateTransition("CAT_HANDR", "CAT_MISC", 0.07f);
		SetAnimStateTransition("CAT_HANDR", "CAT_HANDL", 0.07f);
		SetAnimStateTransition("CAT_HANDR", "FITTING", 0.07f);
		SetAnimStateTransition("CAT_MISC", "CAT_SHOES", 0.07f);
		SetAnimStateTransition("CAT_MISC", "CAT_HANDR", 0.07f);
		SetAnimStateTransition("CAT_MISC", "FITTING", 0.07f);
		SetAnimStateTransition("FITTING", "IDLE", 0.07f);
		SetAnimStateTransition("IDLE", "RUN", 0.07f);
		SetAnimStateTransition("RUN", "CAT_SHOES", 0.07f);
		SetAnimStateTransition("RUN", "CAT_BODY", 0.07f);
		SetAnimStateTransition("RUN", "CAT_HEAD", 0.07f);
		SetAnimStateTransition("RUN", "CAT_HANDL", 0.07f);
		SetAnimStateTransition("RUN", "CAT_HANDR", 0.07f);
		SetAnimStateTransition("RUN", "CAT_MISC", 0.07f);

		//////Start Animation
		switch (CHudTabSel)
		{
		case UIPartShoes:
			m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("CAT_SHOES");
			break;
		case UIPartBody:
			m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("CAT_BODY");
			break;
		case UIPartHead:
			m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("CAT_HEAD");
			break;
		case UIPartHandL:
			m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("CAT_HANDL");
			break;
		case UIPartHandR:
			m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("CAT_HANDR");
			break;
		case UIMiscOption:
			m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("CAT_MISC");
			break;
		}

		////Bind poses
		m_spSnEyes->BindPose(m_spNPCAnimation->m_spAnimationPose);
		m_spPose = m_spNPCAnimation->m_spAnimationPose;

		////Attach renderable to Sonic's animation
		//const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		//const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
		//m_spSnEyes->BindPose(pPlayer->m_spCharacterModel->m_spInstanceInfo->m_spPose);
		//m_spPose = pPlayer->m_spCharacterModel->m_spInstanceInfo->m_spPose;
	}

	void UpdateSerial(const Hedgehog::Universe::SUpdateInfo& in_rUpdateInfo) override
	{

		if (m_spSnHead != nullptr)
			m_spSnHead->m_Enabled = true;
		if (m_spSnBody != nullptr)
			m_spSnBody->m_Enabled = true;
		if (m_spSnShoes != nullptr)
			m_spSnShoes->m_Enabled = true;
		if (m_spSnHandR != nullptr)
			m_spSnHandR->m_Enabled = true;
		if (m_spSnHandL != nullptr)
			m_spSnHandL->m_Enabled = true;
		if (m_spSnEyelid != nullptr)
			m_spSnEyelid->m_Enabled = true;
		if (m_spSnBaseHead != nullptr)
			m_spSnBaseHead->m_Enabled = true;
		
		UpdateRenderables(this, "HUD_AfterModel");
		
		if (m_isHeadLoaded && !m_isEyesLoaded)
		{
			AddRenderable("HUD_AfterModel", m_spSnEyes, m_isCastShadows);
			m_isEyesLoaded = true;
		}
		
		m_spNPCAnimation->m_spAnimationPose->Update(in_rUpdateInfo.DeltaTime);
		m_spNPCAnimation->m_spAnimationStateMachine->UpdateStateMachine(in_rUpdateInfo);
		
		auto transformElement = [=](boost::shared_ptr<Hedgehog::Mirage::CSingleElement> in_spElement)
		{
			if (!in_spElement)
				return;
			
			const auto spCamera = m_pMember->m_pGameDocument->GetWorld()->GetCamera();
			const hh::math::CMatrix44 invProj = spCamera->m_MyCamera.m_Projection.inverse();
			const hh::math::CMatrix invView = spCamera->m_MyCamera.m_View.inverse();
			
			auto& rTransform = in_spElement->m_spInstanceInfo->m_Transform;
			auto& rMatrix = rTransform.matrix();
			auto previewProjection = Eigen::CreatePerspectiveMatrix<float>(DEGREES_TO_RADIANS(12.1), spCamera->m_MyCamera.m_AspectRatio, 0.1, 20);
			
			Eigen::Affine3f transform;
			transform = Eigen::Translation3f(0.0f, 0.0f, -10.0f);
			transform = transform * Eigen::AngleAxisf(DEGREES_TO_RADIANS(10.0f), Eigen::Vector3f::UnitX());
			transform = transform * Eigen::AngleAxisf(DEGREES_TO_RADIANS(PrevRotation), Eigen::Vector3f::UnitY());
			
			Eigen::Affine3f screenTransform;
			screenTransform = Eigen::Translation3f(-0.549f, -0.575f, 0.0f);
			
			rMatrix = transform.matrix();
			rMatrix = previewProjection * rMatrix;
			rMatrix = screenTransform * rMatrix;
			rMatrix = invProj * rMatrix;
			rMatrix = invView * rMatrix;
		};
		
		transformElement(m_spSnEyes);
		transformElement(m_spSnHead);
		transformElement(m_spSnBaseHead);
		transformElement(m_spSnBody);
		transformElement(m_spSnShoes);
		transformElement(m_spSnHandR);
		transformElement(m_spSnHandL);
		transformElement(m_spSnEyelid);

		//printf("%f\n", m_spNPCAnimation->m_spAnimationStateMachine->m_Time);
	}

	bool ProcessMessage(Hedgehog::Universe::Message& in_rMsg, bool in_Flag) override
	{
		if (in_rMsg.Is<MsgRefreshCustomizeSonic>())
		{
			auto& msgRefreshCustomizeSonic = static_cast<MsgRefreshCustomizeSonic&>(in_rMsg);
			
			RemoveRenderable("HUD_AfterModel", m_spSnEyes, true); //should be "HUD_OverlayModel", using "HUD_AfterModel" to overlap temporarily
			m_isEyesLoaded = false;

			RefreshModels(this, "HUD_AfterModel", msgRefreshCustomizeSonic.m_Category);

			printf("REFRESH PREVIEW MODELS: %d\n", int(msgRefreshCustomizeSonic.m_Category));
		}

		return true;
	}

	void ChangeAnimation(const char* in_anim)
	{
		m_spNPCAnimation->m_spAnimationStateMachine->ChangeState(in_anim);
	}

	void KillCallback() override
	{
		printf("KILL PREVIEW RENDERABLE\n");
		RemoveRenderables();
	}
};
boost::shared_ptr<CustomizeSonicPreviewRenderable> obj_CustomizeSonicPreviewRenderable;


//Menu Functions
void CHudUIPlayAnim(Chao::CSD::RCPtr<Chao::CSD::CScene> in_pScene, const char* in_pName, float in_pFrame, bool in_pMotionDisable, Chao::CSD::EMotionRepeatType in_pMotionRepeatType, float in_pSpeed)
{
	in_pScene->SetMotion(in_pName);
	in_pScene->SetMotionFrame(in_pFrame);
	in_pScene->m_MotionDisableFlag = in_pMotionDisable;
	in_pScene->m_MotionRepeatType = in_pMotionRepeatType;
	in_pScene->m_MotionSpeed = in_pSpeed;
	in_pScene->Update();
}

void CHudUISceneDestroy()
{
	if (scBBGui)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBGui);
	if (scBBIcon)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBIcon);
	if (scBBTextArea)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBTextArea);
	if (scBBBtmTextArea)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBBtmTextArea);
	if (scBBLRMove)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBLRMove);
	if (scBBBtmBtn)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBBtmBtn);
	if (scBBDeco)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBDeco);
	if (scBBScroll)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBScroll);
	if (scBBPrev)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBPrev);
	if (scBBPrevBtmTxt)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBPrevBtmTxt);
	if (scBBPrevBtmBtn)
		Chao::CSD::CProject::DestroyScene(prFittingScreenBB.Get(), scBBPrevBtmBtn);
	if (scSWABG1)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWABG1);
	if (scSWATag)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWATag);
	if (scSWATagTxt)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWATagTxt);
	if (scSWAPress)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAPress);
	if (scSWAFooter)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAFooter);
	if (scSWAArrow)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAArrow);
	if (scSWAAlt)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAAlt);
	if (scSWAScroll)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAScroll);
	if (scSWAScrollBG)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWAScrollBG);
	if (scSWASelect)
		Chao::CSD::CProject::DestroyScene(prFittingScreenSWA.Get(), scSWASelect);
	if (obj_CustomizeSonicPreviewRenderable)
		obj_CustomizeSonicPreviewRenderable->Kill();
}

void KillScreen()
{
	if (obBBCustomUI || obSWACustomUI)
	{
		FILE* pFile = fopen(saveFilePath.c_str(), "wb");
		WriteINI(pFile);
	}
	if (obBBCustomUI)
	{
		obBBCustomUI->SendMessage(obBBCustomUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obBBCustomUI = nullptr;
	}
	if (obSWACustomUI)
	{
		obSWACustomUI->SendMessage(obSWACustomUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obSWACustomUI = nullptr;
	}
}


//Handle UI
void CreateFittingUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
	//Sonic::CCsdDatabaseWrapper wrapper(database_ui.get());

	if (!IsUnleashedHUD)
	{
		auto spCsdProject = wrapper.GetCsdProject("ui_fitting_bb");
		if (spCsdProject->IsMadeAll())
		{
			prFittingScreenBB = spCsdProject->m_rcProject;
			obBBCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenBB, 0.5f, "HUD_A1", true);
			Sonic::CGameDocument::GetInstance()->AddGameObject(obBBCustomUI, "main", This);
		}
	}
	else
	{
		auto spCsdProject = wrapper.GetCsdProject("ui_fitting_swa_bb");
		if (spCsdProject->IsMadeAll())
		{
			prFittingScreenBB = spCsdProject->m_rcProject;
			obBBCustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenBB, 0.5f, "HUD_A1", true);
			Sonic::CGameDocument::GetInstance()->AddGameObject(obBBCustomUI, "main", This);
		}

		auto spCsdProjectSWA = wrapper.GetCsdProject("ui_fitting_swa");
		if (spCsdProjectSWA->IsMadeAll())
		{
			prFittingScreenSWA = spCsdProjectSWA->m_rcProject;
			obSWACustomUI = boost::make_shared<Sonic::CGameObjectCSD>(prFittingScreenSWA, 0.4f, "HUD_A1", true);
			Sonic::CGameDocument::GetInstance()->AddGameObject(obSWACustomUI, "main", This);
		}
	}
}

void CHudUIOpen(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo, float Sh, float Bd, float He, float HL, float HR, float SB)
{
	auto arX = 1280.0f;
	auto arY = 720.0f;
	auto scaleY = 0.88642f;
	auto rowX1 = 0.6885f;
	auto rowX2 = 0.7745f;
	auto rowX3 = 0.8611f;
	auto rowY1 = 0.3990f;
	auto rowY2 = 0.5515f;
	auto rowY3 = 0.7046f;

	CHudVarHeMaxScroll = ((((s_ItemDataHead.size() + 2) / 3 - 3) >= 1) ? ((s_ItemDataHead.size() + 2) / 3 - 3) : (0));
	CHudVarBdMaxScroll = ((((s_ItemDataBody.size() + 2) / 3 - 3) >= 1) ? ((s_ItemDataBody.size() + 2) / 3 - 3) : (0));
	CHudVarShMaxScroll = ((((s_ItemDataShoes.size() + 2) / 3 - 3) >= 1) ? ((s_ItemDataShoes.size() + 2) / 3 - 3) : (0));
	CHudVarHRMaxScroll = ((((s_ItemDataHandR.size() + 2) / 3 - 3) >= 1) ? ((s_ItemDataHandR.size() + 2) / 3 - 3) : (0));
	CHudVarHLMaxScroll = ((((s_ItemDataHandL.size() + 2) / 3 - 3) >= 1) ? ((s_ItemDataHandL.size() + 2) / 3 - 3) : (0));

	for (size_t i = 0; i < s_ItemDataHead.size(); i++)
	{
		s_ItemDataHead[i].AltSelect = 0;
	}
	for (size_t i = 0; i < s_ItemDataBody.size(); i++)
	{
		s_ItemDataBody[i].AltSelect = 0;
	}
	for (size_t i = 0; i < s_ItemDataShoes.size(); i++)
	{
		s_ItemDataShoes[i].AltSelect = 0;
	}
	for (size_t i = 0; i < s_ItemDataHandR.size(); i++)
	{
		s_ItemDataHandR[i].AltSelect = 0;
	}
	for (size_t i = 0; i < s_ItemDataHandL.size(); i++)
	{
		s_ItemDataHandL[i].AltSelect = 0;
	}

	ReadINI(saveFilePath);
	prevblur = *ENABLE_BLUR;
	*ENABLE_BLUR = false;
	if (MemoryOpenTimer <= 0)
	{
		CHudTabSel = UIPartShoes;
		CHudVarVisSel = 0;
		CHudVarScroll = 0;
		CHudVarTrueSel = 0;
	}
	if (!obBBCustomUI) //Create UI project if it doesn't exist
		CreateFittingUI(This, Edx, in_rUpdateInfo);

	//Main UI
	scBBGui = prFittingScreenBB->CreateScene("chara");
	CHudUIPlayAnim(scBBGui, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);

	//Cursor
	scBBIcon = prFittingScreenBB->CreateScene("icon");
	scBBIcon->SetPosition(rowX1 * arX, rowY1 * arY);
	CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);

	//Category Text
	scBBTextArea = prFittingScreenBB->CreateScene("textarea");
	scBBTextArea->SetPosition(0, 0);
	CHudUIPlayAnim(scBBTextArea, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 0.5f);

	//L/R Bumpers
	scBBLRMove = prFittingScreenBB->CreateScene("LRmove");
	if (IsUnleashedHUD)
		scBBLRMove->SetHideFlag(true);
	CHudUIPlayAnim(scBBLRMove, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 2.0f);

	//Guide Text
	scBBBtmTextArea = prFittingScreenBB->CreateScene("textarea");
	scBBBtmTextArea->SetPosition(0, 0.59f * arY);
	scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(6);
	CHudUIPlayAnim(scBBBtmTextArea, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 0.5f);

	//Guide Buttons
	scBBBtmBtn = prFittingScreenBB->CreateScene("LRmove");
	scBBBtmBtn->SetPosition(0, 0.59f * arY);
	scBBBtmBtn->GetNode("left")->SetPosition(-0.136f * arX, 0.13333334f * arY);
	scBBBtmBtn->GetNode("left_sdw")->SetPosition(10.0f * arX, 10.0f * arY);
	scBBBtmBtn->GetNode("left_arrow")->SetPosition(10.0f * arX, 10.0f * arY);
	scBBBtmBtn->GetNode("right")->SetPosition(0.016f * arX, 0.13333334f * arY);
	scBBBtmBtn->GetNode("right_sdw")->SetPosition(10.0f * arX, 10.0f * arY);
	scBBBtmBtn->GetNode("right_arrow")->SetPosition(10.0f * arX, 10.0f * arY);
	scBBBtmBtn->GetNode("left_button")->SetPatternIndex(2);
	scBBBtmBtn->GetNode("left_button_0001")->SetPatternIndex(2);
	scBBBtmBtn->GetNode("right_button")->SetPatternIndex(2);
	scBBBtmBtn->GetNode("right_button_0001")->SetPatternIndex(2);
	CHudUIPlayAnim(scBBBtmBtn, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 2.0f);

	//Scroll Bar
	scBBScroll = prFittingScreenBB->CreateScene("scroll");
	if (IsUnleashedHUD)
		scBBScroll->SetHideFlag(true);
	scBBScroll->SetMotion("scroll_Anim");
	switch (CHudTabSel)
	{
	case UIPartShoes:
		scBBScroll->SetMotionFrame(Sh);
		break;
	case UIPartBody:
		scBBScroll->SetMotionFrame(Bd);
		break;
	case UIPartHead:
		scBBScroll->SetMotionFrame(He);
		break;
	case UIPartHandL:
		scBBScroll->SetMotionFrame(HL);
		break;
	case UIPartHandR:
		scBBScroll->SetMotionFrame(HR);
		break;
	case UIMiscOption:
		scBBScroll->SetMotionFrame(SB);
		break;
	}
	scBBScroll->m_MotionSpeed = 0.0f;
	scBBScroll->Update();
	IsInScrollOpen = true;
	scBBScroll->SetPosition(0.9175f * arX, 0.5475f * arY);
	CHudUIPlayAnim(scBBScroll, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);

	//Model Preview
	scBBPrev = prFittingScreenBB->CreateScene("chara_view");
	scBBPrevBtmTxt = prFittingScreenBB->CreateScene("textarea");
	scBBPrevBtmBtn = prFittingScreenBB->CreateScene("LRmove");
	scBBPrevBtmTxt->SetPosition(-0.549f * arX, 0.59f * arY);
	scBBPrevBtmBtn->SetPosition(-0.549f * arX, 0.59f * arY);
	scBBPrevBtmBtn->GetNode("left")->SetPosition(-0.136f * arX, 0.13333334f * arY);
	scBBPrevBtmBtn->GetNode("left_sdw")->SetPosition(10.0f * arX, 10.0f * arY);
	scBBPrevBtmBtn->GetNode("left_arrow")->SetPosition(10.0f * arX, 10.0f * arY);
	scBBPrevBtmBtn->GetNode("right")->SetPosition(0.016f * arX, 0.13333334f * arY);
	scBBPrevBtmBtn->GetNode("right_sdw")->SetPosition(10.0f * arX, 10.0f * arY);
	scBBPrevBtmBtn->GetNode("right_arrow")->SetPosition(10.0f * arX, 10.0f * arY);
	scBBPrevBtmBtn->GetNode("left_button")->SetPatternIndex(0);
	scBBPrevBtmBtn->GetNode("left_button_0001")->SetPatternIndex(0);
	scBBPrevBtmBtn->GetNode("right_button")->SetPatternIndex(3);
	scBBPrevBtmBtn->GetNode("right_button_0001")->SetPatternIndex(3);
	scBBPrevBtmTxt->GetNode("textarea_textbox")->SetPatternIndex(8);
	if (IsPreviewOpen == true)
	{
		PrevOpenTimer = 5;
		CHudUIPlayAnim(scBBPrev, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
		CHudUIPlayAnim(scBBPrevBtmTxt, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 0.5f);
		CHudUIPlayAnim(scBBPrevBtmBtn, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 2.0f);

		obj_CustomizeSonicPreviewRenderable = boost::make_shared<CustomizeSonicPreviewRenderable>();
		Sonic::CGameDocument::GetInstance()->AddGameObject(obj_CustomizeSonicPreviewRenderable);
	}
	if (IsPreviewOpen == false)
	{
		CHudUIPlayAnim(scBBPrev, "Intro_Anim", 0.0f, true, Chao::CSD::eMotionRepeatType_PlayOnce, 0.0f);
		CHudUIPlayAnim(scBBPrevBtmTxt, "Intro_Anim", 0.0f, true, Chao::CSD::eMotionRepeatType_PlayOnce, 0.0f);
		CHudUIPlayAnim(scBBPrevBtmBtn, "Intro_Anim", 0.0f, true, Chao::CSD::eMotionRepeatType_PlayOnce, 0.0f);
	}

	//Decoration
	if (ConfigDecoEnable && !IsUnleashedHUD)
	{
		scBBDeco = prFittingScreenBB->CreateScene("deco");
		scBBDeco->SetPosition(0, 0);
		CHudUIPlayAnim(scBBDeco, "Color_change", 1.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 0.0f);
		CHudUIPlayAnim(scBBDeco, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
	}

	//Unleashed HUD
	if (IsUnleashedHUD)
	{
		//BG
		scSWABG1 = prFittingScreenSWA->CreateScene("bg_1");
		scSWABG1->SetPosition(0, 0.025 * arY);
		CHudUIPlayAnim(scSWABG1, "Size_Anim", 100.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 0.0f);
		CHudUIPlayAnim(scSWABG1, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.5f);

		//Tag
		scSWATag = prFittingScreenSWA->CreateScene("tag");
		scSWATag->SetPosition(0, 0.025 * arY);
		CHudUIPlayAnim(scSWATag, "Intro_3_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);

		//TagText
		scSWATagTxt = prFittingScreenSWA->CreateScene("tag_name_3");
		scSWATagTxt->SetPosition(0, 0.025 * arY);
		CHudUIPlayAnim(scSWATagTxt, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);

		//Footer
		scSWAFooter = prFittingScreenSWA->CreateScene("footer_B");
		scSWAFooter->SetHideFlag(true);
		scSWAFooter->SetPosition(0.5 * arX, 0.333 * arY);
		scSWAFooter->SetScale(0.575, 0.575);
		CHudUIPlayAnim(scSWAFooter, "Usual_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);

		//Arrows
		scSWAArrow = prFittingScreenSWA->CreateScene("arrow");
		scSWAArrow->SetPosition(0, 0.025 * arY);
		scSWAArrow->SetHideFlag(true);
		CHudUIPlayAnim(scSWAArrow, "DefaultAnim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);

		//SWACursor
		scSWASelect = prFittingScreenSWA->CreateScene("skill_select");
		scSWASelect->SetHideFlag(true);
		scSWASelect->SetPosition(rowX1 * arX, rowY1 * arY);
		CHudUIPlayAnim(scSWASelect, "Usual_Anim", 30.0f, false, Chao::CSD::eMotionRepeatType_Loop, 1.0f);

		//SWAAltButton
		scSWAAlt = prFittingScreenSWA->CreateScene("footer_A");
		scSWAAlt->SetHideFlag(true);
		scSWAAlt->SetPosition(0, 0.025 * arY);
		CHudUIPlayAnim(scSWAAlt, "Usual_Anim_2", 0.0f, false, Chao::CSD::eMotionRepeatType_Loop, 1.0f);

		//SWAScroll
		scSWAScroll = prFittingScreenSWA->CreateScene("scroll_bar");
		scSWAScroll->SetHideFlag(true);
		scSWAScroll->SetScale(1, scaleY);
		scSWAScroll->SetMotion("Size_Anim");
		switch (CHudTabSel)
		{
		case UIPartShoes:
			scSWAScroll->SetPosition(0, (0.107 + (Sh * (0.001 * (CHudVarShMaxScroll * 0.47)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarShMaxScroll);
			break;
		case UIPartBody:
			scSWAScroll->SetPosition(0, (0.107 + (Bd * (0.001 * (CHudVarBdMaxScroll * 0.95)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarBdMaxScroll);
			break;
		case UIPartHead:
			scSWAScroll->SetPosition(0, (0.107 + (He * (0.001 * (CHudVarHeMaxScroll * 1.06)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHeMaxScroll);
			break;
		case UIPartHandL:
			scSWAScroll->SetPosition(0, (0.107 + (HL * (0.001 * (CHudVarHLMaxScroll * 0.8)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHLMaxScroll);
			break;
		case UIPartHandR:
			scSWAScroll->SetPosition(0, (0.107 + (HR * (0.001 * (CHudVarHRMaxScroll * 0.685)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHRMaxScroll);
			break;
		}
		scSWAScroll->m_MotionDisableFlag = false;
		scSWAScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_Loop;
		scSWAScroll->m_MotionSpeed = 0.0f;
		scSWAScroll->Update();

		//SWAScrollBG
		scSWAScrollBG = prFittingScreenSWA->CreateScene("scroll_bar_bg");
		scSWAScrollBG->SetHideFlag(true);
		scSWAScrollBG->SetScale(1, scaleY);
		scSWAScrollBG->SetPosition(0, 0.107 * arY);
		CHudUIPlayAnim(scSWAScrollBG, "Scroll_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_Loop, 0.0f);
	}

	//Misc
	IsInMenu = true;
	CHudUISFXOpen();
	return;
}

void RefreshCustomizeSonic(SelectCategory in_category = SelectCategory::All)
{
	auto& currBodyData = s_ItemDataBody[SelectBodyData];
	auto  category = uint32_t(in_category);

	if (currBodyData.HideShoes != s_ItemDataBodyPrev.HideShoes)
		category |= uint32_t(SelectCategory::Shoes);
	if (currBodyData.HideHandR != s_ItemDataBodyPrev.HideHandR)
		category |= uint32_t(SelectCategory::HandR);
	if (currBodyData.HideHandL != s_ItemDataBodyPrev.HideHandL)
		category |= uint32_t(SelectCategory::HandL);

	MsgRefreshCustomizeSonic msgRefreshCustomizeSonic{ SelectCategory(category) };

	if (obj_CustomizeSonicPlayerRenderable)
		obj_CustomizeSonicPlayerRenderable->SendMessageImm<MsgRefreshCustomizeSonic>(obj_CustomizeSonicPlayerRenderable->m_ActorID, msgRefreshCustomizeSonic);

	if (obj_CustomizeSonicPreviewRenderable)
		obj_CustomizeSonicPreviewRenderable->SendMessageImm<MsgRefreshCustomizeSonic>(obj_CustomizeSonicPreviewRenderable->m_ActorID, msgRefreshCustomizeSonic);

	s_ItemDataBodyPrev = currBodyData;
}

void CHudUISelect()
{
	switch (CHudTabSel)
	{
	case UIPartShoes:
		if (CHudVarTrueSel >= s_ItemDataShoes.size())
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			SelectShoesData = CHudVarTrueSel;
			RefreshCustomizeSonic(SelectCategory::Shoes);
		}
		return;
		break;
	case UIPartBody:
		if (CHudVarTrueSel >= s_ItemDataBody.size())
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			SelectBodyData = CHudVarTrueSel;
			RefreshCustomizeSonic(SelectCategory::Body);
		}
		return;
		break;
	case UIPartHead:
		if (CHudVarTrueSel >= s_ItemDataHead.size())
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			SelectHeadData = CHudVarTrueSel;
			RefreshCustomizeSonic(SelectCategory::Head);
		}
		return;
		break;
	case UIPartHandL:
		if (CHudVarTrueSel >= s_ItemDataHandL.size())
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			SelectHandLData = CHudVarTrueSel;
			RefreshCustomizeSonic(SelectCategory::HandL);
		}
		return;
		break;
	case UIPartHandR:
		if (CHudVarTrueSel >= s_ItemDataHandR.size())
			CHudUISFXSelect(false);
		else
		{
			CHudUISFXSelect(true);
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			SelectHandRData = CHudVarTrueSel;
			RefreshCustomizeSonic(SelectCategory::HandR);
		}
		return;
		break;
	case UIMiscOption:
		if (CHudVarTrueSel >= int(SelectOptionType::Overflow01))
		{
			CHudUISFXSelect(false);
		}
		else
		{
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);

			switch ((SelectOptionType)CHudVarTrueSel)
			{
				case SelectOptionType::SnMaterial:
				{
					if (SelectSnMaterial != SelectSnMaterialType::Custom)
						SelectSnMaterial = SelectSnMaterialType(int(SelectSnMaterial) + 1);
					else
						SelectSnMaterial = SelectSnMaterialType::Default;

					CHudUISFXSelect(true);
					RefreshCustomizeSonic(SelectCategory::All);

					return;
				}
					
				case SelectOptionType::Eyelids:
				{
					if (SelectEyelid != SelectEyelidType::Skin)
						SelectEyelid = SelectEyelidType(int(SelectEyelid) + 1);
					else
						SelectEyelid = SelectEyelidType::Default;

					CHudUISFXSelect(true);
					RefreshCustomizeSonic(SelectCategory::All);

					return;
				}
					
				case SelectOptionType::SuperHead:
				{
					if (SelectSsnHead != SelectSsnHeadType::Upward)
						SelectSsnHead = SelectSsnHeadType(int(SelectSsnHead) + 1);
					else
						SelectSsnHead = SelectSsnHeadType::Default;

					CHudUISFXSelect(true);
					RefreshCustomizeSonic(SelectCategory::Head);

					return;
				}

				case SelectOptionType::SsnMaterial:
				{
					if (SelectSsnMaterial != SelectSsnMaterialType::Dark)
						SelectSsnMaterial = SelectSsnMaterialType(int(SelectSsnMaterial) + 1);
					else
						SelectSsnMaterial = SelectSsnMaterialType::Default;

					CHudUISFXSelect(true);
					RefreshCustomizeSonic(SelectCategory::All);

					return;
				}
					
				case SelectOptionType::JumpBall:
				{
					if (SelectJumpBall != SelectJumpBallType::NoBall)
						SelectJumpBall = SelectJumpBallType(int(SelectJumpBall) + 1);
					else
						SelectJumpBall = SelectJumpBallType::Default;

					CHudUISFXSelect(true);

					return;
				}
					
				case SelectOptionType::BounceBall:
				{
					if (IsBounceEnabled == true)
					{
						if (SelectBounceBall != SelectBounceBallType::NoVFX)
						{
							SelectBounceBall = SelectBounceBallType(int(SelectBounceBall) + 1);
						}
						else
						{
							SelectBounceBall = SelectBounceBallType::BAP;
						}
						
						CHudUISFXSelect(true);
					}
					else
					{
						CHudUISFXSelect(false);
					}

					return;
				}
			}
		}
	}
}

void CHudUIMove(int Type)
{
	CHudUISFXMove();
	CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
	switch (CHudTabSel)
	{
	case UIPartShoes:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll < CHudVarShMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			return;
			break;
		case 2: //Left
			switch (CHudVarVisSel)
			{
			case 0:
			case 3:
			case 6:
				CHudVarVisSel += 2;
				break;
			default:
				CHudVarVisSel--;
				break;
			}
			return;
			break;
		case 3: //Right
			switch (CHudVarVisSel)
			{
			case 2:
			case 5:
			case 8:
				CHudVarVisSel -= 2;
				break;
			default:
				CHudVarVisSel++;
				break;
			}
			return;
			break;
		}
		return;
		break;
	case UIPartBody:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll < CHudVarBdMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			return;
			break;
		case 2: //Left
			switch (CHudVarVisSel)
			{
			case 0:
			case 3:
			case 6:
				CHudVarVisSel += 2;
				break;
			default:
				CHudVarVisSel--;
				break;
			}
			return;
			break;
		case 3: //Right
			switch (CHudVarVisSel)
			{
			case 2:
			case 5:
			case 8:
				CHudVarVisSel -= 2;
				break;
			default:
				CHudVarVisSel++;
				break;
			}
			return;
			break;
		}
		return;
		break;
	case UIPartHead:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll < CHudVarHeMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			return;
			break;
		case 2: //Left
			switch (CHudVarVisSel)
			{
			case 0:
			case 3:
			case 6:
				CHudVarVisSel += 2;
				break;
			default:
				CHudVarVisSel--;
				break;
			}
			return;
			break;
		case 3: //Right
			switch (CHudVarVisSel)
			{
			case 2:
			case 5:
			case 8:
				CHudVarVisSel -= 2;
				break;
			default:
				CHudVarVisSel++;
				break;
			}
			return;
			break;
		}
		return;
		break;
	case UIPartHandL:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll < CHudVarHLMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			return;
			break;
		case 2: //Left
			switch (CHudVarVisSel)
			{
			case 0:
			case 3:
			case 6:
				CHudVarVisSel += 2;
				break;
			default:
				CHudVarVisSel--;
				break;
			}
			return;
			break;
		case 3: //Right
			switch (CHudVarVisSel)
			{
			case 2:
			case 5:
			case 8:
				CHudVarVisSel -= 2;
				break;
			default:
				CHudVarVisSel++;
				break;
			}
			return;
			break;
		}
		return;
		break;
	case UIPartHandR:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll < CHudVarHRMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			return;
			break;
		case 2: //Left
			switch (CHudVarVisSel)
			{
			case 0:
			case 3:
			case 6:
				CHudVarVisSel += 2;
				break;
			default:
				CHudVarVisSel--;
				break;
			}
			return;
			break;
		case 3: //Right
			switch (CHudVarVisSel)
			{
			case 2:
			case 5:
			case 8:
				CHudVarVisSel -= 2;
				break;
			default:
				CHudVarVisSel++;
				break;
			}
			return;
			break;
		}
		return;
		break;
	case UIMiscOption:
		switch (Type)
		{
		case 0: //Up
			switch (CHudVarVisSel)
			{
			case 0:
			case 1:
			case 2:
				if (CHudVarScroll <= 0)
				{
					CHudVarVisSel += 6;
					CHudVarScroll = CHudVarSBMaxScroll;
				}
				else if (CHudVarScroll > 0)
					CHudVarScroll--;
				break;
			default:
				CHudVarVisSel -= 3;
				break;
			}
			//CHudVarScroll--;
			return;
			break;
		case 1: //Down
			switch (CHudVarVisSel)
			{
			case 6:
			case 7:
			case 8:
				if (CHudVarScroll >= CHudVarSBMaxScroll)
				{
					CHudVarVisSel -= 6;
					CHudVarScroll = 0;
				}
				else if (CHudVarScroll < CHudVarSBMaxScroll)
					CHudVarScroll++;
				break;
			default:
				CHudVarVisSel += 3;
				break;
			}
			//CHudVarScroll++;
			return;
			break;
		case 2: //Left
			if (CHudVarVisSel <= 0 && CHudVarScroll <= 0)
			{
				CHudVarScroll = CHudVarSBMaxScroll;
				CHudVarVisSel = 8;
			}
			else if (CHudVarVisSel <= 0 && CHudVarScroll > 0)
			{
				CHudVarScroll--;
				CHudVarVisSel += 2;
			}
			else
				CHudVarVisSel--;
			return;
			break;
		case 3: //Right
			if (CHudVarVisSel >= 8 && CHudVarScroll >= CHudVarSBMaxScroll)
			{
				CHudVarScroll = 0;
				CHudVarVisSel = 0;
			}
			else if (CHudVarVisSel >= 8 && CHudVarScroll < CHudVarSBMaxScroll)
			{
				CHudVarScroll++;
				CHudVarVisSel -= 2;
			}
			else
				CHudVarVisSel++;
			return;
			break;
		}
		return;
		break;
	}
}

void CHudUIAlt()
{
	switch (CHudTabSel)
	{
	case UIPartShoes:
		if (!(CHudVarTrueSel >= s_ItemDataShoes.size()) && (s_ItemDataShoes[CHudVarTrueSel].AltCount >= 1))
		{
			if (!(s_ItemDataShoes[CHudVarTrueSel].AltSelect >= s_ItemDataShoes[CHudVarTrueSel].AltCount))
				s_ItemDataShoes[CHudVarTrueSel].AltSelect++;
			else
				s_ItemDataShoes[CHudVarTrueSel].AltSelect = 0;
			CHudUISFXAlt();
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			CHudUIPlayAnim(scBBBtmBtn, "Right_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			RefreshCustomizeSonic(SelectCategory::Shoes);
		}
		return;
		break;
	case UIPartBody:
		if (!(CHudVarTrueSel >= s_ItemDataBody.size()) && (s_ItemDataBody[CHudVarTrueSel].AltCount >= 1))
		{
			if (!(s_ItemDataBody[CHudVarTrueSel].AltSelect >= s_ItemDataBody[CHudVarTrueSel].AltCount))
				s_ItemDataBody[CHudVarTrueSel].AltSelect++;
			else
				s_ItemDataBody[CHudVarTrueSel].AltSelect = 0;
			CHudUISFXAlt();
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			CHudUIPlayAnim(scBBBtmBtn, "Right_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			RefreshCustomizeSonic(SelectCategory::Body);
		}
		return;
		break;
	case UIPartHead:
		if (!(CHudVarTrueSel >= s_ItemDataHead.size()) && (s_ItemDataHead[CHudVarTrueSel].AltCount >= 1))
		{
			if (!(s_ItemDataHead[CHudVarTrueSel].AltSelect >= s_ItemDataHead[CHudVarTrueSel].AltCount))
				s_ItemDataHead[CHudVarTrueSel].AltSelect++;
			else
				s_ItemDataHead[CHudVarTrueSel].AltSelect = 0;
			CHudUISFXAlt();
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			CHudUIPlayAnim(scBBBtmBtn, "Right_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			RefreshCustomizeSonic(SelectCategory::Head);
		}
		return;
		break;
	case UIPartHandR:
		if (!(CHudVarTrueSel >= s_ItemDataHandR.size()) && (s_ItemDataHandR[CHudVarTrueSel].AltCount >= 1))
		{
			if (!(s_ItemDataHandR[CHudVarTrueSel].AltSelect >= s_ItemDataHandR[CHudVarTrueSel].AltCount))
				s_ItemDataHandR[CHudVarTrueSel].AltSelect++;
			else
				s_ItemDataHandR[CHudVarTrueSel].AltSelect = 0;
			CHudUISFXAlt();
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			CHudUIPlayAnim(scBBBtmBtn, "Right_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			RefreshCustomizeSonic(SelectCategory::HandR);
		}
		return;
		break;
	case UIPartHandL:
		if (!(CHudVarTrueSel >= s_ItemDataHandL.size()) && (s_ItemDataHandL[CHudVarTrueSel].AltCount >= 1))
		{
			if (!(s_ItemDataHandL[CHudVarTrueSel].AltSelect >= s_ItemDataHandL[CHudVarTrueSel].AltCount))
				s_ItemDataHandL[CHudVarTrueSel].AltSelect++;
			else
				s_ItemDataHandL[CHudVarTrueSel].AltSelect = 0;
			CHudUISFXAlt();
			CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			CHudUIPlayAnim(scBBBtmBtn, "Right_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
			RefreshCustomizeSonic(SelectCategory::HandL);
		}
		return;
		break;
	}
}

void CHedUIPreview()
{
	if (IsPreviewOpen == false && PrevOpenTimer == 0)
	{
		CHudUIPlayAnim(scBBPrev, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.5f);
		CHudUIPlayAnim(scBBPrevBtmTxt, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 0.5f);
		CHudUIPlayAnim(scBBPrevBtmBtn, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 2.0f);
		PrevOpenTimer = 5;
		PrevAnim = 0;
		PrevCatAnim = true;
		PrevRotation = 0.0f;
		IsPreviewOpen = true;
		CHudUISFXOpen();

		obj_CustomizeSonicPreviewRenderable = boost::make_shared<CustomizeSonicPreviewRenderable>();
		Sonic::CGameDocument::GetInstance()->AddGameObject(obj_CustomizeSonicPreviewRenderable);
	}
	if (IsPreviewOpen == true && PrevOpenTimer == 0)
	{
		CHudUIPlayAnim(scBBPrev, "Intro_Anim", 25.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -2.0f);
		CHudUIPlayAnim(scBBPrevBtmTxt, "Intro_Anim", 17.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -0.5f);
		CHudUIPlayAnim(scBBPrevBtmBtn, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);
		PrevOpenTimer = 5;
		PrevAnim = 0;
		PrevCatAnim = true;
		PrevRotation = 0.0f;
		IsPreviewOpen = false;
		CHudUISFXExit();

		if (obj_CustomizeSonicPreviewRenderable)
			obj_CustomizeSonicPreviewRenderable->Kill();
	}
	CHudUIPlayAnim(scBBBtmBtn, "Left_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
}

void CHudUISwitch(int Type)
{
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedLB = input.IsTapped(Sonic::eKeyState_LeftBumper);
	bool PressedRB = input.IsTapped(Sonic::eKeyState_RightBumper);

	if (Type == 0)
	{
		CHudUISFXSwitch();
		IsInMenuChange = true;
		if (PressedRB)
		{
			IsInMenuChangeR = true;
			CHudUIPlayAnim(scBBLRMove, "Right_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
		}
		if (PressedLB)
		{
			IsInMenuChangeL = true;
			CHudUIPlayAnim(scBBLRMove, "Left_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
		}
		IsInMenuExit = false;
		SWAOpenTimer = 25;
		CHudUIPlayAnim(scBBGui, "Intro_Anim", 22.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -2.0f);
		CHudUIPlayAnim(scBBIcon, "OFF_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
		CHudUIPlayAnim(scBBTextArea, "Intro_Anim", 17.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.8f);
		CHudUIPlayAnim(scBBBtmTextArea, "Intro_Anim", 17.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.8f);
		CHudUIPlayAnim(scBBBtmBtn, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);
		IsInScrollOpen = true;
		CHudUIPlayAnim(scBBScroll, "Intro_Anim", 22.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -2.0f);
		if (IsUnleashedHUD)
		{
			CHudUIPlayAnim(scSWABG1, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -2.5f);
			CHudUIPlayAnim(scSWATag, "Intro_3_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);
			CHudUIPlayAnim(scSWATagTxt, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);
		}
		return;
	}
	if (Type == 1)
	{
		IsInMenuChange = false;
		CHudVarVisSel = 0;
		CHudVarScroll = 0;
		CHudVarTrueSel = 0;
		switch (CHudTabSel)
		{
		case UIPartShoes:
			if (IsInMenuChangeR)
			{
				CHudTabSel = UIPartBody;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_BODY");
			}
			if (IsInMenuChangeL)
			{
				CHudTabSel = UIMiscOption;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_MISC");
			}
			break;
		case UIPartBody:
			if (IsInMenuChangeR)
			{
				CHudTabSel = UIPartHead;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_HEAD");
			}
			if (IsInMenuChangeL)
			{
				CHudTabSel = UIPartShoes;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_SHOES");
			}
			break;
		case UIPartHead:
			if (IsInMenuChangeR)
			{
				CHudTabSel = UIPartHandL;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_HANDL");
			}
			if (IsInMenuChangeL)
			{
				CHudTabSel = UIPartBody;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_BODY");
			}
			break;
		case UIPartHandL:
			if (IsInMenuChangeR)
			{
				CHudTabSel = UIPartHandR;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_HANDR");
			}
			if (IsInMenuChangeL)
			{
				CHudTabSel = UIPartHead;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_HEAD");
			}
			break;
		case UIPartHandR:
			if (IsInMenuChangeR)
			{
				CHudTabSel = UIMiscOption;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_MISC");
			}
			if (IsInMenuChangeL)
			{
				CHudTabSel = UIPartHandL;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_HANDL");
			}
			break;
		case UIMiscOption:
			if (IsInMenuChangeR)
			{
				CHudTabSel = UIPartShoes;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_SHOES");
			}
			if (IsInMenuChangeL)
			{
				CHudTabSel = UIPartHandR;
				if (PrevCatAnim)
					obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_HANDR");
			}
			break;
		}
		IsInMenuChangeR = false;
		IsInMenuChangeL = false;
		CHudUIPlayAnim(scBBGui, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 2.0f);
		CHudUIPlayAnim(scBBIcon, "ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
		CHudUIPlayAnim(scBBTextArea, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 2.0f);
		CHudUIPlayAnim(scBBBtmTextArea, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 2.0f);
		CHudUIPlayAnim(scBBBtmBtn, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 4.0f);
		CHudUIPlayAnim(scBBScroll, "Intro_Anim", 15.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
		if (IsUnleashedHUD)
		{
			CHudUIPlayAnim(scSWABG1, "Intro_Anim", 100.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.5f);
			CHudUIPlayAnim(scSWATag, "Intro_3_Anim", 15.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.5f);
			CHudUIPlayAnim(scSWATagTxt, "Intro_Anim", 15.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.5f);
		}
		return;
	}
}

void CHudUIExit(int Type)
{
	if (Type == 0)
	{
		CHudUISFXExit();
		IsInMenuExit = true;
		SWAOpenTimer = 15;
		PrevOpenTimer = 5;
		PrevAnim = 0;
		PrevCatAnim = true;
		PrevRotation = 0.0f;

		CHudUIPlayAnim(scBBGui, "Intro_Anim", 22.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);
		CHudUIPlayAnim(scBBIcon, "OFF_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
		CHudUIPlayAnim(scBBTextArea, "Intro_Anim", 17.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -0.5f);
		CHudUIPlayAnim(scBBBtmTextArea, "Intro_Anim", 17.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -0.5f);
		CHudUIPlayAnim(scBBLRMove, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);
		CHudUIPlayAnim(scBBBtmBtn, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);
		IsInScrollOpen = true;
		CHudUIPlayAnim(scBBScroll, "Intro_Anim", 22.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);

		if (IsPreviewOpen == true)
		{
			CHudUIPlayAnim(scBBPrev, "Intro_Anim", 25.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);
			CHudUIPlayAnim(scBBPrevBtmTxt, "Intro_Anim", 17.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -0.5f);
			CHudUIPlayAnim(scBBPrevBtmBtn, "Intro_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);

			if (obj_CustomizeSonicPreviewRenderable)
				obj_CustomizeSonicPreviewRenderable->Kill();
		}

		if (scBBDeco)
			CHudUIPlayAnim(scBBDeco, "Intro_Anim", 23.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -1.0f);

		if (scSWABG1)
			CHudUIPlayAnim(scSWABG1, "Intro_Anim", 23.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -0.9f);
		
		if (scSWATag)
			CHudUIPlayAnim(scSWATag, "Intro_3_Anim", 15.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -0.5f);
		
		if (scSWATagTxt)
			CHudUIPlayAnim(scSWATagTxt, "Intro_Anim", 15.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, -0.5f);
		
		if (scSWAFooter)
		{
			SWAOpenTimer = 15;
			scSWAFooter->SetHideFlag(true);
			scSWAFooter->Update();
		}

		if (scSWAArrow)
		{
			SWAOpenTimer = 15;
			scSWAArrow->SetHideFlag(true);
			scSWAArrow->Update();
		}

		if (scSWAAlt)
		{
			SWAOpenTimer = 15;
			scSWAAlt->SetHideFlag(true);
			scSWAAlt->Update();
		}

		if (scSWASelect)
		{
			SWAOpenTimer = 15;
			scSWASelect->SetHideFlag(true);
			scSWASelect->Update();
		}

		if (scSWAScroll)
		{
			SWAOpenTimer = 15;
			scSWAScroll->SetHideFlag(true);
			scSWAScroll->Update();
		}

		if (scSWAScrollBG)
		{
			SWAOpenTimer = 15;
			scSWAScrollBG->SetHideFlag(true);
			scSWAScrollBG->Update();
		}

		return;
	}
	if (Type == 1)
	{
		IsInMenuExit = false;
		IsInMenu = false;
		MemoryOpenTimer = 1800;
		*ENABLE_BLUR = prevblur;
		FILE* pFile = fopen(saveFilePath.c_str(), "wb");
		WriteINI(pFile);
		CHudUISceneDestroy();
		return;
	}
}

void CHudUITexsetData(Sonic::CGameObject* This, int texSetID, const char* texString)
{
	auto texList = static_cast<Sonic::CCsdTexListMirage*>(prFittingScreenBB->m_rcTexList.Get());
	auto ui_cat_item_alt_sel = boost::make_shared<hh::mr::CTextureData>();
	ui_cat_item_alt_sel->m_spPictureData = hh::mr::CMirageDatabaseWrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get()).GetPictureData(texString);

	if (ui_cat_item_alt_sel->m_spPictureData == nullptr)
		ui_cat_item_alt_sel->m_spPictureData = hh::mr::CMirageDatabaseWrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get()).GetPictureData("ui_NotFound");
	
	texList->m_spTexsetData->m_TextureList[texSetID] = ui_cat_item_alt_sel;
}

const char* CHudUIThumbHeadString(int id, char* result)
{
	if (((CHudVarScroll * 3) + id) >= s_ItemDataHead.size())
	{
		sprintf(result, "ui_Null");
		return result;
	}
	auto mapAltCount = s_ItemDataHead[(CHudVarScroll * 3) + id].AltCount;
	auto mapChar = s_ItemDataHead[(CHudVarScroll * 3) + id].Name;
	auto mapInt = s_ItemDataHead[(CHudVarScroll * 3) + id].AltSelect;
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	bool active = ((CHudVarScroll * 3) + id == SelectHeadData);
	if (mapAltCount >= 1)
		sprintf(result, "ui_%s_%02d%s", mapChar.c_str(), mapInt, active ? texExtOn : texExtOff);
	else
		sprintf(result, "ui_%s%s", mapChar.c_str(), active ? texExtOn : texExtOff);
	return result;
}
const char* CHudUIThumbBodyString(int id, char* result)
{
	if (((CHudVarScroll * 3) + id) >= s_ItemDataBody.size())
	{
		sprintf(result, "ui_Null");
		return result;
	}
	auto mapAltCount = s_ItemDataBody[(CHudVarScroll * 3) + id].AltCount;
	auto mapChar = s_ItemDataBody[(CHudVarScroll * 3) + id].Name;
	auto mapInt = s_ItemDataBody[(CHudVarScroll * 3) + id].AltSelect;
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	bool active = ((CHudVarScroll * 3) + id == SelectBodyData);
	if (mapAltCount >= 1)
		sprintf(result, "ui_%s_%02d%s", mapChar.c_str(), mapInt, active ? texExtOn : texExtOff);
	else
		sprintf(result, "ui_%s%s", mapChar.c_str(), active ? texExtOn : texExtOff);
	return result;
}
const char* CHudUIThumbShoeString(int id, char* result)
{
	if (((CHudVarScroll * 3) + id) >= s_ItemDataShoes.size())
	{
		sprintf(result, "ui_Null");
		return result;
	}
	auto mapAltCount = s_ItemDataShoes[(CHudVarScroll * 3) + id].AltCount;
	auto mapChar = s_ItemDataShoes[(CHudVarScroll * 3) + id].Name;
	auto mapInt = s_ItemDataShoes[(CHudVarScroll * 3) + id].AltSelect;
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	bool active = ((CHudVarScroll * 3) + id == SelectShoesData);
	if (mapAltCount >= 1)
		sprintf(result, "ui_%s_%02d%s", mapChar.c_str(), mapInt, active ? texExtOn : texExtOff);
	else
		sprintf(result, "ui_%s%s", mapChar.c_str(), active ? texExtOn : texExtOff);
	return result;
}
const char* CHudUIThumbHandRString(int id, char* result)
{
	if (((CHudVarScroll * 3) + id) >= s_ItemDataHandR.size())
	{
		sprintf(result, "ui_Null");
		return result;
	}
	auto mapAltCount = s_ItemDataHandR[(CHudVarScroll * 3) + id].AltCount;
	auto mapChar = s_ItemDataHandR[(CHudVarScroll * 3) + id].Name;
	auto mapInt = s_ItemDataHandR[(CHudVarScroll * 3) + id].AltSelect;
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	bool active = ((CHudVarScroll * 3) + id == SelectHandRData);
	if (mapAltCount >= 1)
		sprintf(result, "ui_%s_%02d%s", mapChar.c_str(), mapInt, active ? texExtOn : texExtOff);
	else
		sprintf(result, "ui_%s%s", mapChar.c_str(), active ? texExtOn : texExtOff);
	return result;
}
const char* CHudUIThumbHandLString(int id, char* result)
{
	if (((CHudVarScroll * 3) + id) >= s_ItemDataHandL.size())
	{
		sprintf(result, "ui_Null");
		return result;
	}
	auto mapAltCount = s_ItemDataHandL[(CHudVarScroll * 3) + id].AltCount;
	auto mapChar = s_ItemDataHandL[(CHudVarScroll * 3) + id].Name;
	auto mapInt = s_ItemDataHandL[(CHudVarScroll * 3) + id].AltSelect;
	const char* texExtOn = "_On";
	const char* texExtOff = "_Off";
	bool active = ((CHudVarScroll * 3) + id == SelectHandLData);
	if (mapAltCount >= 1)
		sprintf(result, "ui_%s_%02d%s", mapChar.c_str(), mapInt, active ? texExtOn : texExtOff);
	else
		sprintf(result, "ui_%s%s", mapChar.c_str(), active ? texExtOn : texExtOff);
	return result;
}

const char* CHudUIThumbOptionsString(int id, char* result)
{
	if (((CHudVarScroll * 3) + id) >= 6)
	{
		sprintf(result, "ui_Null");
		return result;
	}

	auto mapChar = SelectOptionNameMap[SelectOptionType(int(SelectOptionType::SnMaterial) + (CHudVarScroll * 3) + id)];
	auto mapInt = (int(SelectOptionType::SnMaterial) + (CHudVarScroll * 3) + id);
	const char* texExtUI = "ui_";

	if (mapChar == SelectOptionNameMap[SelectOptionType(int(SelectOptionType::SnMaterial) + (CHudVarScroll * 3))])
		sprintf(result, "%s%s_%02d", texExtUI, mapChar, SelectSnMaterial);
	else if (mapChar == SelectOptionNameMap[SelectOptionType(int(SelectOptionType::Eyelids) + (CHudVarScroll * 3))])
		sprintf(result, "%s%s_%02d", texExtUI, mapChar, SelectEyelid);
	else if (mapChar == SelectOptionNameMap[SelectOptionType(int(SelectOptionType::SuperHead) + (CHudVarScroll * 3))])
		sprintf(result, "%s%s_%02d", texExtUI, mapChar, SelectSsnHead);
	else if (mapChar == SelectOptionNameMap[SelectOptionType(int(SelectOptionType::SsnMaterial) + (CHudVarScroll * 3))])
		sprintf(result, "%s%s_%02d", texExtUI, mapChar, SelectSsnMaterial);
	else if (mapChar == SelectOptionNameMap[SelectOptionType(int(SelectOptionType::JumpBall) + (CHudVarScroll * 3))])
		sprintf(result, "%s%s_%02d", texExtUI, mapChar, SelectJumpBall);
	else if (mapChar == SelectOptionNameMap[SelectOptionType(int(SelectOptionType::BounceBall) + (CHudVarScroll * 3))])
	{
		if (IsBounceEnabled == true)
			sprintf(result, "%s%s_%02d", texExtUI, mapChar, SelectBounceBall);
		else
			sprintf(result, "ui_Null");
	}
	else
		sprintf(result, "ui_Null");
	
	
	//sprintf(result, "%s%s_%02d", texExtUI, mapChar, 00);
	return result;
}

void CHudUIThumbManager(Sonic::CGameObject* This)
{
	char ShBuffer[256];
	char BdBuffer[256];
	char HeBuffer[256];
	char HLBuffer[256];
	char HRBuffer[256];
	char SBBuffer[256];

	switch (CHudTabSel)
	{
	case UIPartShoes:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbShoeString(i, ShBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartShoes);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartShoes);
		return;
		break;
	case UIPartBody:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbBodyString(i, BdBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartBody);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartBody);
		return;
		break;
	case UIPartHead:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbHeadString(i, HeBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartHead);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartHead);
		return;
		break;
	case UIPartHandL:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbHandLString(i, HLBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartHandL);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartHandL);
		return;
		break;
	case UIPartHandR:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbHandRString(i, HRBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIPartHandR);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIPartHandR);
		return;
		break;
	case UIMiscOption:
		for (int i = 0; i <= 8; i++)
			CHudUITexsetData(This, i + 6, CHudUIThumbOptionsString(i, SBBuffer));
		scBBTextArea->GetNode("textarea_textbox")->SetPatternIndex(UIMiscOption);
		if (IsUnleashedHUD)
			scSWATagTxt->GetNode("img")->SetPatternIndex(UIMiscOption);
		return;
		break;
	}

}

void CHudUIScrollManager(Sonic::CGameObject* This, float Sh, float Bd, float He, float HL, float HR, float SB)
{
	auto arY = 720.0f;
	auto scaleY = 0.88642f;
	scBBScroll->SetMotion("scroll_Anim");
	switch (CHudTabSel)
	{
	case UIPartShoes:
		scBBScroll->SetMotionFrame(Sh);
		break;
	case UIPartBody:
		scBBScroll->SetMotionFrame(Bd);
		break;
	case UIPartHead:
		scBBScroll->SetMotionFrame(He);
		break;
	case UIPartHandL:
		scBBScroll->SetMotionFrame(HL);
		break;
	case UIPartHandR:
		scBBScroll->SetMotionFrame(HR);
		break;
	case UIMiscOption:
		scBBScroll->SetMotionFrame(SB);
		break;
	}
	scBBScroll->m_MotionSpeed = 0.0f;
	scBBScroll->Update();
	if (IsUnleashedHUD)
	{
		switch (CHudTabSel)
		{
		case UIPartShoes:
			scSWAScroll->SetPosition(0, (0.107 + (Sh * (0.001 * (CHudVarShMaxScroll * 0.47)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarShMaxScroll);
			break;
		case UIPartBody:
			scSWAScroll->SetPosition(0, (0.107 + (Bd * (0.001 * (CHudVarBdMaxScroll * 0.95)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarBdMaxScroll);
			break;
		case UIPartHead:
			scSWAScroll->SetPosition(0, (0.107 + (He * (0.001 * (CHudVarHeMaxScroll * 1.06)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHeMaxScroll);
			break;
		case UIPartHandL:
			scSWAScroll->SetPosition(0, (0.107 + (HL * (0.001 * (CHudVarHLMaxScroll * 0.8)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHLMaxScroll);
			break;
		case UIPartHandR:
			scSWAScroll->SetPosition(0, (0.107 + (HR * (0.001 * (CHudVarHRMaxScroll * 0.685)))) * arY);
			scSWAScroll->SetMotionFrame(100.f / CHudVarHRMaxScroll);
			break;
		case UIMiscOption:
			scSWAScroll->SetPosition(0, 0.107 * arY);
			scSWAScroll->SetMotionFrame(0);
			break;
		}
	}
}

//Update Fitting Menu
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
	bool PressedLST = input.IsTapped(Sonic::eKeyState_LeftStick);
	bool PressedRST = input.IsTapped(Sonic::eKeyState_RightStick);
	bool DownLB = input.IsDown(Sonic::eKeyState_LeftBumper);
	bool DownRB = input.IsDown(Sonic::eKeyState_RightBumper);
	bool DownLT = input.IsDown(Sonic::eKeyState_LeftTrigger);
	bool DownRT = input.IsDown(Sonic::eKeyState_RightTrigger);
	bool PushedLSUp = inputPtr->LeftStickVertical >= 0.5f;
	bool PushedLSDown = inputPtr->LeftStickVertical <= -0.5f;
	bool PushedLSLeft = inputPtr->LeftStickHorizontal <= -0.5f;
	bool PushedLSRight = inputPtr->LeftStickHorizontal >= 0.5f;
	auto LSHor = inputPtr->LeftStickHorizontal;
	auto LSVer = inputPtr->LeftStickVertical;
	auto RSHor = inputPtr->RightStickHorizontal;
	auto RSVer = inputPtr->RightStickVertical;
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
	auto scenecheck = scBBIcon && scBBGui && scBBTextArea && scBBLRMove && scBBScroll;
	FUNCTION_PTR(void, __thiscall, changeState, 0x773250, void* This, boost::shared_ptr<void>&spState, const Hedgehog::Base::CSharedString name);
	boost::shared_ptr<void> spState;

	bool PressedActivation =
		(ActivateButton == 0 && PressedSL) ||
		(ActivateButton == 1 && PressedLST) ||
		(ActivateButton == 2 && PressedRST) ||
		(ActivateButton == 3 && DownLT && DownRT) ||
		(ActivateButton == 4 && DownLB && DownRB) ||
		(ActivateButton == 5 && PressedUp) ||
		(ActivateButton == 6 && PressedDown) ||
		(ActivateButton == 7 && PressedLeft) ||
		(ActivateButton == 8 && PressedRight);

	CHudVarTrueSel = CHudVarVisSel + (CHudVarScroll * 3);
	float CHudVarScrollBarStarSpin = (CHudVarScrollBarStarSpinFlt >= 360 ? CHudVarScrollBarStarSpinFlt = 0 : CHudVarScrollBarStarSpinFlt += 1.5);
	float CHudVarShScrollBar = (CHudVarScroll * (100.f / CHudVarShMaxScroll));
	float CHudVarBdScrollBar = (CHudVarScroll * (100.f / CHudVarBdMaxScroll));
	float CHudVarHeScrollBar = (CHudVarScroll * (100.f / CHudVarHeMaxScroll));
	float CHudVarHLScrollBar = (CHudVarScroll * (100.f / CHudVarHLMaxScroll));
	float CHudVarHRScrollBar = (CHudVarScroll * (100.f / CHudVarHRMaxScroll));
	float CHudVarSBScrollBar = (CHudVarScroll * (100.f / CHudVarSBMaxScroll));


	////------Open Fitting Menu------////
	if (PressedActivation && !IsInMenu && IsModernSonic && !ExternalControl && !Goal && !((getPackedID == pam000) && IsOutOfControl) && (IsLookAt == false))
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
			CHudUIOpen(This, Edx, in_rUpdateInfo, CHudVarShScrollBar, CHudVarBdScrollBar, CHudVarHeScrollBar, CHudVarHLScrollBar, CHudVarHRScrollBar, CHudVarSBScrollBar);
			break;
		}
	}


	////------Main Fitting Menu------////
	if (IsInMenu && scBBIcon && scBBGui && IsModernSonic && !ExternalControl && !Goal && !((getPackedID == pam000) && IsOutOfControl))
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

			////------Automatics Handling------////

			////------Handle Cursor Position
			switch (CHudVarVisSel)
			{
			case 0:
				scBBIcon->SetPosition(rowX1 * arX, rowY1 * arY);
				break;
			case 1:
				scBBIcon->SetPosition(rowX2 * arX, rowY1 * arY);
				break;
			case 2:
				scBBIcon->SetPosition(rowX3 * arX, rowY1 * arY);
				break;
			case 3:
				scBBIcon->SetPosition(rowX1 * arX, rowY2 * arY);
				break;
			case 4:
				scBBIcon->SetPosition(rowX2 * arX, rowY2 * arY);
				break;
			case 5:
				scBBIcon->SetPosition(rowX3 * arX, rowY2 * arY);
				break;
			case 6:
				scBBIcon->SetPosition(rowX1 * arX, rowY3 * arY);
				break;
			case 7:
				scBBIcon->SetPosition(rowX2 * arX, rowY3 * arY);
				break;
			case 8:
				scBBIcon->SetPosition(rowX3 * arX, rowY3 * arY);
				break;
			}
			if (IsUnleashedHUD)
			{
				switch (CHudVarVisSel)
				{
				case 0:
					scSWASelect->SetPosition(rowX1 * arX, rowY1 * arY);
					break;
				case 1:
					scSWASelect->SetPosition(rowX2 * arX, rowY1 * arY);
					break;
				case 2:
					scSWASelect->SetPosition(rowX3 * arX, rowY1 * arY);
					break;
				case 3:
					scSWASelect->SetPosition(rowX1 * arX, rowY2 * arY);
					break;
				case 4:
					scSWASelect->SetPosition(rowX2 * arX, rowY2 * arY);
					break;
				case 5:
					scSWASelect->SetPosition(rowX3 * arX, rowY2 * arY);
					break;
				case 6:
					scSWASelect->SetPosition(rowX1 * arX, rowY3 * arY);
					break;
				case 7:
					scSWASelect->SetPosition(rowX2 * arX, rowY3 * arY);
					break;
				case 8:
					scSWASelect->SetPosition(rowX3 * arX, rowY3 * arY);
					break;
				}
			}

			////------Icon Handler
			if (scenecheck)
				CHudUIThumbManager(This);

			////------Scrollbar Handler
			if (scBBScroll->m_MotionFrame >= scBBScroll->m_MotionEndFrame)
				IsInScrollOpen = false;
			if (scenecheck && IsInScrollOpen == false)
			{
				scBBScroll->GetNode("star")->SetRotation(CHudVarScrollBarStarSpin);
				CHudUIScrollManager(This, CHudVarShScrollBar, CHudVarBdScrollBar, CHudVarHeScrollBar, CHudVarHLScrollBar, CHudVarHRScrollBar, CHudVarSBScrollBar);
			}
				
			////------SWA Open Timer Unhide
			if (scSWAFooter)
			{
				if (SWAOpenTimer <= 0)
					scSWAFooter->SetHideFlag(false);
				else
					scSWAFooter->SetHideFlag(true);
			}
			if (scSWAArrow)
			{
				if (SWAOpenTimer <= 0)
					scSWAArrow->SetHideFlag(false);
				else
					scSWAArrow->SetHideFlag(true);
			}
			if (scSWAAlt)
			{
				if (SWAOpenTimer <= 0)
					scSWAAlt->SetHideFlag(false);
				else
					scSWAAlt->SetHideFlag(true);
			}
			if (scSWASelect)
			{
				if (SWAOpenTimer <= 0)
					scSWASelect->SetHideFlag(false);
				else
					scSWASelect->SetHideFlag(true);
			}
			if (scSWAScroll)
			{
				if (SWAOpenTimer <= 0)
					scSWAScroll->SetHideFlag(false);
				else
					scSWAScroll->SetHideFlag(true);
			}
			if (scSWAScrollBG)
			{
				if (SWAOpenTimer <= 0)
					scSWAScrollBG->SetHideFlag(false);
				else
					scSWAScrollBG->SetHideFlag(true);
			}

			////------Preview Handler
			if (scBBPrev)
			{
				if (scBBPrev->m_MotionFrame <= 0 && IsPreviewOpen == false)
				{
					CHudUIPlayAnim(scBBPrev, "Intro_Anim", 0.0f, true, Chao::CSD::eMotionRepeatType_PlayOnce, 0.0f);
					CHudUIPlayAnim(scBBPrevBtmTxt, "Intro_Anim", 0.0f, true, Chao::CSD::eMotionRepeatType_PlayOnce, 0.0f);
					CHudUIPlayAnim(scBBPrevBtmBtn, "Intro_Anim", 0.0f, true, Chao::CSD::eMotionRepeatType_PlayOnce, 0.0f);
					scBBPrev->SetHideFlag(true);
				}
				else
					scBBPrev->SetHideFlag(false);

				////-----Right Stick Handle
				PrevRotation += (RSHor * 2.0f);

				if (PressedRST && IsPreviewOpen && PrevOpenTimer == 0)
				{
					PrevRotation = 0.0f;
					CHudUIPlayAnim(scBBPrevBtmBtn, "Right_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
				}
				if (IsPreviewOpen == false)
					PrevRotation = 0.0f;

				if (PressedLST && IsPreviewOpen)
				{
					switch (PrevAnim)
					{
					case 0:
						obj_CustomizeSonicPreviewRenderable->ChangeAnimation("FITTING");
						PrevCatAnim = false;
						PrevAnim = 1;
						break;
					case 1:
						obj_CustomizeSonicPreviewRenderable->ChangeAnimation("IDLE");
						PrevCatAnim = false;
						PrevAnim = 2;
						break;
					case 2:
						obj_CustomizeSonicPreviewRenderable->ChangeAnimation("RUN");
						PrevCatAnim = false;
						PrevAnim = 3;
						break;
					case 3:
						switch (CHudTabSel)
						{
						case UIPartShoes:
							obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_SHOES");
							break;
						case UIPartBody:
							obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_BODY");
							break;
						case UIPartHead:
							obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_HEAD");
							break;
						case UIPartHandL:
							obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_HANDL");
							break;
						case UIPartHandR:
							obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_HANDR");
							break;
						case UIMiscOption:
							obj_CustomizeSonicPreviewRenderable->ChangeAnimation("CAT_MISC");
							break;
						}
						PrevCatAnim = true;
						PrevAnim = 0;
						break;
					}
					CHudUISFXSwitch();
					CHudUIPlayAnim(scBBPrevBtmBtn, "Left_ON_Anim", 0.0f, false, Chao::CSD::eMotionRepeatType_PlayOnce, 1.0f);
				}
			}

			////------Handle Alt Prompt
			switch (CHudTabSel)
			{
			case UIPartShoes:
				if (!(CHudVarTrueSel >= s_ItemDataShoes.size()) && (s_ItemDataShoes[CHudVarTrueSel].AltCount >= 1))
				{
					scBBIcon->GetNode("star")->SetPatternIndex(1);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(7);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(false);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(false);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(false);
				}
				else
				{
					scBBIcon->GetNode("star")->SetPatternIndex(0);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(6);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(true);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(true);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(true);
				}
				break;
			case UIPartBody:
				if (!(CHudVarTrueSel >= s_ItemDataBody.size()) && (s_ItemDataBody[CHudVarTrueSel].AltCount >= 1))
				{
					scBBIcon->GetNode("star")->SetPatternIndex(1);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(7);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(false);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(false);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(false);
				}
				else
				{
					scBBIcon->GetNode("star")->SetPatternIndex(0);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(6);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(true);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(true);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(true);
				}
				break;
			case UIPartHandR:
				if (!(CHudVarTrueSel >= s_ItemDataHandR.size()) && (s_ItemDataHandR[CHudVarTrueSel].AltCount >= 1))
				{
					scBBIcon->GetNode("star")->SetPatternIndex(1);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(7);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(false);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(false);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(false);
				}
				else
				{
					scBBIcon->GetNode("star")->SetPatternIndex(0);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(6);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(true);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(true);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(true);
				}
				break;
			case UIPartHandL:
				if (!(CHudVarTrueSel >= s_ItemDataHandL.size()) && (s_ItemDataHandL[CHudVarTrueSel].AltCount >= 1))
				{
					scBBIcon->GetNode("star")->SetPatternIndex(1);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(7);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(false);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(false);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(false);
				}
				else
				{
					scBBIcon->GetNode("star")->SetPatternIndex(0);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(6);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(true);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(true);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(true);
				}
				break;
			case UIPartHead:
				if (!(CHudVarTrueSel >= s_ItemDataHead.size()) && (s_ItemDataHead[CHudVarTrueSel].AltCount >= 1))
				{
					scBBIcon->GetNode("star")->SetPatternIndex(1);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(7);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(false);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(false);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(false);
				}
				else
				{
					scBBIcon->GetNode("star")->SetPatternIndex(0);
					scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(6);
					scBBBtmBtn->GetNode("right_button")->SetHideFlag(true);
					scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(true);
					if (IsUnleashedHUD && SWAOpenTimer <= 0)
						scSWAAlt->SetHideFlag(true);
				}
				break;
			default:
				scBBIcon->GetNode("star")->SetPatternIndex(0);
				scBBBtmTextArea->GetNode("textarea_textbox")->SetPatternIndex(6);
				scBBBtmBtn->GetNode("right_button")->SetHideFlag(true);
				scBBBtmBtn->GetNode("right_button_0001")->SetHideFlag(true);
				if (IsUnleashedHUD && SWAOpenTimer <= 0)
					scSWAAlt->SetHideFlag(true);
				break;
			}

			////------Menu Input Handling------////

			////------A Button Press
			if (scenecheck && (PressedA && scBBIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUISelect();
			}

			////------B Button Press
			if (scenecheck && PressedB && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUIExit(0);
				return;
			}

			////------Y Button Press
			if (scenecheck && (PressedY && scBBIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				CHudUIAlt();
			}

			////------X Button Press
			if (scenecheck && (PressedX && scBBIcon->m_MotionFrame >= 3) && !IsInMenuExit && !IsInMenuChange)
			{
				CHedUIPreview();
			}

			////------Shoulder Button Press
			if (scenecheck && (PressedRB || PressedLB) && !IsInMenuExit)
			{
				CHudUISwitch(0);
			}

			////------B Button Exit
			if (scenecheck && IsInMenuExit && scBBGui->m_MotionFrame <= 0 && !IsInMenuChange)
			{
				CHudUIExit(1);
			}

			////------Shoulder Button Change
			if (scenecheck && IsInMenuChange && scBBGui->m_MotionFrame <= 0 && !IsInMenuExit)
			{
				CHudUISwitch(1);
			}

			////------Cursor Movement Handle
			if (scenecheck && !IsInMenuExit && !IsInMenuChange && scBBIcon == !nullptr)
			{
				if ((PressedUp && scBBIcon->m_MotionFrame >= 3) || (PushedLSUp && scBBIcon->m_MotionFrame >= 12))
				{
					CHudUIMove(0);
				}

				if ((PressedDown && scBBIcon->m_MotionFrame >= 3) || (PushedLSDown && scBBIcon->m_MotionFrame >= 12))
				{
					CHudUIMove(1);
				}

				if ((PressedLeft && scBBIcon->m_MotionFrame >= 3) || (PushedLSLeft && scBBIcon->m_MotionFrame >= 12))
				{
					CHudUIMove(2);
				}

				if ((PressedRight && scBBIcon->m_MotionFrame >= 3) || (PushedLSRight && scBBIcon->m_MotionFrame >= 12))
				{
					CHudUIMove(3);
				}
			}

			break;
		}
	}

	////------Iterators and Timers------////

	if (!IsInMenu && MemoryOpenTimer >= 1)
		MemoryOpenTimer--;

	if (IsInMenu && PrevOpenTimer >= 1)
		PrevOpenTimer--;

	if (IsInMenu && !IsInMenuExit)
	{
		if (SWAOpenTimer >= 0)
			SWAOpenTimer--;
	}
	else
	{
		SWAOpenTimer = 15;
	}


	////------Debug UI------////

	//auto DebugUIX = 0.85;
	//if (PressedY && !DebugOpen)
	//{
	//	if (!obDebugTxtUI) //Create UI project if it doesn't exist
	//		CreateDebugTxtUI(This, Edx, in_rUpdateInfo);
	//	DebugSelect = 0;
	//	DebugScrollCount = 0;
	//	DebugOpen = true;
	//	scDebugTxtVisSel = prDebugTxtScreen->CreateScene("deco_text");
	//	scDebugTxtVisSel->SetMotion("Intro_Anim");
	//	scDebugTxtVisSel->SetPosition(0.7 * arX, 0.05 * arY);
	//	scDebugTxtVisSel->SetScale(0.7, 0.7);
	//	scDebugTxtVisSel->SetMotionFrame(0.0f);
	//	scDebugTxtVisSel->m_MotionDisableFlag = false;
	//	scDebugTxtVisSel->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	//	scDebugTxtVisSel->m_MotionSpeed = 1.0f;
	//	scDebugTxtVisSel->Update();
	//	SetDebugDispText(0, scDebugTxtVisSel, 0);
	//	scDebugTxtScroll = prDebugTxtScreen->CreateScene("deco_text");
	//	scDebugTxtScroll->SetMotion("Intro_Anim");
	//	scDebugTxtScroll->SetPosition(0.7 * arX, 0.10 * arY);
	//	scDebugTxtScroll->SetScale(0.7, 0.7);
	//	scDebugTxtScroll->SetMotionFrame(0.0f);
	//	scDebugTxtScroll->m_MotionDisableFlag = false;
	//	scDebugTxtScroll->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	//	scDebugTxtScroll->m_MotionSpeed = 1.0f;
	//	scDebugTxtScroll->Update();
	//	SetDebugDispText(0, scDebugTxtScroll, 1);
	//	scDebugTxtTrueSel = prDebugTxtScreen->CreateScene("deco_text");
	//	scDebugTxtTrueSel->SetMotion("Intro_Anim");
	//	scDebugTxtTrueSel->SetPosition(0.7 * arX, 0.15 * arY);
	//	scDebugTxtTrueSel->SetScale(0.7, 0.7);
	//	scDebugTxtTrueSel->SetMotionFrame(0.0f);
	//	scDebugTxtTrueSel->m_MotionDisableFlag = false;
	//	scDebugTxtTrueSel->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	//	scDebugTxtTrueSel->m_MotionSpeed = 1.0f;
	//	scDebugTxtTrueSel->Update();
	//	SetDebugDispText(0, scDebugTxtTrueSel, 2);
	//}
	//
	//if (DebugOpen)
	//{
	//	//DebugOption01 = DebugScrollCount - 0;
	//	//DebugOption02 = DebugScrollCount + 1;
	//	//DebugOption03 = DebugScrollCount + 2;
	//	//DebugOption04 = DebugScrollCount + 3;
	//	//DebugOption05 = DebugScrollCount + 4;
	//	//DebugOptionSelect = DebugSelect - DebugScrollCount;
	//	SetDebugDispText(CHudVarVisSel, scDebugTxtVisSel, 0);
	//	SetDebugDispText(CHudVarScroll, scDebugTxtScroll, 1);
	//	SetDebugDispText(CHudVarTrueSel, scDebugTxtTrueSel, 2);
	//	//SetDebugOptions(DebugOption01, scDebugOpt01);
	//	//SetDebugOptions(DebugOption02, scDebugOpt02);
	//	//SetDebugOptions(DebugOption03, scDebugOpt03);
	//	//SetDebugOptions(DebugOption04, scDebugOpt04);
	//	//SetDebugOptions(DebugOption05, scDebugOpt05);
	//	//SetDebug01Text("1");
	//	//switch (DebugOptionSelect)
	//	//{
	//	//case -2:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.025 * arY);
	//	//	break;
	//	//case -1:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.05 * arY);
	//	//	break;
	//	//case 0:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.1 * arY);
	//	//	break;
	//	//case 1:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.15 * arY);
	//	//	break;
	//	//case 2:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.2 * arY);
	//	//	break;
	//	//case 3:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.25 * arY);
	//	//	break;
	//	//case 4:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.3 * arY);
	//	//	break;
	//	//case 5:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.35 * arY);
	//	//	break;
	//	//case 6:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.4 * arY);
	//	//	break;
	//	//default:
	//	//	scDebugSelector->SetPosition(DebugUIX * arX, 0.05 * arY);
	//	//	break;
	//	//}
	//	//scDebugSelector->SetPosition(0.75 * arX, 0.1 * arY);
	//
	//	//if (PressedY && DebugOpen)
	//	//{
	//	//	//DebugSelect = DebugListStart;
	//	//	//DebugScrollCount = DebugListStart;
	//	//	
	//	//}
	//}

}

//Create Fitting Menu
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

//Kill Fitting Menu
HOOK(void, __fastcall, CHUDPauseUpdate, 0x0042A520, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	IsInMenuExit = false;
	IsInMenu = false;
	CHudTabSel = UIPartShoes;
	CHudVarVisSel = 0;
	MemoryOpenTimer = 0;
	CHudUISceneDestroy();
	originalCHUDPauseUpdate(This);
}
void __fastcall CHudSonicStageRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreen();
	IsInMenuExit = false;
	IsInMenu = false;
	CHudTabSel = UIPartShoes;
	CHudVarVisSel = 0;
	MemoryOpenTimer = 0;
	CHudUISceneDestroy();
}
HOOK(void, __fastcall, CHudResultStart, 0x010B6840, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	IsInMenuExit = false;
	IsInMenu = false;
	CHudTabSel = UIPartShoes;
	CHudVarVisSel = 0;
	MemoryOpenTimer = 0;
	CHudUISceneDestroy();
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

//Look At
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

//Super Sonic
HOOK(int, __fastcall, CSonicStatePluginSuperSonicStart, 0x11D6840, uint32_t This)
{
	int result = originalCSonicStatePluginSuperSonicStart(This);
	printf("SUPER SONIC START\n");

	if(obj_CustomizeSonicPlayerRenderable)
		obj_CustomizeSonicPlayerRenderable->SetSuper(true);
	RefreshCustomizeSonic(SelectCategory::All);
	
	return result;
}
HOOK(int, __fastcall, CSonicStatePluginSuperSonicEnd, 0x11D6720, uint32_t This)
{
	int result = originalCSonicStatePluginSuperSonicEnd(This);
	printf("SUPER SONIC END\n");

	if (obj_CustomizeSonicPlayerRenderable)
		obj_CustomizeSonicPlayerRenderable->SetSuper(false);
	RefreshCustomizeSonic(SelectCategory::All);

	return result;
}

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationUIParams, 0x00D65180, Sonic::CParameterFile* This)
{
	auto parameterGroup = This->CreateParameterGroup("Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce = parameterGroup->CreateParameterCategory("Customizable Sonic", "Parameters for customizable Sonic");

	cat_Bounce->CreateParamBool(&ConfigDecoEnable, "Show the fitting room UI decorations.");
	cat_Bounce->CreateParamTypeList((uint32_t*)&ActivateButton, "Activation Button", "Choose which button to press to open the Fitting Room",
		{
			{ "Select", 0},
			{ "Left Stick", 1},
			{ "Right Stick", 2},
			{ "Triggers", 3},
			{ "Shoulders", 4},
			{ "Dup", 5},
			{ "Ddown", 6},
			{ "Dleft", 7},
			{ "Dright", 8},
		});

	parameterGroup->Flush();

	originalInitializeApplicationUIParams(This);
}

//Install UI
void InstallCustomUI::applyPatches(ModInfo_t* modInfo)
{
	saveFilePath = modInfo->CurrentMod->Path;
	saveFilePath.erase(saveFilePath.find_last_of("\\/") + 1);
	saveFilePath += "CustomizeSave.ini";
	ReadINI(saveFilePath);

	INSTALL_HOOK(CHudSonicStageUpdate);
	INSTALL_HOOK(CHudPlayableMenuUpdate);
	INSTALL_HOOK(CHUDPauseUpdate);
	INSTALL_HOOK(CHudResultStart);
	WRITE_MEMORY(0x16A467C, void*, CHudSonicStageRemoveCallback);
	WRITE_MEMORY(0x16A5598, void*, CHudSonicStageRemoveCallback);
	INSTALL_HOOK(UpdateDirectorSimple);
	INSTALL_HOOK(UpdateDirectorNormal);
	INSTALL_HOOK(CPlayerAddCallback);
	INSTALL_HOOK(MsgLookAtStart);
	INSTALL_HOOK(MsgLookAtEnd);
	INSTALL_HOOK(CSonicStatePluginSuperSonicStart);
	INSTALL_HOOK(CSonicStatePluginSuperSonicEnd);
	INSTALL_HOOK(InitializeApplicationUIParams);
	//if (ActivateButton >= 5 && ActivateButton <= 8)
		WRITE_JUMP(0xD97B56, (void*)0xD97B9E); // Ignore D-pad input for Sonic's control
}