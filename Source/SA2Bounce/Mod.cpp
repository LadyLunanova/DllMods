#include "NPCAnim.h"

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

bool checkCode = false;
int BounceCount = 0; //Bounce counter
SharedPtrTypeless BounceBallVfxHandle; //Ball VFX handler
SharedPtrTypeless BounceTrailVfxHandle; //Following balls VFX handler
SharedPtrTypeless BounceLandVfxHandle; // Bounce landing VFX handler
SharedPtrTypeless ClassicBounceBallVfxHandle; // Bounce landing VFX handler
////Config Options////
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
//Classic
bool  ClassicBounceEnable = true;
bool  ClassicBounceTrigger = true;
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

enum SelectJumpBallVFXType
{
	JumpBallBAP,
	JumpBallDefault,
	JumpBallSWA,
	JumpBallBetaSWA,
	JumpBallLW,
	JumpBallForces,
	JumpBallSA1,
	JumpBallNoVFX,
};
SelectJumpBallVFXType SelectJumpBallVFX = SelectJumpBallVFXType::JumpBallBAP;

//////Renderables//////
//boost::shared_ptr<hh::mr::CSingleElement> m_spSonicJumpBall;
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
		auto BallNode = m_spElement->GetNode("mesh_ball"); //Set up bone matrix for VFX
		Common::fCGlitterCreate(cpcontext->m_spContext.get(), SA2ballVfxHandle, &BallNode, "ef_ch_sng_yh1_sa2spinattack", 1);  //Create VFX
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

		//if (m_spElement->m_Enabled && isVisible && !typeFlicker)
		//{
		//	pPlayer->m_spCharacterModel->m_Enabled = false;
		//	isVisible = false;
		//}

		//flickerTimer += in_rUpdateInfo.DeltaTime;
		//if (flickerTimer >= flickerTimerMax)
		//{
		//	flickerTimer = 0.0f;
		//	typeFlicker = !typeFlicker;
		//}

		//if (typeFlicker)
		//{
		//	pPlayer->m_spCharacterModel->m_Enabled = isVisible;
		//	isVisible = !isVisible;
		//	m_spElement->m_Enabled = isVisible;
		//}
		//else if (!m_spElement->m_Enabled)
		//{
		//	pPlayer->m_spCharacterModel->m_Enabled = false;
		//	isVisible = false;
		//	m_spElement->m_Enabled = true;
		//}

		//printf("%f\n", m_spNPCAnimation->m_spAnimationStateMachine->m_Time);
	}

	void KillCallback() override
	{
		const int playerID = GetGameDocument()->m_pMember->m_PlayerIDs.begin()[0];
		const Sonic::Player::CPlayerSpeed* pPlayer = static_cast<Sonic::Player::CPlayerSpeed*>(m_pMessageManager->GetMessageActor(playerID));
		const Sonic::Player::CPlayer* cpcontext = static_cast<Sonic::Player::CPlayer*>(m_pMessageManager->GetMessageActor(playerID));
		//pPlayer->m_spCharacterModel->m_Enabled = true;
		Common::fCGlitterEnd(cpcontext->m_spContext.get(), SA2ballVfxHandle, true); //Destroy Ball VFX
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

//Base Sonic Handling
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;
	auto IsGrounded = This->GetContext()->m_Grounded;
	auto IsOutOfControl = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_OutOfControl];
	auto IsDisableStomping = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping];
	auto IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);
	auto IsClassicSonic = (Sonic::Player::CSonicClassicContext::GetInstance() != nullptr);
	bool IsStomping = This->m_StateMachine.GetCurrentState()->GetStateName() == "Stomping";
	bool IsJumping = This->m_StateMachine.GetCurrentState()->GetStateName() == "Jump";
	bool IsFalling = This->m_StateMachine.GetCurrentState()->GetStateName() == "Fall";
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedB = input.IsTapped(Sonic::eKeyState_B);
	bool PressedRT = input.IsTapped(Sonic::eKeyState_RightTrigger);
	bool PressedLT = input.IsTapped(Sonic::eKeyState_LeftTrigger);
	bool PressedTrigger = PressedRT || PressedLT;
	bool HoldB = input.IsDown(Sonic::eKeyState_B);

	if (IsGrounded && !IsStomping && !IsJumping && !IsFalling)
	{
		BounceCount = 0;
	}

	//Classic Sonic
	if (IsClassicSonic)
	{
		if (ClassicLWBounce && (BounceCount >= 3))
			sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping] = 1;
		if (!IsDisableStomping && !IsGrounded && !IsOutOfControl && (IsJumping || IsFalling) && ClassicBounceEnable)
		{
			if (PressedB || (PressedTrigger && ClassicBounceTrigger))
				sonic->ChangeState("Stomping");
		}
	}

	//Modern Sonic
	if (IsModernSonic)
	{
		if (ModernLWBounce && (BounceCount >= 3))
			sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping] = 1;
		if (!IsDisableStomping && !IsGrounded && !IsOutOfControl && (IsJumping || IsFalling) && ModernBounceEnable)
		{
			if (PressedTrigger && ModernBounceTrigger)
				sonic->ChangeState("Stomping");
		}
	}

	//printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	//printf(sonic->GetCurrentAnimationName().c_str());
	//printf("%d", sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_OnWater]);
	//printf("%d", sonic->m_Field164);
	//printf("%d", sonic->m_Field16C);
	//printf("%f,%f,%f",sonic->m_Velocity.x(),sonic->m_Velocity.y(),sonic->m_Velocity.z());
	//printf("%d",sonic->m_VelocityDirty);
	//printf("%f",sonic->m_ChaosEnergy);
	//printf("%d",sonic->m_Is2DMode);
	//printf("%d",BounceCount);
	//printf("%d", ModernRangersBounceTimer);
	//printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}

//Modern Sonic States
HOOK(void, __fastcall, EnterStompBounce, 0x01254CA0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (!ModernBounceEnable)
	{
		WRITE_MEMORY(0x16D6474, size_t, 0x01254870); //Set stomp's processMessage to the original
		originalEnterStompBounce(This);
		return;
	}

	if (!ModernNoBounceEnemy) //Check config setting
	{
		WRITE_MEMORY(0x16D6474, size_t, 0x11BCAB0); //Replace stomp's processMessage with jump's
	}
	else
	{
		WRITE_MEMORY(0x16D6474, size_t, 0x01254870); //Set stomp's processMessage to the original
	}

	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto pPlayer = sonic->m_pPlayer;
	bool IsSuper = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30); //Set up center matrix for VFX
	Common::SonicContextSetCollision(TypeSonicStomping, true, sonic); //Set sonic's collision type to stomping
	sonic->ChangeAnimation("JumpBall"); //Play ball animation

	if (!IsSuper)
	{
		switch (SelectJumpBallVFX)
		{
		case (enum SelectJumpBallVFXType)JumpBallNoVFX:
			return;
			break;
		case (enum SelectJumpBallVFXType)JumpBallBAP:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_bounceattack", 1);  //Create Ball VFX
			break;
		case (enum SelectJumpBallVFXType)JumpBallForces:
			obj_SonicJumpBallWarsRenderable = boost::make_shared<JumpballWarsAnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallWarsRenderable);
			pPlayer->m_spCharacterModel->m_Enabled = false;
			break;
		case (enum SelectJumpBallVFXType)JumpBallBetaSWA:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_swaspinattack", 1);  //Create Ball VFX
			break;
		case (enum SelectJumpBallVFXType)JumpBallSWA:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_swaretailspinattack", 1);  //Create Ball VFX
			break;
		case (enum SelectJumpBallVFXType)JumpBallSA1:
			obj_SonicJumpBallSA1Renderable = boost::make_shared<JumpballSA1AnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallSA1Renderable);
			pPlayer->m_spCharacterModel->m_Enabled = false;
			break;
		case (enum SelectJumpBallVFXType)JumpBallLW:
			obj_SonicJumpBallLWRenderable = boost::make_shared<JumpballLWAnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallLWRenderable);
			pPlayer->m_spCharacterModel->m_Enabled = false;
			break;
		default:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_spinattack", 1);  //Create Ball VFX
			break;
		}
	}
	else
	{
		switch (SelectJumpBallVFX)
		{
		case (enum SelectJumpBallVFXType)JumpBallNoVFX:
			return;
			break;
		case (enum SelectJumpBallVFXType)JumpBallBAP:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_bounceattack", 1);
			break;
		case (enum SelectJumpBallVFXType)JumpBallForces:
			
			break;
		case (enum SelectJumpBallVFXType)JumpBallBetaSWA:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_swaspinattack", 1);
			break;
		case (enum SelectJumpBallVFXType)JumpBallSWA:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_swaretailspinattack", 1);
			break;
		case (enum SelectJumpBallVFXType)JumpBallSA1:
			obj_SonicJumpBallSA1Renderable = boost::make_shared<JumpballSA1AnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallSA1Renderable);
			break;
		case (enum SelectJumpBallVFXType)JumpBallLW:
			obj_SonicJumpBallLWRenderable = boost::make_shared<JumpballLWAnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallLWRenderable);
			break;
		default:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_spinattack", 1);
			break;
		}
	}

	if (ModernBounceHorCnl)
	{
		sonic->m_Velocity.z() = 0.0;
		sonic->m_Velocity.x() = 0.0;
	}

	if (ModernBlueTrail)
		Common::SonicContextSpawnLocusEffect(sonic); //Spawn blue trail

	sonic->m_Velocity.y() = -ModernBounceDrop; //Set Sonic's vertical global velocity
}
HOOK(void, __fastcall, StompBounce, 0x012548C0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (!ModernBounceEnable)
	{
		originalStompBounce(This);
		return;
	}
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto player = sonic->m_pPlayer;
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;
	auto Flags = sonic->m_pStateFlag;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedA = input.IsTapped(Sonic::eKeyState_A);
	bool PressedX = input.IsTapped(Sonic::eKeyState_X);
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	bool HoldB = input.IsDown(Sonic::eKeyState_B);
	bool OrigStompCond = Flags->m_Flags[sonic->eStateFlag_DisableStomping];
	auto IsDisableStomping = Flags->m_Flags[sonic->eStateFlag_DisableStomping];
	bool IsInWater = Flags->m_Flags[sonic->eStateFlag_OnWater];
	bool IsSuper = Flags->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
	auto IsGrounded = sonic->m_Grounded;
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	void* groundmatrixNode = (void*)((uint32_t)sonic + 0x10);
	int rand = std::rand() % 3;

	sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EnableHomingAttack] = 1; //Set allow homing attacking flag
	sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping] = 1; //Set stomp disable flag

	FUNCTION_PTR(bool, __thiscall, CommonActSwitch, 0x00E012A0, Sonic::Player::CPlayerSpeedContext * a1, hh::fnd::CStateMachineBase::CStateBase * a2);
	if (CommonActSwitch(sonic, This))
	{
		Common::fCGlitterEnd(sonic, BounceBallVfxHandle, true); //Destroy Ball VFX
		return;
	}

	sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping] = OrigStompCond; //Set the stomp flag back to what it originally was

	if (IsGrounded)  //Check if sonic is touching the ground and check config option
	{
		Common::fCGlitterEnd(sonic, BounceBallVfxHandle, true); //Destroy Ball VFX

		if ((ModernGroundActionType == 1 && !HoldB) || (ModernGroundActionType == 2 && HoldB) || (ModernGroundActionType == 3)) //Check ground type and if holding B
		{
			sonic->PlaySound(2002043, true); //Play stomp land SFX
			Common::fCGlitterCreate(sonic, BounceLandVfxHandle, groundmatrixNode, "ef_ch_sng_yh1_stomping2", 1);  //Create Stomp Land VFX

			if (localVelocity.z() > 25.0f) //Check forward speed
			{
				player->m_StateMachine.ChangeState("Sliding"); //Change state to slide state
			}
			else
			{
				localVelocity.z() = 0.0f;
				localVelocity.x() = 0.0f;
				player->m_StateMachine.ChangeState("StompingLand"); //Change state to stomping land state
				//originalStompBounce(This);
			}
		}
		else
		{
			switch (ModernSoundType)
			{
			case 0:
				sonic->PlaySound(694206660, true); //Play bounce SFX
				break;
			case 1:
				sonic->PlaySound(694206661, true); //Play bounce SFX
				break;
			case 2:
				sonic->PlaySound(694206662, true); //Play bounce SFX
				break;
			case 3:
				sonic->PlaySound(2002027, true); //Play bounce SFX
				break;
			case 4:
				sonic->PlaySound(2002043, true); //Play bounce SFX
				break;
			}
			//MessageBoxA(nullptr, "SFX time", "Window", MB_OK);

			if (ModernBounceVO)
			{
				//sonic->PlaySound(3002008, false); //Play voiceline
				switch (rand)
				{
				case 0:
					sonic->PlaySound(3002008, false); //Play voiceline
					break;
				case 1:
					sonic->PlaySound(3002008, false); //Play voiceline
					break;
				}
			}

			if (IsSuper)
			{
				if (ModernBallType != 2)
					Common::fCGlitterCreate(sonic, BounceLandVfxHandle, groundmatrixNode, "ef_ch_sps_yh1_bounceland", 1);  //Create Super Stomp Land VFX
			}
			else
			{
				if (ModernBallType != 2)
					Common::fCGlitterCreate(sonic, BounceLandVfxHandle, groundmatrixNode, "ef_ch_sng_yh1_bounceland", 1);  //Create Normal Stomp Land VFX
			}

			if (IsInWater || ModernBounceTricking || (ModernLWBounce && BounceCount >= 2)) //Check for tricking option
			{
				player->m_StateMachine.ChangeState("HomingAttackAfter"); //Change state to homing attack tricking state
			}
			else
			{
				player->m_StateMachine.ChangeState("Jump"); //Change state to jump state
			}

			switch (BounceCount) //Check how many times sonic has bounced
			{
			case 0:
				localVelocity.y() = ModernBounce01; //Config First bounce height
				break;
			case 1:
				localVelocity.y() = ModernBounce02; //Config Second bounce height
				break;
			case 2:
				localVelocity.y() = ModernBounce03; //Config Third bounce height
				break;
			case 3:
				localVelocity.y() = ModernBounce04; //Config Fourth bounce height
				break;
			default:
				localVelocity.y() = ModernBounce05; //Config Final bounce height
				break;
			}

			//player->m_StateMachine.ChangeState("Jump"); //Change state to jump state
			//player->m_StateMachine.ChangeState("Fall"); //Change state to jump state

			sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EnableHomingAttack] = 1; //Set allow homing attack flag
			BounceCount++; //Add to bounce int after bouncing
		}
	}
	else if (ModernBounceHorCnl)
	{
		localVelocity.z() = 0.0f;
		localVelocity.x() = 0.0f;
	}

	localVelocity.y() += (-1.022 * ModernBounceMulti); //Add downward acceleration

	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
}
HOOK(void, __fastcall, ExitStompBounce, 0x01254B80, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalExitStompBounce(This);
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto pPlayer = sonic->m_pPlayer;
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool HoldB = input.IsDown(Sonic::eKeyState_B);
	if ((ModernGroundActionType == 1 && !HoldB) || (ModernGroundActionType == 2 && HoldB) || (ModernGroundActionType == 3) || ModernBounceHorCnl)
	{
		localVelocity.z() = 0.0f;
		localVelocity.x() = 0.0f;
	}
	pPlayer->m_spCharacterModel->m_Enabled = true;
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
	Common::fCGlitterEnd(sonic, BounceBallVfxHandle, true); //Destroy Ball VFX
	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
}

//Classic Sonic States
HOOK(void, __fastcall, EnterClassicStompBounce, 0x012555D0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (!ClassicBounceEnable)
	{
		WRITE_MEMORY(0x16D9268, size_t, 0x00E49B10); //Set classic stomp's processMessage to the original
		originalEnterStompBounce(This);
		return;
	}

	if (!ClassicNoBounceEnemy) //Check config setting
	{
		WRITE_MEMORY(0x16D9268, size_t, 0x1114EB0); //Replace classic stomp's processMessage with jump's
	}
	else
	{
		WRITE_MEMORY(0x16D9268, size_t, 0x00E49B10); //Set classic stomp's processMessage to the original
	}

	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto player = sonic->m_pPlayer;
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30); //Set up center matrix for VFX
	Common::SonicContextSetCollision(TypeSonicStomping, true, sonic); //Set sonic's collision type to stomping
	sonic->ChangeAnimation("JumpBall"); //Play ball animation

	Common::fCGlitterCreate(sonic, ClassicBounceBallVfxHandle, middlematrixNode, "ef_ch_snc_yh1_spindash1", 1);  //Create Ball VFX
	if (ClassicBlueTrail)
		Common::SonicContextSpawnLocusEffect(sonic); //Spawn blue trail

	if (ClassicBounceHorCnl)
	{
		sonic->m_Velocity.z() = 0.0;
		sonic->m_Velocity.x() = 0.0;
	}

	sonic->m_Velocity.y() = -ClassicBounceDrop; //Set Sonic's vertical global velocity
}
HOOK(void, __fastcall, ClassicStompBounce, 0x01255240, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (!ClassicBounceEnable)
	{
		originalStompBounce(This);
		return;
	}
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto player = sonic->m_pPlayer;
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;
	auto Flags = sonic->m_pStateFlag;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedA = input.IsTapped(Sonic::eKeyState_A);
	bool PressedX = input.IsTapped(Sonic::eKeyState_X);
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	bool HoldB = input.IsDown(Sonic::eKeyState_B);
	bool OrigStompCond = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping];
	bool IsInWater = Flags->m_Flags[sonic->eStateFlag_OnWater];
	auto IsGrounded = sonic->m_Grounded;
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	void* groundmatrixNode = (void*)((uint32_t)sonic + 0x10);

	sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EnableHomingAttack] = 1; //Set allow homing attacking flag
	sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping] = 1; //Set stomp disable flag

	FUNCTION_PTR(bool, __thiscall, CommonActSwitch, 0x00E012A0, Sonic::Player::CPlayerSpeedContext * a1, hh::fnd::CStateMachineBase::CStateBase * a2);
	if (CommonActSwitch(sonic, This))
	{
		Common::fCGlitterEnd(sonic, ClassicBounceBallVfxHandle, true); //Destroy Ball VFX
		//Common::fCGlitterEnd(*PLAYER_CONTEXT, BounceTrailVfxHandle, false); //Stop Creating Ball Trail VFX
		return;
	}

	sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping] = OrigStompCond; //Set the stomp flag back to what it originally was

	if (IsGrounded)  //Check if sonic is touching the ground and check config option
	{
		sonic->PlaySound(2001027, true); //Play bounce SFX

		if (ClassicBounceTricking || IsInWater || (ClassicLWBounce && BounceCount >= 2)) //Check for tricking option and water
		{
			player->m_StateMachine.ChangeState("HomingAttackAfter"); //Change state to homing attack tricking state
		}
		else
		{
			player->m_StateMachine.ChangeState("Jump"); //Change state to jump state
		}

		switch (BounceCount) //Check how many times sonic has bounced
		{
		case 0:
			localVelocity.y() = ClassicBounce01; //Config First bounce height
			break;
		case 1:
			localVelocity.y() = ClassicBounce02; //Config Second bounce height
			break;
		case 2:
			localVelocity.y() = ClassicBounce03; //Config Third bounce height
			break;
		case 3:
			localVelocity.y() = ClassicBounce04; //Config Fourth bounce height
			break;
		default:
			localVelocity.y() = ClassicBounce05; //Config Final bounce height
			break;
		}

		sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EnableHomingAttack] = 1; //Set allow homing attack flag
		BounceCount++; //Add to bounce int after bouncing

	}

	localVelocity.y() += (-1.022 * ModernBounceMulti); //Add downward acceleration

	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
}
HOOK(void, __fastcall, ExitClassicStompBounce, 0x012554C0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (!ClassicBounceEnable)
	{
		ExitClassicStompBounce(This);
		return;
	}
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;
	auto SparkEffectManager = sonic->m_pSparkEffectManager;
	FUNCTION_PTR(void*, __stdcall, StopLocusEffect, 0x00E8C940, void* a1, const hh::base::CSharedString & sharedString);
	if (ClassicBounceHorCnl)
	{
		localVelocity.z() > 0.0f;
		localVelocity.x() > 0.0f;
	}
	Common::SonicContextSetCollision(TypeSonicStomping, false, sonic);
	Common::fCGlitterEnd(sonic, ClassicBounceBallVfxHandle, true); //Destroy Ball VFX
	if (SparkEffectManager)
	{
		StopLocusEffect(SparkEffectManager, "HomingAttack");
	}
	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
}

//Debug
HOOK(void, __cdecl, InitializeApplicationParams_BOUNCE, 0x00D65180, Sonic::CParameterFile* This)
{
	auto parameterGroupGen = This->CreateParameterGroup("Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce_Gen = parameterGroupGen->CreateParameterCategory("Bounce Attack+ Modern", "Parameters for Modern Sonic in Bounce Attack+");

	//cat_Bounce->CreateParamInt(&someInt, "Integer");
	//cat_Bounce->CreateParamBool(&someBool, "Bool");
	//cat_Bounce->CreateParamFloat(&someFloat, "Float");

	cat_Bounce_Gen->CreateParamBool(&ModernBounceEnable, "Enable bouncing for Modern Sonic");
	cat_Bounce_Gen->CreateParamBool(&ModernBounceTrigger, "Enable bouncing when pressing one of the triggers mid-air");
	cat_Bounce_Gen->CreateParamTypeList((uint32_t*)&ModernSoundType, "Sound Type", "Choose what SFX gets played when you bounce",
		{
			{ "SA2", 0},
			{ "06", 1},
			{ "LW", 2},
			{ "Jump", 3},
			{ "Stomp", 4},
		});
	cat_Bounce_Gen->CreateParamBool(&ModernBounceVO, "Play one of Sonic's voicelines when you bounce");
	cat_Bounce_Gen->CreateParamBool(&ModernBlueTrail, "Enable Blue Trail Creation");
	cat_Bounce_Gen->CreateParamTypeList((uint32_t*)&SelectJumpBallVFX, "Ball VFX Type", "Choose what VFX gets used when bouncing",
		{
			{ "Bounce Attack+", (enum SelectJumpBallVFXType)JumpBallBAP},
			{ "Main Jumpball", (enum SelectJumpBallVFXType)JumpBallDefault},
			{ "Unleashed Ball VFX", (enum SelectJumpBallVFXType)JumpBallSWA},
			{ "Beta Unleashed Ball VFX", (enum SelectJumpBallVFXType)JumpBallBetaSWA},
			{ "Lost World Ball", (enum SelectJumpBallVFXType)JumpBallLW},
			{ "Forces Ball VFX", (enum SelectJumpBallVFXType)JumpBallForces},
			{ "Adventure Ball", (enum SelectJumpBallVFXType)JumpBallSA1},
			{ "No Ball VFX", (enum SelectJumpBallVFXType)JumpBallNoVFX},
		});
	cat_Bounce_Gen->CreateParamBool(&ModernLWBounce, "Lost World Style Bounce");
	cat_Bounce_Gen->CreateParamBool(&ModernBounceHorCnl, "Cancel Horizontal Momentum");
	cat_Bounce_Gen->CreateParamBool(&ModernNoBounceEnemy, "Bounce goes through enemies");
	cat_Bounce_Gen->CreateParamTypeList((uint32_t*)&ModernGroundActionType, "Ground Action Type", "The behavior for bouncing on the ground",
		{
			{ "Always Bounce", 0},
			{ "Hold B to Bounce", 1},
			{ "Hold B to Stomp/Slide", 2},
			{ "Always Stomp", 3},
		});
	cat_Bounce_Gen->CreateParamBool(&ModernBounceTricking, "Bounce Tricking");
	cat_Bounce_Gen->CreateParamFloat(&ModernBounce01, "1st bounce height");
	cat_Bounce_Gen->CreateParamFloat(&ModernBounce02, "2nd bounce height");
	cat_Bounce_Gen->CreateParamFloat(&ModernBounce03, "3rd bounce height");
	cat_Bounce_Gen->CreateParamFloat(&ModernBounce04, "4th bounce height");
	cat_Bounce_Gen->CreateParamFloat(&ModernBounce05, "5th bounce height");
	cat_Bounce_Gen->CreateParamFloat(&ModernBounceDrop, "Initial downward force");
	cat_Bounce_Gen->CreateParamFloat(&ModernBounceMulti, "Downward acceleration multiplier");

	parameterGroupGen->Flush();

	////Classic Sonic////

	auto parameterGroupCla = This->CreateParameterGroup("Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce_Cla = parameterGroupCla->CreateParameterCategory("Bounce Attack+ Classic", "Parameters for Classic Sonic in Bounce Attack+");

	cat_Bounce_Cla->CreateParamBool(&ClassicBounceEnable, "Enable bouncing for Modern Sonic");
	cat_Bounce_Cla->CreateParamBool(&ClassicBounceTrigger, "Enable bouncing when pressing one of the triggers mid-air");
	cat_Bounce_Cla->CreateParamBool(&ClassicBlueTrail, "Enable Blue Trail Creation");
	cat_Bounce_Cla->CreateParamBool(&ClassicLWBounce, "Lost World Style Bounce");
	cat_Bounce_Cla->CreateParamBool(&ClassicBounceHorCnl, "Cancel Horizontal Momentum");
	cat_Bounce_Cla->CreateParamBool(&ClassicNoBounceEnemy, "Bounce goes through enemies");
	cat_Bounce_Cla->CreateParamBool(&ClassicBounceTricking, "Bounce Tricking");
	cat_Bounce_Cla->CreateParamFloat(&ClassicBounce01, "1st bounce height");
	cat_Bounce_Cla->CreateParamFloat(&ClassicBounce02, "2nd bounce height");
	cat_Bounce_Cla->CreateParamFloat(&ClassicBounce03, "3rd bounce height");
	cat_Bounce_Cla->CreateParamFloat(&ClassicBounce04, "4th bounce height");
	cat_Bounce_Cla->CreateParamFloat(&ClassicBounce05, "5th bounce height");
	cat_Bounce_Cla->CreateParamFloat(&ClassicBounceDrop, "Initial downward force");
	cat_Bounce_Cla->CreateParamFloat(&ClassicBounceMulti, "Downward acceleration multiplier");

	parameterGroupCla->Flush();

	originalInitializeApplicationParams_BOUNCE(This);
}

EXPORT void OnFrame()
{
	if (checkCode)
		return;
	checkCode = true;
	constexpr int key = 0xFFB6F908;
	bool isNotModified = *(int*)0x012552E4 == key;

	if (!isNotModified)
	{
		MessageBoxA(nullptr, "The Unleashed Style Stomp code is not compatible with Bounce Attack+, Disable one before rebooting", "Code Incompatibility", MB_ICONERROR);
		exit(-1);
	}

}

EXPORT void Init()
{
	//MessageBoxA(nullptr, "Hook Now", "Window", MB_OK);
	INIReader reader("BAPBounce.ini");
	//Modern Sonic
	ModernBounceEnable = reader.GetBoolean("Modern", "ModernBounceEnable", ModernBounceEnable);
	ModernBounceTrigger = reader.GetBoolean("Modern", "ModernBounceTrigger", ModernBounceTrigger);
	ModernSoundType = reader.GetInteger("Modern", "ModernSoundType", ModernSoundType);
	ModernBounceVO = reader.GetBoolean("Modern", "ModernBounceVO", ModernBounceVO);
	ModernBallType = reader.GetInteger("Modern", "ModernBallType", ModernBallType);
	SelectJumpBallVFX = (enum SelectJumpBallVFXType)ModernBallType;
	ModernLWBounce = reader.GetBoolean("Modern", "ModernLWBounce", ModernLWBounce);
	ModernBounceHorCnl = reader.GetBoolean("Modern", "ModernBounceHorCnl", ModernBounceHorCnl);
	ModernNoBounceEnemy = reader.GetBoolean("Modern", "ModernNoBounceEnemy", ModernNoBounceEnemy);
	ModernGroundActionType = reader.GetInteger("Modern", "ModernGroundActionType", ModernGroundActionType);
	ModernBounceTricking = reader.GetBoolean("Modern", "ModernBounceTricking", ModernBounceTricking);
	ModernBounce01 = reader.GetFloat("Modern", "ModernBounce01", ModernBounce01);
	ModernBounce02 = reader.GetFloat("Modern", "ModernBounce02", ModernBounce02);
	ModernBounce03 = reader.GetFloat("Modern", "ModernBounce03", ModernBounce03);
	ModernBounce04 = reader.GetFloat("Modern", "ModernBounce04", ModernBounce04);
	ModernBounce05 = reader.GetFloat("Modern", "ModernBounce05", ModernBounce05);
	ModernBounceDrop = reader.GetFloat("Modern", "ModernBounceDrop", ModernBounceDrop);
	ModernBounceMulti = reader.GetFloat("Modern", "ModernBounceMulti", ModernBounceMulti);
	//Classic
	ClassicBounceEnable = reader.GetBoolean("Classic", "ClassicBounceEnable", ClassicBounceEnable);
	ClassicBounceTrigger = reader.GetBoolean("Classic", "ClassicBounceTrigger", ClassicBounceTrigger);
	ClassicLWBounce = reader.GetBoolean("Classic", "ClassicLWBounce", ClassicLWBounce);
	ClassicBounceHorCnl = reader.GetBoolean("Classic", "ClassicBounceHorCnl", ClassicBounceHorCnl);
	ClassicNoBounceEnemy = reader.GetBoolean("Classic", "ClassicNoBounceEnemy", ClassicNoBounceEnemy);
	ClassicBounceTricking = reader.GetBoolean("Classic", "ClassicBounceTricking", ClassicBounceTricking);
	ClassicBounce01 = reader.GetFloat("Classic", "ClassicBounce01", ClassicBounce01);
	ClassicBounce02 = reader.GetFloat("Classic", "ClassicBounce02", ClassicBounce02);
	ClassicBounce03 = reader.GetFloat("Classic", "ClassicBounce03", ClassicBounce03);
	ClassicBounce04 = reader.GetFloat("Classic", "ClassicBounce04", ClassicBounce04);
	ClassicBounce05 = reader.GetFloat("Classic", "ClassicBounce05", ClassicBounce05);
	ClassicBounceDrop = reader.GetFloat("Classic", "ClassicBounceDrop", ClassicBounceDrop);
	ClassicBounceMulti = reader.GetFloat("Classic", "ClassicBounceMulti", ClassicBounceMulti);
	//HOOKS
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(EnterStompBounce);
	INSTALL_HOOK(StompBounce);
	INSTALL_HOOK(ExitStompBounce);
	INSTALL_HOOK(EnterClassicStompBounce);
	INSTALL_HOOK(ClassicStompBounce);
	INSTALL_HOOK(ExitClassicStompBounce);
	INSTALL_HOOK(InitializeApplicationParams_BOUNCE);
}