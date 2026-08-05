#pragma once

// Original code by Brianuuu: https://github.com/brianuuu
using SharedPtrTypeless = boost::shared_ptr<void>;
enum SonicCollision : uint32_t
{
	TypeNoAttack = 0x1E61B5C,
	TypeRagdoll = 0x1E61B60,
	TypeSonicSpinCharge = 0x1E61B64,
	TypeSonicSpin = 0x1E61B68,
	TypeSonicUnbeaten = 0x1E61B6C,
	TypeSuperSonic = 0x1E61B70,
	TypeSonicSliding = 0x1E61B74,
	TypeSonicHoming = 0x1E61B78,
	TypeSonicSelectJump = 0x1E61B7C,
	TypeSonicDrift = 0x1E61B80,
	TypeSonicBoost = 0x1E61B84,
	TypeSonicStomping = 0x1E61B88,
	TypeSonicTrickAttack = 0x1E61B8C,
	TypeSonicSquatKick = 0x1E61B90,
	TypeSonicClassicSpin = 0x1E61B94,
	TypeExplosion = 0x1E61B98,
	TypeBossAttack = 0x1E61B9C,
	TypeGunTruckAttack = 0x1E61BA0,
	TypeRagdollEnemyAttack = 0x1E61BA4,
};
namespace Common
{
	static void* SonicContextSetCollision(SonicCollision collisionType, bool enabled, Sonic::Player::CPlayerSpeedContext* sonic)
	{
		static void* const pEnableFunc = (void*)0xE65610;
		static void* const pDisableFunc = (void*)0xE655C0;
		__asm
		{
			mov		edi, sonic

			mov		ecx, collisionType
			mov		ecx, [ecx]
			push	ecx

			cmp		enabled, 0
			je		jump

			call[pEnableFunc]
			jmp		end

			jump :
			call[pDisableFunc]

				end :
		}
	}
	static void* fCGlitterCreate
	(
		void* pContext,
		SharedPtrTypeless& handle,
		void* pMatrixTransformNode,
		Hedgehog::Base::CSharedString const& name,
		uint32_t flag
	)
	{
		static void* const pCGlitterCreate = (void*)0xE73890;
		__asm
		{
			push    flag
			push    name
			push    pMatrixTransformNode
			mov     eax, pContext
			mov     esi, handle
			call[pCGlitterCreate]
		}
	}
	static void fCGlitterEnd
	(
		void* pContext,
		SharedPtrTypeless& handle,
		bool instantStop
	)
	{
		static void* const pCGlitterEnd = (void*)0xE72650;
		static void* const pCGlitterKill = (void*)0xE72570;
		__asm
		{
			mov     eax, [handle]
			mov     ebx, [eax + 4]
			push    ebx
			test	ebx, ebx
			jz		noIncrement
			mov		edx, 1
			add		ebx, 4
			lock xadd[ebx], edx

			noIncrement :
			mov     ebx, [eax]
				push    ebx
				mov     eax, pContext
				cmp     instantStop, 0
				jnz     jump
				call[pCGlitterEnd]
				jmp     end

				jump :
			call[pCGlitterKill]

				end :
		}
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
}

//////Globals//////
SharedPtrTypeless BounceBallVfxHandle; //Ball VFX handler
SharedPtrTypeless BounceTrailVfxHandle; //Following balls VFX handler
SharedPtrTypeless BounceLandVfxHandle; // Bounce landing VFX handler
SharedPtrTypeless ClassicBounceBallVfxHandle; // Bounce landing VFX handler

bool isRenderableCreated = false;
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

int BounceCount = 0; //Bounce counter

//Modern
bool  ModernBounceEnable = true;
bool  ModernBounceTrigger = true;
int   ModernSoundType = 0;
bool  ModernBounceVO = false;
bool  ModernBlueTrail = true;
int   ModernBallType = 0;
bool  ModernLWBounce = false;
bool  ModernBounceHorCnl = false;
bool  ModernNoBounceEnemy = false;
int   ModernGroundActionType = 0;
bool  ModernBounceTricking = false;
float ModernBounce01 = 17.0f;
float ModernBounce02 = 18.5f;
float ModernBounce03 = 20.0f;
float ModernBounce04 = 22.0f;
float ModernBounce05 = 24.0f;
float ModernBounceDrop = 21.0f;
float ModernBounceMulti = 1.0f;

enum class SelectBounceBallType : uint32_t
{
	BAP,
	Original,
	SWA,
	BetaSWA,
	SA1,
	SA2,
	LW,
	Forces,
	NoVFX,
};
SelectBounceBallType SelectBounceBall = SelectBounceBallType::BAP;

//Classic
bool  ClassicBounceEnable = true;
bool  ClassicBounceTrigger = true;
int   ClassicSoundType = 0;
bool  ClassicBlueTrail = false;
bool  ClassicLWBounce = false;
bool  ClassicBounceHorCnl = false;
bool  ClassicNoBounceEnemy = false;
bool  ClassicBounceTricking = false;
float ClassicBounce01 = 17.0f;
float ClassicBounce02 = 18.5f;
float ClassicBounce03 = 20.0f;
float ClassicBounce04 = 22.0f;
float ClassicBounce05 = 24.0f;
float ClassicBounceDrop = 21.0f;
float ClassicBounceMulti = 1.0f;

//////Renderables//////
class JumpballSA1AnimRenderable : public Sonic::CGameObject3D
{
public:
	boost::shared_ptr<hh::mr::CSingleElement> m_spElement;
	boost::shared_ptr<Sonic::CMatrixNodeTransform> m_spChildNode;
	boost::shared_ptr<Sonic::CNPCAnimation> m_spNPCAnimation;
	const Hedgehog::Base::CSharedString& sngSpinModel = "chr_sonic_spin_SA1";
	const Hedgehog::Base::CSharedString& spsSpinModel = "chr_supersonic_spin_SA1";
	const Hedgehog::Base::CSharedString& sngSpinEffect = "ef_ch_sng_yh1_sa2spinattack";
	const Hedgehog::Base::CSharedString& spsSpinEffect = "ef_ch_sps_yh1_sa2spinattack";
	SharedPtrTypeless SA2ballVfxHandle;
	bool hasChangedState = false;

	bool isVisible = true;
	bool typeFlicker = false;
	float flickerTimer = 0.0f;
	const float flickerTimerMax = (1.0f / 60.0f) * 16.0f;

	////Animation List
	static inline hh::anim::SMotionInfo m_sAnimList[1]
	{
		{ "LOOP", "sns_spin_sa1_loop", 1.4f, 0 }
	};

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder,
		Sonic::CGameDocument* in_pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		in_pGameDocument->AddUpdateUnit("a", this);

		auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		bool IsSuper = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
		//printf("%s\n", IsSuper ? "SUPER TRUE" : "SUPER FALSE");

		////Setup Model
		hh::mr::CMirageDatabaseWrapper wrapper(in_spDatabase.get());
		boost::shared_ptr<hh::mr::CModelData> spModelData = wrapper.GetModelData(IsSuper ? spsSpinModel : sngSpinModel, 0);

		////Spawn Model
		m_spElement = boost::make_shared<hh::mr::CSingleElement>(spModelData);
		if (!spModelData)
			return;
		AddRenderable("Object", m_spElement, true);

		////Attach renderable to Sonic with offset
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
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

		//////Initialize Skeleton
		m_spNPCAnimation->Initialize(in_spDatabase, "chr_Sonic_spin_SA1");
		m_spNPCAnimation->NPC_ADD_ANIM_LIST(m_sAnimList);
		m_spElement->BindPose(m_spNPCAnimation->m_spAnimationPose);
		m_spNPCAnimation->m_spAnimationPose->Update(0.0f);

		//////Start Animation
		m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("LOOP");

		////Spawn VFX
		if (SelectBounceBall == SelectBounceBallType::SA2)
		{
			auto BallNode = m_spElement->GetNode("mesh_ball"); //Set up bone matrix for VFX
			Common::fCGlitterCreate(cpcontext->m_spContext.get(), SA2ballVfxHandle, &BallNode, IsSuper ? spsSpinEffect : sngSpinEffect, 1);  //Create VFX
		}

		//////Hide Sonic
		pPlayer->m_spCharacterModel->m_Enabled = false;
		context->m_pPlayer->SendMessageImm(context->m_SuperRenderableActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(false));
	}

	void UpdateParallel(const Hedgehog::Universe::SUpdateInfo& in_rUpdateInfo) override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));

		m_spNPCAnimation->m_spAnimationPose->Update(in_rUpdateInfo.DeltaTime);
		m_spNPCAnimation->m_spAnimationStateMachine->UpdateStateMachine(in_rUpdateInfo);

		if (SelectBounceBall == SelectBounceBallType::SA1)
		{
			if (m_spElement->m_Enabled && isVisible && !typeFlicker)
			{
				pPlayer->m_spCharacterModel->m_Enabled = false;
				context->m_pPlayer->SendMessageImm(context->m_SuperRenderableActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(false));
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
				pPlayer->m_spCharacterModel->m_Enabled = isVisible;
				context->m_pPlayer->SendMessageImm(context->m_SuperRenderableActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(isVisible));
				isVisible = !isVisible;
				m_spElement->m_Enabled = isVisible;
			}
			else if (!m_spElement->m_Enabled)
			{
				pPlayer->m_spCharacterModel->m_Enabled = false;
				context->m_pPlayer->SendMessageImm(context->m_SuperRenderableActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(false));
				isVisible = false;
				m_spElement->m_Enabled = true;
			}
		}
	}

	void KillCallback() override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
		const Sonic::Player::CPlayer* cpcontext = static_cast<Sonic::Player::CPlayer*>(m_pMessageManager->GetMessageActor(playerID));
		pPlayer->m_spCharacterModel->m_Enabled = true;
		context->m_pPlayer->SendMessageImm(context->m_SuperRenderableActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(true));
		if (SelectBounceBall == SelectBounceBallType::SA2)
			Common::fCGlitterEnd(cpcontext->m_spContext.get(), SA2ballVfxHandle, true); //Destroy Ball VFX
	}
};
boost::shared_ptr<JumpballSA1AnimRenderable> obj_SonicJumpBallSA1Renderable;
class JumpballLWAnimRenderable : public Sonic::CGameObject3D
{
public:
	boost::shared_ptr<hh::mr::CSingleElement> m_spElement;
	boost::shared_ptr<Sonic::CMatrixNodeTransform> m_spChildNode;
	boost::shared_ptr<Sonic::CNPCAnimation> m_spNPCAnimation;
	const Hedgehog::Base::CSharedString& sngSpinModel = "chr_sonic_spin";
	const Hedgehog::Base::CSharedString& spsSpinModel = "chr_supersonic_spin";
	bool hasChangedState = false;

	////Animation List
	static inline hh::anim::SMotionInfo m_sAnimList[2]
	{
		{ "START", "spin_jp_start", 1.0f, 1 },
		{ "LOOP", "spin_nomal_loop", 1.0f, 0 }
	};

	void SetAnimStateTransition(const char* in_pStartState, const char* in_pEndState, float in_TransitionSpeed)
	{
		auto* state = m_spNPCAnimation->m_spAnimationStateMachine->GetAnimationState(in_pStartState).get();
		state->m_TransitionState = in_pEndState;
		state->m_Field90 = true;
		state->m_Field8C = -1.0f;
		fCAnimationStateMachineSetBlend(m_spNPCAnimation->m_spAnimationStateMachine.get(), in_pEndState, in_pStartState, in_TransitionSpeed);
	}

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder,
		Sonic::CGameDocument* in_pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		in_pGameDocument->AddUpdateUnit("a", this);

		auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		bool IsSuper = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
		//printf("%s\n", IsSuper ? "SUPER TRUE" : "SUPER FALSE");

		////Setup Model
		hh::mr::CMirageDatabaseWrapper wrapper(in_spDatabase.get());
		boost::shared_ptr<hh::mr::CModelData> spModelData = wrapper.GetModelData(IsSuper ? spsSpinModel : sngSpinModel, 0);

		////Spawn Model
		m_spElement = boost::make_shared<hh::mr::CSingleElement>(spModelData);
		if (!spModelData)
			return;
		AddRenderable("Object", m_spElement, true);

		////Attach renderable to Sonic with offset
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
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

		//////Initialize Skeleton
		m_spNPCAnimation->Initialize(in_spDatabase, "chr_sonic_spin");
		m_spNPCAnimation->NPC_ADD_ANIM_LIST(m_sAnimList);
		m_spElement->BindPose(m_spNPCAnimation->m_spAnimationPose);
		m_spNPCAnimation->m_spAnimationPose->Update(0.0f);

		//////Animation transition
		SetAnimStateTransition("START", "LOOP", 0.1f);

		//////Start Animation
		m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("START");

		//////Hide Sonic
		pPlayer->m_spCharacterModel->m_Enabled = false;
		context->m_pPlayer->SendMessageImm(context->m_SuperRenderableActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(false));
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
	}

	void KillCallback() override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
		pPlayer->m_spCharacterModel->m_Enabled = true;
		context->m_pPlayer->SendMessageImm(context->m_SuperRenderableActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(true));
	}
};
boost::shared_ptr<JumpballLWAnimRenderable> obj_SonicJumpBallLWRenderable;
class JumpballWarsAnimRenderable : public Sonic::CGameObject3D
{
public:
	boost::shared_ptr<hh::mr::CSingleElement> m_spElement;
	boost::shared_ptr<Sonic::CMatrixNodeTransform> m_spChildNode;
	boost::shared_ptr<Sonic::CNPCAnimation> m_spNPCAnimation;
	const Hedgehog::Base::CSharedString& sngSpinModel = "chr_sonicspin_wars";
	const Hedgehog::Base::CSharedString& spsSpinModel = "chr_supersonicspin_wars";
	const Hedgehog::Base::CSharedString& sngSpinEffect = "ef_ch_sng_yh1_forcesspinattack";
	const Hedgehog::Base::CSharedString& spsSpinEffect = "ef_ch_sps_yh1_forcesspinattack";
	SharedPtrTypeless WarsballVfxHandle;
	bool hasChangedState = false;

	////Animation List
	static inline hh::anim::SMotionInfo m_sAnimList[2]
	{
		{ "START", "wars_spin_jump", 1.0f, 1 },
		{ "LOOP", "wars_spin_nomal_loop", 1.0f, 0 }
	};

	void SetAnimStateTransition(const char* in_pStartState, const char* in_pEndState, float in_TransitionSpeed)
	{
		auto* state = m_spNPCAnimation->m_spAnimationStateMachine->GetAnimationState(in_pStartState).get();
		state->m_TransitionState = in_pEndState;
		state->m_Field90 = true;
		state->m_Field8C = -1.0f;
		fCAnimationStateMachineSetBlend(m_spNPCAnimation->m_spAnimationStateMachine.get(), in_pEndState, in_pStartState, in_TransitionSpeed);
	}

	void AddCallback(const Hedgehog::Base::THolder<Sonic::CWorld>& in_rWorldHolder,
		Sonic::CGameDocument* in_pGameDocument, const boost::shared_ptr<Hedgehog::Database::CDatabase>& in_spDatabase) override
	{
		Sonic::CApplicationDocument::GetInstance()->AddMessageActor("GameObject", this);
		in_pGameDocument->AddUpdateUnit("a", this);

		auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
		bool IsSuper = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
		//printf("%s\n", IsSuper ? "SUPER TRUE" : "SUPER FALSE");

		////Setup Model
		hh::mr::CMirageDatabaseWrapper wrapper(in_spDatabase.get());
		boost::shared_ptr<hh::mr::CModelData> spModelData = wrapper.GetModelData(IsSuper ? spsSpinModel : sngSpinModel, 0);

		////Spawn Model
		m_spElement = boost::make_shared<hh::mr::CSingleElement>(spModelData);
		if (!spModelData)
			return;
		AddRenderable("Object", m_spElement, true);

		////Attach renderable to Sonic with offset
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
		const Sonic::Player::CPlayer* cpcontext = static_cast<Sonic::Player::CPlayer*>(m_pMessageManager->GetMessageActor(playerID));
		m_spChildNode = boost::make_shared<Sonic::CMatrixNodeTransform>();
		const float scale = 1.0f;
		const float offset = 0.0f;
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

		//////Initialize Skeleton
		m_spNPCAnimation->Initialize(in_spDatabase, "chr_sonicspin_wars");
		m_spNPCAnimation->NPC_ADD_ANIM_LIST(m_sAnimList);
		m_spElement->BindPose(m_spNPCAnimation->m_spAnimationPose);
		m_spNPCAnimation->m_spAnimationPose->Update(0.0f);

		//////Animation transition
		SetAnimStateTransition("START", "LOOP", 0.1f);

		//////Start Animation
		m_spNPCAnimation->m_spAnimationStateMachine->ChangeState("START");

		auto BallNode = m_spElement->GetNode("Mesh"); //Set up bone matrix for VFX
		Common::fCGlitterCreate(cpcontext->m_spContext.get(), WarsballVfxHandle, &BallNode, IsSuper ? spsSpinEffect : sngSpinEffect, 1);

		//////Hide Sonic
		pPlayer->m_spCharacterModel->m_Enabled = false;
		context->m_pPlayer->SendMessageImm(context->m_SuperRenderableActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(false));
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
	}

	void KillCallback() override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeedContext* context = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID))->GetContext();
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
		const Sonic::Player::CPlayer* cpcontext = static_cast<Sonic::Player::CPlayer*>(m_pMessageManager->GetMessageActor(playerID));
		Common::fCGlitterEnd(cpcontext->m_spContext.get(), WarsballVfxHandle, true); //Destroy Ball VFX
		pPlayer->m_spCharacterModel->m_Enabled = true;
		context->m_pPlayer->SendMessageImm(context->m_SuperRenderableActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(true));
	}
};
boost::shared_ptr<JumpballWarsAnimRenderable> obj_SonicJumpBallWarsRenderable;

//INI file Handling
void WriteINI(FILE* iniFile)
{
	if (!iniFile)
		return;
	//printf("WRITING INI");
	printf("\n");
	char buffer[1024]{};
	snprintf(buffer, sizeof(buffer),
		"%s\n" //"[Modern]"
		"%s%s\n" //ModernBounceEnable
		"%s%s\n" //ModernBounceTrigger
		"%s%d\n" //ModernSoundType
		"%s%s\n" //ModernBounceVO
		"%s%s\n" //ModernBlueTrail
		"%s%d\n" //ModernBallType
		"%s%s\n" //ModernLWBounce
		"%s%s\n" //ModernBounceHorCnl
		"%s%s\n" //ModernNoBounceEnemy
		"%s%s\n" //ModernGroundActionType
		"%s%s\n" //ModernBounceTricking
		"%s%d\n" //ModernBounce01
		"%s%d\n" //ModernBounce02
		"%s%d\n" //ModernBounce03
		"%s%d\n" //ModernBounce04
		"%s%d\n" //ModernBounce05
		"%s%d\n" //ModernBounceDrop
		"%s%d\n" //ModernBounceMulti
		"\n"     //
		"%s\n"   //[Classic]
		"%s%s\n" //ClassicBounceEnable
		"%s%s\n" //ClassicBounceTrigger
		"%s%d\n" //ClassicSoundType
		"%s%s\n" //ClassicBlueTrail
		"%s%s\n" //ClassicLWBounce
		"%s%s\n" //ClassicBounceHorCnl
		"%s%s\n" //ClassicNoBounceEnemy
		"%s%s\n" //ClassicBounceTricking
		"%s%d\n" //ClassicBounce01
		"%s%d\n" //ClassicBounce02
		"%s%d\n" //ClassicBounce03
		"%s%d\n" //ClassicBounce04
		"%s%d\n" //ClassicBounce05
		"%s%d\n" //ClassicBounceDrop
		"%s%d\n", //ClassicBounceMulti
		"[Modern]",
		"ModernBounceEnable=", ModernBounceEnable,
		"ModernBounceTrigger=", ModernBounceTrigger,
		"ModernSoundType=", ModernSoundType,
		"ModernBounceVO=", ModernBounceVO,
		"ModernBlueTrail=", ModernBlueTrail,
		"ModernBallType=", ModernBallType,
		"ModernLWBounce=", ModernLWBounce,
		"ModernBounceHorCnl=", ModernBounceHorCnl,
		"ModernNoBounceEnemy=", ModernNoBounceEnemy,
		"ModernGroundActionType=", ModernGroundActionType,
		"ModernBounceTricking=", ModernBounceTricking,
		"ModernBounce01=", ModernBounce01,
		"ModernBounce02=", ModernBounce02,
		"ModernBounce03=", ModernBounce03,
		"ModernBounce04=", ModernBounce04,
		"ModernBounce05=", ModernBounce05,
		"ModernBounceDrop=", ModernBounceDrop,
		"ModernBounceMulti=", ModernBounceMulti,
		"[Classic]",
		"ClassicBounceEnable", ClassicBounceEnable,
		"ClassicBounceTrigger", ClassicBounceTrigger,
		"ClassicSoundType=", ClassicSoundType,
		"ClassicBlueTrail=", ClassicBlueTrail,
		"ClassicLWBounce", ClassicLWBounce,
		"ClassicBounceHorCnl", ClassicBounceHorCnl,
		"ClassicNoBounceEnemy", ClassicNoBounceEnemy,
		"ClassicBounceTricking", ClassicBounceTricking,
		"ClassicBounce01", ClassicBounce01,
		"ClassicBounce02", ClassicBounce02,
		"ClassicBounce03", ClassicBounce03,
		"ClassicBounce04", ClassicBounce04,
		"ClassicBounce05", ClassicBounce05,
		"ClassicBounceDrop", ClassicBounceDrop,
		"ClassicBounceMulti=", ClassicBounceMulti);
	fputs(buffer, iniFile);
	fclose(iniFile);
}

void ReadINI(std::string saveFilePath)
{
	//printf("READING INI");
	printf("\n");
	INIReader* reader = new INIReader(saveFilePath);
	if (reader->ParseError() != 0)
	{
		printf("INI PARSE FAIL");
		printf("\n");
		FILE* pFile = fopen(saveFilePath.c_str(), "wb");
		WriteINI(pFile);
		reader = new INIReader(saveFilePath);
	}

	//Modern Sonic
	ModernBounceEnable = reader->GetBoolean("Modern", "ModernBounceEnable", ModernBounceEnable);
	ModernBounceTrigger = reader->GetBoolean("Modern", "ModernBounceTrigger", ModernBounceTrigger);
	ModernSoundType = reader->GetInteger("Modern", "ModernSoundType", ModernSoundType);
	ModernBounceVO = reader->GetBoolean("Modern", "ModernBounceVO", ModernBounceVO);
	ModernBlueTrail = reader->GetBoolean("Modern", "ModernBlueTrail", ModernBlueTrail);
	ModernBallType = reader->GetInteger("Modern", "ModernBallType", ModernBallType);
	SelectBounceBall = (SelectBounceBallType)ModernBallType;
	ModernLWBounce = reader->GetBoolean("Modern", "ModernLWBounce", ModernLWBounce);
	ModernBounceHorCnl = reader->GetBoolean("Modern", "ModernBounceHorCnl", ModernBounceHorCnl);
	ModernNoBounceEnemy = reader->GetBoolean("Modern", "ModernNoBounceEnemy", ModernNoBounceEnemy);
	ModernGroundActionType = reader->GetInteger("Modern", "ModernGroundActionType", ModernGroundActionType);
	ModernBounceTricking = reader->GetBoolean("Modern", "ModernBounceTricking", ModernBounceTricking);
	ModernBounce01 = reader->GetFloat("Modern", "ModernBounce01", ModernBounce01);
	ModernBounce02 = reader->GetFloat("Modern", "ModernBounce02", ModernBounce02);
	ModernBounce03 = reader->GetFloat("Modern", "ModernBounce03", ModernBounce03);
	ModernBounce04 = reader->GetFloat("Modern", "ModernBounce04", ModernBounce04);
	ModernBounce05 = reader->GetFloat("Modern", "ModernBounce05", ModernBounce05);
	ModernBounceDrop = reader->GetFloat("Modern", "ModernBounceDrop", ModernBounceDrop);
	ModernBounceMulti = reader->GetFloat("Modern", "ModernBounceMulti", ModernBounceMulti);
	//Classic
	ClassicBounceEnable = reader->GetBoolean("Classic", "ClassicBounceEnable", ClassicBounceEnable);
	ClassicBounceTrigger = reader->GetBoolean("Classic", "ClassicBounceTrigger", ClassicBounceTrigger);
	ClassicSoundType = reader->GetInteger("Classic", "ClassicSoundType", ClassicSoundType);
	ClassicBlueTrail = reader->GetBoolean("Classic", "ClassicBlueTrail", ClassicBlueTrail);
	ClassicLWBounce = reader->GetBoolean("Classic", "ClassicLWBounce", ClassicLWBounce);
	ClassicBounceHorCnl = reader->GetBoolean("Classic", "ClassicBounceHorCnl", ClassicBounceHorCnl);
	ClassicNoBounceEnemy = reader->GetBoolean("Classic", "ClassicNoBounceEnemy", ClassicNoBounceEnemy);
	ClassicBounceTricking = reader->GetBoolean("Classic", "ClassicBounceTricking", ClassicBounceTricking);
	ClassicBounce02 = reader->GetFloat("Classic", "ClassicBounce02", ClassicBounce02);
	ClassicBounce03 = reader->GetFloat("Classic", "ClassicBounce03", ClassicBounce03);
	ClassicBounce01 = reader->GetFloat("Classic", "ClassicBounce01", ClassicBounce01);
	ClassicBounce04 = reader->GetFloat("Classic", "ClassicBounce04", ClassicBounce04);
	ClassicBounce05 = reader->GetFloat("Classic", "ClassicBounce05", ClassicBounce05);
	ClassicBounceDrop = reader->GetFloat("Classic", "ClassicBounceDrop", ClassicBounceDrop);
	ClassicBounceMulti = reader->GetFloat("Classic", "ClassicBounceMulti", ClassicBounceMulti);
}
