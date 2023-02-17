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
int   ModernBallType = 0;
bool  ModernLWBounce = false;
bool  ModernRangersBounce = false;
int   ModernRangersBounceTimer = 39;
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

		if (IsJumping && BounceCount >= 1 && HoldB && ModernRangersBounce)
		{
			if (ModernRangersBounceTimer >= 1)
			{
				ModernRangersBounceTimer--;
				if (ModernBounceHorCnl)
				{
					localVelocity.z() = 0.0f;
					localVelocity.x() = 0.0f;
					sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
				}
			}
			else
			{
				sonic->ChangeState("Stomping");
				ModernRangersBounceTimer = 39;
			}
		}
		else
			ModernRangersBounceTimer = 39;
		
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
	auto player = sonic->m_pPlayer;
	bool IsSuper = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30); //Set up center matrix for VFX
	Common::SonicContextSetCollision(TypeSonicStomping, true, sonic); //Set sonic's collision type to stomping
	sonic->ChangeAnimation("JumpBall"); //Play ball animation

	if (IsSuper)
	{
		//Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_bounceattack", 1);  //Create Super Ball VFX
		switch (ModernBallType)
		{
		case 0:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_bounceattack", 1);  //Create Super Ball VFX
			break;
		case 1:
		case 3:
		case 4:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_spinattack", 1);  //Create Super Ball VFX
			break;
		case 2:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_forcesspinattack", 1);  //Create Super Ball VFX
			break;
		}
	}
	else
	{
		//Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_bounceattack", 1);  //Create Normal Ball VFX
		switch (ModernBallType)
		{
		case 0:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_bounceattack", 1);  //Create Normal Ball VFX
			break;
		case 1:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_spinattack", 1);  //Create Normal Ball VFX
			break;
		case 2:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_forcesspinattack", 1);  //Create Normal Ball VFX
			break;
		case 3:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_swaspinattack", 1);  //Create Normal Ball VFX
			break;
		case 4:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_customspinattack", 1);  //Create Normal Ball VFX
			break;
		}
	}

	if (ModernBounceHorCnl)
	{
		sonic->m_Velocity.z() = 0.0;
		sonic->m_Velocity.x() = 0.0;
	}

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

		if (ModernRangersBounce)
		{
			switch (ModernSoundType)
			{
			case 0:
				sonic->PlaySound(2002420, true); //Play bounce SFX
				break;
			case 1:
				sonic->PlaySound(2002421, true); //Play bounce SFX
				break;
			case 2:
				sonic->PlaySound(2002422, true); //Play bounce SFX
				break;
			case 3:
				sonic->PlaySound(2002027, true); //Play bounce SFX
				break;
			case 4:
				sonic->PlaySound(2002043, true); //Play bounce SFX
				break;
			}

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

			switch (BounceCount) //Check how many times sonic has bounced
			{
			case 0:
				if (HoldB)
				{
					if (IsInWater || ModernBounceTricking || (ModernLWBounce && BounceCount >= 2)) //Check for tricking option
					{
						player->m_StateMachine.ChangeState("HomingAttackAfter"); //Change state to homing attack tricking state
					}
					else
					{
						player->m_StateMachine.ChangeState("Jump"); //Change state to jump state
					}
					localVelocity.y() = ModernBounce04; //Config First bounce height
				}
				else
				{
					localVelocity.z() = 0.0f;
					localVelocity.x() = 0.0f;
					player->m_StateMachine.ChangeState("StompingLand"); //Change state to stomping land state
				}
				break;
			case 1:
				if (HoldB)
				{
					if (IsInWater || ModernBounceTricking || (ModernLWBounce && BounceCount >= 2)) //Check for tricking option
					{
						player->m_StateMachine.ChangeState("HomingAttackAfter"); //Change state to homing attack tricking state
					}
					else
					{
						player->m_StateMachine.ChangeState("Jump"); //Change state to jump state
					}
					localVelocity.y() = ModernBounce05; //Config Second bounce height
				}
				else
				{
					localVelocity.z() = 0.0f;
					localVelocity.x() = 0.0f;
					player->m_StateMachine.ChangeState("StompingLand"); //Change state to stomping land state
				}
				break;
			default:
				localVelocity.z() = 0.0f;
				localVelocity.x() = 0.0f;
				player->m_StateMachine.ChangeState("StompingLand"); //Change state to stomping land state
				//Common::SonicContextSetCollision(TypeSonicStomping, true, sonic); //Set sonic's collision type to stomping
				break;
			}

			sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EnableHomingAttack] = 1; //Set allow homing attack flag
			BounceCount++; //Add to bounce int after bouncing
		}
		else
		{
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
					sonic->PlaySound(2002420, true); //Play bounce SFX
					break;
				case 1:
					sonic->PlaySound(2002421, true); //Play bounce SFX
					break;
				case 2:
					sonic->PlaySound(2002422, true); //Play bounce SFX
					break;
				case 3:
					sonic->PlaySound(2002027, true); //Play bounce SFX
					break;
				case 4:
					sonic->PlaySound(2002043, true); //Play bounce SFX
					break;
				}

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
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool HoldB = input.IsDown(Sonic::eKeyState_B);
	if ((ModernGroundActionType == 1 && !HoldB) || (ModernGroundActionType == 2 && HoldB) || (ModernGroundActionType == 3) || ModernBounceHorCnl)
	{
		localVelocity.z() = 0.0f;
		localVelocity.x() = 0.0f;
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

//Mod Management
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicVFX</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>SonicClassic</Name>\n";
		stream << "    <Archive>SonicVFX</Archive>\n";
		stream << "  </DefAppend>\n";

		str = stream.str();
	}

	const size_t newSize = size + str.size();
	const std::unique_ptr<char[]> buffer = std::make_unique<char[]>(newSize);
	memcpy(buffer.get(), data, size);

	char* insertionPos = strstr(buffer.get(), "<Include>");

	memmove(insertionPos + str.size(), insertionPos, size - (size_t)(insertionPos - buffer.get()));
	memcpy(insertionPos, str.c_str(), str.size());

	bool result;
	{
		result = originalParseArchiveTree(A1, buffer.get(), newSize, database);
	}

	return result;
}
HOOK(void, __cdecl, InitializeApplicationParams_BOUNCE, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroupGen;
	This->CreateParameterGroup(parameterGroupGen, "Luna's Mods", "Parameters for Lady Luna's code mods");
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
	cat_Bounce_Gen->CreateParamTypeList((uint32_t*)&ModernBallType, "Ball VFX Type", "Choose what VFX gets used when bouncing",
		{
			{ "Bounce Attack+", 0},
			{ "Main Jumpball", 1},
			{ "Forces Ball VFX", 2},
			{ "Beta Unleashed Ball VFX", 3},
			{ "Custom Beta Unleashed Ball VFX", 4},
			{ "No Ball VFX", 5},
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

	boost::shared_ptr<Sonic::CParameterGroup> parameterGroupCla;
	This->CreateParameterGroup(parameterGroupCla, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce_Cla = parameterGroupCla->CreateParameterCategory("Bounce Attack+ Classic", "Parameters for Classic Sonic in Bounce Attack+");

	cat_Bounce_Cla->CreateParamBool(&ClassicBounceEnable, "Enable bouncing for Modern Sonic");
	cat_Bounce_Cla->CreateParamBool(&ClassicBounceTrigger, "Enable bouncing when pressing one of the triggers mid-air");
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
	ModernLWBounce = reader.GetBoolean("Modern", "ModernLWBounce", ModernLWBounce);
	ModernRangersBounce = reader.GetBoolean("Modern", "ModernRangersBounce", ModernRangersBounce);
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
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(InitializeApplicationParams_BOUNCE);
}