// Original code by Brianuuu: https://github.com/brianuuu
using SharedPtrTypeless = boost::shared_ptr<void>;
namespace Common
{
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
}

//Dynamic Omnis
// Original code by Skyth: https://github.com/blueskythlikesclouds
hh::base::CRefPtr<Sonic::CLocalLight> genericPLight;
hh::base::CRefPtr<Sonic::CLocalLight> superPLight;
hh::base::CRefPtr<Sonic::CLocalLight> grindPLight;
void ParticleLight(float ColorR, float ColorG, float ColorB, float ColorA, float LightRange)
{
	auto gameDocument = Sonic::CGameDocument::GetInstance();
	if (!gameDocument.get().get() || !gameDocument->m_pMember->m_spLightManager)
		return;

	auto playerContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!playerContext)
		return;

	const auto& stateName = playerContext->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	const auto& animName = playerContext->GetCurrentAnimationName();

	if ((animName == "SpinAttack" && stateName == "Jump") || stateName == "Stomping" || stateName == "Sliding" || stateName == "SquatKick" || playerContext->StateFlag(eStateFlag_Boost))
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		if (stateName == "Sliding" || playerContext->StateFlag(eStateFlag_Boost))
			position += fwd * 0.5f + up * 0.25f;

		if (!genericPLight)
			if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
				superPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
			else
				genericPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { ColorR, ColorG, ColorB, ColorA }, { 0.0f, 0.0f, 0.0f, LightRange });
		else
			genericPLight->SetPosition(position);
	}
	else
		genericPLight = nullptr;

	if (stateName == "Grind" || stateName == "GrindSquat" || stateName == "GrindLandJumpShort")
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		if (!grindPLight)
			grindPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 2.0f, 0.666f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.5f });
		else
			grindPLight->SetPosition(position);
	}
	else
		grindPLight = nullptr;

	if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		if (stateName == "Sliding" || playerContext->StateFlag(eStateFlag_Boost))
			position += fwd * 0.5f + up * 0.25f;

		if (!superPLight)
			superPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
		else
			superPLight->SetPosition(position);
	}
	else
		superPLight = nullptr;
}

SharedPtrTypeless BoostJump_VfxHandle; //VFX handler
SharedPtrTypeless Warp_S_VfxHandle; //VFX handler
SharedPtrTypeless Warp_E_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus1R_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus2R_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus3R_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus4R_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus5R_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus1L_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus2L_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus3L_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus4L_VfxHandle; //VFX handler
SharedPtrTypeless JetLocus5L_VfxHandle; //VFX handler
SharedPtrTypeless Jet1R_VfxHandle; //VFX handler
SharedPtrTypeless Jet2R_VfxHandle; //VFX handler
SharedPtrTypeless Jet3R_VfxHandle; //VFX handler
SharedPtrTypeless Jet4R_VfxHandle; //VFX handler
SharedPtrTypeless Jet5R_VfxHandle; //VFX handler
SharedPtrTypeless Jet1L_VfxHandle; //VFX handler
SharedPtrTypeless Jet2L_VfxHandle; //VFX handler
SharedPtrTypeless Jet3L_VfxHandle; //VFX handler
SharedPtrTypeless Jet4L_VfxHandle; //VFX handler
SharedPtrTypeless Jet5L_VfxHandle; //VFX handler


void sonicNodeGlitterCreate(Sonic::Player::CPlayerSpeed* This, SharedPtrTypeless& VFXHandle, Hedgehog::Base::CSharedString boneNode, Hedgehog::Base::CSharedString GTEFile)
{
	auto sonic = This->GetContext();
	auto BoneNodeMatrix = This->m_spCharacterModel->GetNode(boneNode); //Set up bone matrix for VFX
	Eigen::Affine3f affine;
	affine = BoneNodeMatrix->m_WorldMatrix;

	if (VFXHandle == nullptr)
	{
		Common::fCGlitterCreate(sonic, VFXHandle, &BoneNodeMatrix, GTEFile, 1);  //Create VFX
	}
}
void sonicGlitterEnd(Sonic::Player::CPlayerSpeed* This, SharedPtrTypeless& VFXHandle, bool Kill)
{
	auto sonic = This->GetContext();
	Common::fCGlitterEnd(sonic, VFXHandle, Kill); //Destroy VFX
	VFXHandle = nullptr;
}

void shadowCreateJetLocus(Sonic::Player::CPlayerSpeed* This)
{
	sonicNodeGlitterCreate(This, JetLocus1R_VfxHandle, "EffectJet1_R", "ef_ch_sh_yh1_jetlocus");
	sonicNodeGlitterCreate(This, JetLocus2R_VfxHandle, "EffectJet2_R", "ef_ch_sh_yh1_jetlocus");
	sonicNodeGlitterCreate(This, JetLocus3R_VfxHandle, "EffectJet3_R", "ef_ch_sh_yh1_jetlocus");
	sonicNodeGlitterCreate(This, JetLocus4R_VfxHandle, "EffectJet4_R", "ef_ch_sh_yh1_jetlocus");
	sonicNodeGlitterCreate(This, JetLocus5R_VfxHandle, "EffectJet5_R", "ef_ch_sh_yh1_jetlocus");
	sonicNodeGlitterCreate(This, JetLocus1L_VfxHandle, "EffectJet1_L", "ef_ch_sh_yh1_jetlocus");
	sonicNodeGlitterCreate(This, JetLocus2L_VfxHandle, "EffectJet2_L", "ef_ch_sh_yh1_jetlocus");
	sonicNodeGlitterCreate(This, JetLocus3L_VfxHandle, "EffectJet3_L", "ef_ch_sh_yh1_jetlocus");
	sonicNodeGlitterCreate(This, JetLocus4L_VfxHandle, "EffectJet4_L", "ef_ch_sh_yh1_jetlocus");
	sonicNodeGlitterCreate(This, JetLocus5L_VfxHandle, "EffectJet5_L", "ef_ch_sh_yh1_jetlocus");
}
void shadowEndJetLocus(Sonic::Player::CPlayerSpeed* This)
{
	sonicGlitterEnd(This, JetLocus1R_VfxHandle, true);
	sonicGlitterEnd(This, JetLocus2R_VfxHandle, true);
	sonicGlitterEnd(This, JetLocus3R_VfxHandle, true);
	sonicGlitterEnd(This, JetLocus4R_VfxHandle, true);
	sonicGlitterEnd(This, JetLocus5R_VfxHandle, true);
	sonicGlitterEnd(This, JetLocus1L_VfxHandle, true);
	sonicGlitterEnd(This, JetLocus2L_VfxHandle, true);
	sonicGlitterEnd(This, JetLocus3L_VfxHandle, true);
	sonicGlitterEnd(This, JetLocus4L_VfxHandle, true);
	sonicGlitterEnd(This, JetLocus5L_VfxHandle, true);
}
void shadowCreateJet(Sonic::Player::CPlayerSpeed* This)
{
	sonicNodeGlitterCreate(This, Jet1R_VfxHandle, "EffectJet1_R", "ef_bo_sha_yh2_jet");
	sonicNodeGlitterCreate(This, Jet2R_VfxHandle, "EffectJet2_R", "ef_bo_sha_yh2_jet");
	sonicNodeGlitterCreate(This, Jet3R_VfxHandle, "EffectJet3_R", "ef_bo_sha_yh2_jet");
	sonicNodeGlitterCreate(This, Jet4R_VfxHandle, "EffectJet4_R", "ef_bo_sha_yh2_jet");
	sonicNodeGlitterCreate(This, Jet5R_VfxHandle, "EffectJet5_R", "ef_bo_sha_yh2_jet");
	sonicNodeGlitterCreate(This, Jet1L_VfxHandle, "EffectJet1_L", "ef_bo_sha_yh2_jet");
	sonicNodeGlitterCreate(This, Jet2L_VfxHandle, "EffectJet2_L", "ef_bo_sha_yh2_jet");
	sonicNodeGlitterCreate(This, Jet3L_VfxHandle, "EffectJet3_L", "ef_bo_sha_yh2_jet");
	sonicNodeGlitterCreate(This, Jet4L_VfxHandle, "EffectJet4_L", "ef_bo_sha_yh2_jet");
	sonicNodeGlitterCreate(This, Jet5L_VfxHandle, "EffectJet5_L", "ef_bo_sha_yh2_jet");
}
void shadowEndJet(Sonic::Player::CPlayerSpeed* This)
{
	sonicGlitterEnd(This, Jet1R_VfxHandle, true);
	sonicGlitterEnd(This, Jet2R_VfxHandle, true);
	sonicGlitterEnd(This, Jet3R_VfxHandle, true);
	sonicGlitterEnd(This, Jet4R_VfxHandle, true);
	sonicGlitterEnd(This, Jet5R_VfxHandle, true);
	sonicGlitterEnd(This, Jet1L_VfxHandle, true);
	sonicGlitterEnd(This, Jet2L_VfxHandle, true);
	sonicGlitterEnd(This, Jet3L_VfxHandle, true);
	sonicGlitterEnd(This, Jet4L_VfxHandle, true);
	sonicGlitterEnd(This, Jet5L_VfxHandle, true);
}

//---------------------------------------------------
// Main Func Shit
//---------------------------------------------------

HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	//---------------------------------------------------
	// Locals/Setup
	//---------------------------------------------------

	auto sonic = This->GetContext();
	auto pPlayer = sonic->m_pPlayer;
	const auto GetAnimInfo = boost::make_shared<Sonic::Message::MsgGetAnimationInfo>();
	sonic->m_pPlayer->SendMessageImm(sonic->m_pPlayer->m_ActorID, GetAnimInfo);
	bool isSkateAnim =
		GetAnimInfo->m_Name == "Boost" ||
		GetAnimInfo->m_Name == "Jet" ||
		GetAnimInfo->m_Name == "Dash";
	

	//---------------------------------------------------
	// Main Run Code
	//---------------------------------------------------

	if (isSkateAnim)
	{
		shadowCreateJet(This);
		//shadowCreateJetLocus(This);
	}
	else
	{
		shadowEndJet(This);
		//shadowEndJetLocus(This);
	}

	//---------------------------------------------------
	// Console Stuff
	//---------------------------------------------------
	 
	//printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str()); 
	//printf(sonic->GetCurrentAnimationName().c_str());
	//printf(" - ");
	printf(GetAnimInfo->m_Name.c_str());
	//printf(" - ");
	//printf("%f", GetAnimInfo->m_Frame);
	printf("\n");

	//---------------------------------------------------
	// Call Original Player Update
	//---------------------------------------------------

	originalCPlayerSpeedUpdate(This, _, updateInfo);

	//Particle Omni Lights
	ParticleLight(1.0, 0.32, 0.0, 1.0, 4.2);
}

HOOK(void, __fastcall, CSonicStateHomingAttackStart, 0x01232040, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto pPlayer = sonic->m_pPlayer;
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	void* groundmatrixNode = (void*)((uint32_t)sonic + 0x10);
	auto sonic_spMatrix = sonic->m_spMatrixNode.get();
	boost::shared_ptr<Sonic::CMatrixNodeTransform> m_spVFXMatrix;

	Common::fCGlitterCreate(sonic, Warp_S_VfxHandle, groundmatrixNode, "ef_ch_sha_yh2_warp_s", 1);  //Create VFX
	pPlayer->m_spCharacterModel->m_Enabled = false;
	printf("ENTER HOMING ATTACK\n");
	originalCSonicStateHomingAttackStart(This);
}

HOOK(void, __fastcall, CSonicStateHomingAttackEnd, 0x01231F80, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto pPlayer = sonic->m_pPlayer;
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	void* groundmatrixNode = (void*)((uint32_t)sonic + 0x10);
	auto sonic_spMatrix = sonic->m_spMatrixNode.get();
	boost::shared_ptr<Sonic::CMatrixNodeTransform> m_spVFXMatrix;

	Common::fCGlitterCreate(sonic, Warp_E_VfxHandle, groundmatrixNode, "ef_ch_sha_yh2_warp_e", 1);  //Create VFX
	pPlayer->m_spCharacterModel->m_Enabled = true;
	//Common::fCGlitterEnd(sonic, Warp_S_VfxHandle, false); //Destroy Ball VFX
	//Common::fCGlitterEnd(sonic, Warp_E_VfxHandle, false); //Destroy Ball VFX
	printf("END HOMING ATTACK\n");
	originalCSonicStateHomingAttackEnd(This);
}

EXPORT void Init()
{
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(CSonicStateHomingAttackStart);
	INSTALL_HOOK(CSonicStateHomingAttackEnd);
}