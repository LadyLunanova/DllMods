#include "SonicPlayer.h"
#include "NPCAnim.h"

//////Globals//////
SharedPtrTypeless WildfireVfxHandle;
SharedPtrTypeless JumpballVfxHandle;
bool WildFireActive = false;
void MsgWildFire(int Enabled)
{
	if (Enabled == 1)
		WildFireActive = true;
	else
		WildFireActive = false;
}
bool ParticleLightActive = false;
void ParticleLightGet(int Enabled)
{
	if (Enabled == 1)
		ParticleLightActive = true;
	else
		ParticleLightActive = false;
}
bool JumpBlueTrailActive = false;
void JumpBlueTrailGet(int Enabled)
{
	if (Enabled == 1)
		JumpBlueTrailActive = true;
	else
		JumpBlueTrailActive = false;
}
enum PlayerSelectJumpBallType
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
PlayerSelectJumpBallType PlayerSelectJumpBall = PlayerSelectJumpBallType::JumpBallDefault;
void MsgJumpBall(int BallType)
{
	PlayerSelectJumpBall = (PlayerSelectJumpBallType)BallType;
}
void MsgModelHide(bool Enabled);
void CreateCustomizeSonicRenderable();


//////Renderables//////
bool isSA1FlickerHide = false;
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

//////Jumpballs
class JumpballLWAnimRenderable : public Sonic::CGameObject3D
{
public:
	boost::shared_ptr<hh::mr::CSingleElement> m_spElement;
	boost::shared_ptr<Sonic::CMatrixNodeTransform> m_spChildNode;
	boost::shared_ptr<Sonic::CNPCAnimation> m_spNPCAnimation;
	bool hasChangedState = false;

	////Animation List
	static inline hh::anim::SMotionInfo m_sAnimList[2];

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder,
		Sonic::CGameDocument* in_pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
		{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		in_pGameDocument->AddUpdateUnit("a", this);

		////Setup Model
		hh::mr::CMirageDatabaseWrapper wrapper(in_spDatabase.get());
		boost::shared_ptr<hh::mr::CModelData> spModelData = wrapper.GetModelData("chr_sonic_spin", 0);

		////Spawn Model
		m_spElement = boost::make_shared<hh::mr::CSingleElement>(spModelData);
		if (!spModelData)
			return;
		AddRenderable("Object", m_spElement, true);

		////Attach renderable to Sonic with offset
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0]; 
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		m_spChildNode = boost::make_shared<Sonic::CMatrixNodeTransform>();
		const float scale = 1.5f;
		const float offset = 0.075f;
		m_spChildNode->m_Transform.SetPosition(hh::math::CVector(0, offset, 0));
		m_spChildNode->m_Transform.m_Matrix *= Eigen::Scaling(scale);
		m_spChildNode->NotifyChanged();
		m_spChildNode->SetParent(m_spMatrixNodeTransform.get());
		m_spMatrixNodeTransform->SetParent(context->m_spMatrixNode.get());
		m_spMatrixNodeTransform->NotifyChanged();
		m_spElement->BindMatrixNode(m_spChildNode);

		////Construct animator
		auto npcAnimation = reinterpret_cast<Sonic::CNPCAnimation*>(__HH_ALLOC(0x30));
		fCNPCAnimationCtor(npcAnimation);
		m_spNPCAnimation = boost::shared_ptr<Sonic::CNPCAnimation>(npcAnimation);

		////Setup anim list
		m_sAnimList[0].Name = "START";
		m_sAnimList[0].FileName = "spin_jp_start";
		m_sAnimList[0].RepeatType = 1;
		m_sAnimList[1].Name = "LOOP";
		m_sAnimList[1].FileName = "spin_nomal_loop";
		//m_sAnimList[1].FileName = "spin_dash_charge_loop";
		m_sAnimList[1].RepeatType = 0;

		//////Initialize Skeleton
		m_spNPCAnimation->Initialize(in_spDatabase, "chr_sonic_spin");
		m_spNPCAnimation->NPC_ADD_ANIM_LIST(m_sAnimList);
		m_spElement->BindPose(m_spNPCAnimation->m_spAnimationPose);
		m_spNPCAnimation->m_spAnimationPose->Update(0.0f);

		//////Animation transition
		auto* state = m_spNPCAnimation->m_spAnimationStateMachine->GetAnimationState("START").get();
		state->m_TransitionState = "LOOP";
		state->m_Field90 = true;
		state->m_Field8C = -1.0f;
		fCAnimationStateMachineSetBlend(m_spNPCAnimation->m_spAnimationStateMachine.get(), "LOOP", "START", 0.1f);

		//////Start Animation
		m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("START");

		}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& in_rUpdateInfo) override
	{
		if (m_spNPCAnimation->m_spAnimationStateMachine->m_Time >= 0.35 && !hasChangedState)
		{
			hasChangedState = true;
			m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("LOOP");
		}
		m_spNPCAnimation->m_spAnimationPose->Update(in_rUpdateInfo.DeltaTime);
		m_spNPCAnimation->m_spAnimationStateMachine->UpdateStateMachine(in_rUpdateInfo);
		
		//printf("%f\n", m_spNPCAnimation->m_spAnimationStateMachine->m_Time);
	}
};
boost::shared_ptr<JumpballLWAnimRenderable> obj_SonicJumpBallLWRenderable;
class JumpballSA1AnimRenderable : public Sonic::CGameObject3D
{
public:
	boost::shared_ptr<hh::mr::CSingleElement> m_spElement;
	boost::shared_ptr<Sonic::CMatrixNodeTransform> m_spChildNode;
	boost::shared_ptr<Sonic::CNPCAnimation> m_spNPCAnimation;
	SharedPtrTypeless SA2ballVfxHandle;
	bool hasChangedState = false;

	bool isVisible = true;
	bool typeFlicker = false;
	float flickerTimer = 0.0f;
	const float flickerTimerMax = (1.0f / 60.0f) * 16.0f;

	////Animation List
	static inline hh::anim::SMotionInfo m_sAnimList[1];

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder,
		Sonic::CGameDocument* in_pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		in_pGameDocument->AddUpdateUnit("a", this);

		////Setup Model
		hh::mr::CMirageDatabaseWrapper wrapper(in_spDatabase.get());
		boost::shared_ptr<hh::mr::CModelData> spModelData = wrapper.GetModelData("chr_sonic_spin_SA1", 0);

		////Spawn Model
		m_spElement = boost::make_shared<hh::mr::CSingleElement>(spModelData);
		if (!spModelData)
			return;
		AddRenderable("Object", m_spElement, true);

		////Attach renderable to Sonic with offset
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayer* cpcontext = static_cast<Sonic::Player::CPlayer*>(m_pMessageManager->GetMessageActor(playerID));
		m_spChildNode = boost::make_shared<Sonic::CMatrixNodeTransform>();
		const float scale = 1.07f;
		const float offset = 0.4f;
		m_spChildNode->m_Transform.SetPosition(hh::math::CVector(0, offset, 0));
		m_spChildNode->m_Transform.m_Matrix *= Eigen::Scaling(scale);
		m_spChildNode->NotifyChanged();
		m_spChildNode->SetParent(m_spMatrixNodeTransform.get());
		m_spMatrixNodeTransform->SetParent(context->m_spMatrixNode.get());
		m_spMatrixNodeTransform->NotifyChanged();
		m_spElement->BindMatrixNode(m_spChildNode);

		////Construct animator
		auto npcAnimation = reinterpret_cast<Sonic::CNPCAnimation*>(__HH_ALLOC(0x30));
		fCNPCAnimationCtor(npcAnimation);
		m_spNPCAnimation = boost::shared_ptr<Sonic::CNPCAnimation>(npcAnimation);

		////Setup anim list
		m_sAnimList[0].Name = "LOOP";
		m_sAnimList[0].FileName = "sns_spin_sa1_loop";
		m_sAnimList[0].RepeatType = 0;
		m_sAnimList[0].Speed = 1.4f;

		//////Initialize Skeleton
		m_spNPCAnimation->Initialize(in_spDatabase, "chr_Sonic_spin_SA1");
		m_spNPCAnimation->NPC_ADD_ANIM_LIST(m_sAnimList);
		m_spElement->BindPose(m_spNPCAnimation->m_spAnimationPose);
		m_spNPCAnimation->m_spAnimationPose->Update(0.0f);

		//////Start Animation
		m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("LOOP");

		////Spawn VFX
		//auto BallNode = m_spElement->GetNode("mesh_ball"); //Set up bone matrix for VFX
		//Common::fCGlitterCreate(cpcontext->m_spContext.get(), SA2ballVfxHandle, &BallNode, "ef_ch_sng_yh1_sa2spinattack", 1);  //Create VFX
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& in_rUpdateInfo) override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));

		if (m_spNPCAnimation->m_spAnimationStateMachine->m_Time >= 0.35 && !hasChangedState)
		{
			hasChangedState = true;
			m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("LOOP");
		}
		m_spNPCAnimation->m_spAnimationPose->Update(in_rUpdateInfo.DeltaTime);
		m_spNPCAnimation->m_spAnimationStateMachine->UpdateStateMachine(in_rUpdateInfo);

		if (m_spElement->m_Enabled && isVisible && !typeFlicker)
		{
			pPlayer->m_spCharacterModel->m_Enabled = false;
			//isSA1FlickerHide = true;
			isVisible = false;
		}

		flickerTimer += in_rUpdateInfo.DeltaTime;
		if (flickerTimer >= flickerTimerMax)
		{
			flickerTimer = 0.0f;
			typeFlicker = !typeFlicker;
		}

		if (typeFlicker)
		{
			//isSA1FlickerHide = !isVisible;
			pPlayer->m_spCharacterModel->m_Enabled = isVisible;
			isVisible = !isVisible;
			m_spElement->m_Enabled = isVisible;
		}
		else if (!m_spElement->m_Enabled)
		{
			//isSA1FlickerHide = true;
			pPlayer->m_spCharacterModel->m_Enabled = false;
			isVisible = false;
			m_spElement->m_Enabled = true;
		}

		//printf("%f\n", m_spNPCAnimation->m_spAnimationStateMachine->m_Time);
	}

	void KillCallback() override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
		const Sonic::Player::CPlayer* cpcontext = static_cast<Sonic::Player::CPlayer*>(m_pMessageManager->GetMessageActor(playerID));
		pPlayer->m_spCharacterModel->m_Enabled = true;
		isSA1FlickerHide = false;
		//Common::fCGlitterEnd(cpcontext->m_spContext.get(), SA2ballVfxHandle, true); //Destroy Ball VFX
		//printf("KILL CALLBACK\n");
	}
};
boost::shared_ptr<JumpballSA1AnimRenderable> obj_SonicJumpBallSA1Renderable;
class JumpballWarsAnimRenderable : public Sonic::CGameObject3D
{
public:
	boost::shared_ptr<hh::mr::CSingleElement> m_spElement;
	boost::shared_ptr<Sonic::CMatrixNodeTransform> m_spChildNode;
	boost::shared_ptr<Sonic::CNPCAnimation> m_spNPCAnimation;
	SharedPtrTypeless WarsballVfxHandle;
	bool hasChangedState = false;

	////Animation List
	static inline hh::anim::SMotionInfo m_sAnimList[2];

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder,
		Sonic::CGameDocument* in_pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		in_pGameDocument->AddUpdateUnit("a", this);

		////Setup Model
		hh::mr::CMirageDatabaseWrapper wrapper(in_spDatabase.get());
		boost::shared_ptr<hh::mr::CModelData> spModelData = wrapper.GetModelData("chr_sonicspin_wars", 0);

		////Spawn Model
		m_spElement = boost::make_shared<hh::mr::CSingleElement>(spModelData);
		if (!spModelData)
			return;
		AddRenderable("Object", m_spElement, true);

		////Attach renderable to Sonic with offset
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* cpscontext = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayer* cpcontext = static_cast<Sonic::Player::CPlayer*>(m_pMessageManager->GetMessageActor(playerID));
		m_spChildNode = boost::make_shared<Sonic::CMatrixNodeTransform>();
		const float scale = 1.0f;
		const float offset = 0.0f;
		m_spChildNode->m_Transform.SetPosition(hh::math::CVector(0, offset, 0));
		m_spChildNode->m_Transform.m_Matrix *= Eigen::Scaling(scale);
		m_spChildNode->NotifyChanged();
		m_spChildNode->SetParent(m_spMatrixNodeTransform.get());
		m_spMatrixNodeTransform->SetParent(cpscontext->m_spMatrixNode.get());
		m_spMatrixNodeTransform->NotifyChanged();
		m_spElement->BindMatrixNode(m_spChildNode);

		////Construct animator
		auto npcAnimation = reinterpret_cast<Sonic::CNPCAnimation*>(__HH_ALLOC(0x30));
		fCNPCAnimationCtor(npcAnimation);
		m_spNPCAnimation = boost::shared_ptr<Sonic::CNPCAnimation>(npcAnimation);

		////Setup anim list
		m_sAnimList[0].Name = "START";
		m_sAnimList[0].FileName = "wars_spin_jump";
		m_sAnimList[0].RepeatType = 1;
		m_sAnimList[1].Name = "LOOP";
		m_sAnimList[1].FileName = "wars_spin_nomal_loop";
		m_sAnimList[1].RepeatType = 0;

		//////Initialize Skeleton
		m_spNPCAnimation->Initialize(in_spDatabase, "chr_sonicspin_wars");
		m_spNPCAnimation->NPC_ADD_ANIM_LIST(m_sAnimList);
		m_spElement->BindPose(m_spNPCAnimation->m_spAnimationPose);
		m_spNPCAnimation->m_spAnimationPose->Update(0.0f);

		//////Animation transition
		auto* state = m_spNPCAnimation->m_spAnimationStateMachine->GetAnimationState("START").get();
		state->m_TransitionState = "LOOP";
		state->m_Field90 = true;
		state->m_Field8C = -1.0f;
		fCAnimationStateMachineSetBlend(m_spNPCAnimation->m_spAnimationStateMachine.get(), "LOOP", "START", 0.1f);

		//////Start Animation
		m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("START");

		auto BallNode = m_spElement->GetNode("Mesh"); //Set up bone matrix for VFX
		Common::fCGlitterCreate(cpcontext->m_spContext.get(), WarsballVfxHandle, &BallNode, "ef_ch_sng_yh1_forcesspinattack", 1);  //Create VFX
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& in_rUpdateInfo) override
	{
		//auto BallNode = m_spElement->GetNode("Mesh"); //Set up bone matrix for VFX
		//BallNode->NotifyChanged();
		if (m_spNPCAnimation->m_spAnimationStateMachine->m_Time >= 0.35 && !hasChangedState)
		{
			hasChangedState = true;
			m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("LOOP");
		}
		m_spNPCAnimation->m_spAnimationPose->Update(in_rUpdateInfo.DeltaTime);
		m_spNPCAnimation->m_spAnimationStateMachine->UpdateStateMachine(in_rUpdateInfo);
	}

	void KillCallback() override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayer* cpcontext = static_cast<Sonic::Player::CPlayer*>(m_pMessageManager->GetMessageActor(playerID));

		//printf("KILL CALLBACK\n");
		Common::fCGlitterEnd(cpcontext->m_spContext.get(), WarsballVfxHandle, true); //Destroy Ball VFX
	}
};
boost::shared_ptr<JumpballWarsAnimRenderable> obj_SonicJumpBallWarsRenderable;

//////Dynamic Omnis//////
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

	//if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
	//{
	//	auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

	//	const auto& up = playerContext->m_ModelUpDirection;
	//	const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

	//	if (stateName == "Sliding" || playerContext->StateFlag(eStateFlag_Boost))
	//		position += fwd * 0.5f + up * 0.25f;

	//	if (!superPLight)
	//		superPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
	//	else
	//		superPLight->SetPosition(position);
	//}
	//else
	//	superPLight = nullptr;
}
void SuperParticleLight(Sonic::Player::CPlayer* player)
{
	auto gameDocument = Sonic::CGameDocument::GetInstance();
	if (!gameDocument.get().get() || !gameDocument->m_pMember->m_spLightManager)
		return;

	auto playerContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!playerContext)
		return;

	if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
	{
		auto positionSS = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& upSS = playerContext->m_ModelUpDirection;
		const auto fwdSS = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		positionSS += fwdSS * 0.064f + upSS * 0.7f;

		if (!superPLight)
			superPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(positionSS, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 7.0f });
		else
			superPLight->SetPosition(positionSS);
	}
	else
		superPLight = nullptr;
}
inline void SonicContextSpawnLocusEffect(Sonic::Player::CPlayerSpeedContext* pSonicContext)
{
	// 1 seems to not stop? Force it to be 0
	WRITE_MEMORY(0xE178E5, uint32_t, 0);

	struct MsgRequestLocusEffect
	{
		INSERT_PADDING(0x10);
		uint32_t flag;
	};
	MsgRequestLocusEffect message{};
	message.flag = 0;

	FUNCTION_PTR(int, __thiscall, processMsgRequestLocusEffect, 0xE178D0, void* This, void* pMessage); //0x00E178D0
	//void* player = *(void**)((uint32_t)pSonicContext + 0x110);
	processMsgRequestLocusEffect(pSonicContext->m_pPlayer, &message);
}

void SpawnFireParticle(Sonic::Player::CPlayer* player)
{
	if (player == nullptr)
		return;
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

HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedLeft = input.IsTapped(Sonic::eKeyState_DpadLeft);
	bool PressedRight = input.IsTapped(Sonic::eKeyState_DpadRight);
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);

	if (IsModernSonic)
	{
		//if (!isRenderableCreated)
		//{
		//	obj_CustomizeSonicRenderable = boost::make_shared<CustomizeSonicRenderable>();
		//	Sonic::CGameDocument::GetInstance()->AddGameObject(obj_CustomizeSonicRenderable);
		//	isRenderableCreated = true;
		//}

		CreateCustomizeSonicRenderable();

		if (WildFireActive == true)
			SpawnFireParticle(This);
		if (WildFireActive == false)
			KillFireParticle(This);

		if (PlayerSelectJumpBall == (enum PlayerSelectJumpBallType)JumpBallNoBall)
			sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_JumpShortReleaseTime] = 256.0f;
		else if ((PlayerSelectJumpBall == (enum PlayerSelectJumpBallType)JumpBallLW)
			|| (PlayerSelectJumpBall == (enum PlayerSelectJumpBallType)JumpBallForces)
			|| (PlayerSelectJumpBall == (enum PlayerSelectJumpBallType)JumpBallSA1))
		{
			sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_JumpShortReleaseTime] = 0.0f;
			sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_VertVelocityBallToFall] = -1000.0f;
		}
		else
		{
			sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_JumpShortReleaseTime);
			sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_VertVelocityBallToFall);
		}
	}

	//printf(sonic->GetCurrentAnimationName().c_str());
	//printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	//printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);

	if (ParticleLightActive == true)
	{
		CustomParticleLight(0.0, 0.4, 2.0, 1.0, 4.2, This);
		SuperParticleLight(This);
	}
		
}
HOOK(void, __fastcall, CSonicStateJumpBallStartState, 0x011BCBE0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalCSonicStateJumpBallStartState(This);
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	bool IsSuper = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30); //Set up center matrix for VFX

	if (JumpBlueTrailActive == true)
		SonicContextSpawnLocusEffect(sonic); //Spawn blue trail

	if (!IsSuper)
	{
		switch (PlayerSelectJumpBall)
		{
		case (enum PlayerSelectJumpBallType)JumpBallNoVFX:
		case (enum PlayerSelectJumpBallType)JumpBallNoBall:
			return;
			break;
		case (enum PlayerSelectJumpBallType)JumpBallBAP:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_bounceattack", 1);  //Create Ball VFX
			break;
		case (enum PlayerSelectJumpBallType)JumpBallForces:
			obj_SonicJumpBallWarsRenderable = boost::make_shared<JumpballWarsAnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallWarsRenderable);
			MsgModelHide(true);
			//Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_forcesspinattack", 1);  //Create Ball VFX
			break;
		case (enum PlayerSelectJumpBallType)JumpBallBetaSWA:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_swaspinattack", 1);  //Create Ball VFX
			break;
		case (enum PlayerSelectJumpBallType)JumpBallSWA:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_swaretailspinattack", 1);  //Create Ball VFX
			break;
		case (enum PlayerSelectJumpBallType)JumpBallSA1:
			obj_SonicJumpBallSA1Renderable = boost::make_shared<JumpballSA1AnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallSA1Renderable);
			//MsgModelHide(true);
			break;
		case (enum PlayerSelectJumpBallType)JumpBallSA2:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_sa2spinattack", 1);  //Create Ball VFX
			break;
		case (enum PlayerSelectJumpBallType)JumpBallLW:
			obj_SonicJumpBallLWRenderable = boost::make_shared<JumpballLWAnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallLWRenderable);
			MsgModelHide(true);
			break;
		default:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_spinattack", 1);  //Create Ball VFX
			break;
		}
	}
	else
	{
		switch (PlayerSelectJumpBall)
		{
		case (enum PlayerSelectJumpBallType)JumpBallNoVFX:
		case (enum PlayerSelectJumpBallType)JumpBallNoBall:
			return;
			break;
		case (enum PlayerSelectJumpBallType)JumpBallBAP:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_ssn_yh1_bounceattack", 1);
			break;
		case (enum PlayerSelectJumpBallType)JumpBallForces:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_ssn_yh1_forcesspinattack", 1);
			break;
		case (enum PlayerSelectJumpBallType)JumpBallBetaSWA:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_ssn_yh1_swaspinattack", 1);
			break;
		case (enum PlayerSelectJumpBallType)JumpBallSWA:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_ssn_yh1_swaretailspinattack", 1);
			break;
		case (enum PlayerSelectJumpBallType)JumpBallSA1:
			obj_SonicJumpBallSA1Renderable = boost::make_shared<JumpballSA1AnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallSA1Renderable);
			break;
		case (enum PlayerSelectJumpBallType)JumpBallLW:
			obj_SonicJumpBallLWRenderable = boost::make_shared<JumpballLWAnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallLWRenderable);
			break;
		default:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sps_yh1_spinattack", 1);
			break;
		}
	}

}
HOOK(void, __fastcall, CSonicStateJumpBallEndState, 0x011BCB60, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalCSonicStateJumpBallEndState(This);
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	MsgModelHide(false);
	if (obj_SonicJumpBallLWRenderable)
	{
		obj_SonicJumpBallLWRenderable->SendMessageImm<Sonic::Message::MsgKill>(obj_SonicJumpBallLWRenderable->m_ActorID);
	}
	if (obj_SonicJumpBallSA1Renderable)
	{
		obj_SonicJumpBallSA1Renderable->SendMessageImm<Sonic::Message::MsgKill>(obj_SonicJumpBallSA1Renderable->m_ActorID);
	}
	if (obj_SonicJumpBallWarsRenderable)
	{
		obj_SonicJumpBallWarsRenderable->SendMessageImm<Sonic::Message::MsgKill>(obj_SonicJumpBallWarsRenderable->m_ActorID);
	}

	Common::fCGlitterEnd(sonic, JumpballVfxHandle, true); //Destroy Ball VFX
	auto SparkEffectManager = sonic->m_pSparkEffectManager;
	FUNCTION_PTR(void*, __stdcall, StopLocusEffect, 0x00E8C940, void* a1, const hh::base::CSharedString & sharedString);
	if (SparkEffectManager)
	{
		StopLocusEffect(SparkEffectManager, "HomingAttack");
	}
}

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationVFXParams, 0x00D65180, Sonic::CParameterFile* This)
{
	auto parameterGroup = This->CreateParameterGroup("Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce = parameterGroup->CreateParameterCategory("Customizable Sonic", "Parameters for customizable Sonic");

	cat_Bounce->CreateParamBool(&JumpBlueTrailActive, "Create a blue trail when Sonic jumps.");

	parameterGroup->Flush();

	originalInitializeApplicationVFXParams(This);
}

//Install VFX
void InstallSonicPlayer::applyPatches()
{
	INSTALL_HOOK(MsgRestartStage);
	WRITE_MEMORY(0x16D4B4C, void*, CSonicRemoveCallback);
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(CSonicStateJumpBallStartState);
	INSTALL_HOOK(CSonicStateJumpBallEndState);
	INSTALL_HOOK(InitializeApplicationVFXParams);
}