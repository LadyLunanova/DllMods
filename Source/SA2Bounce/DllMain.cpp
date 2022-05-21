BOOL WINAPI DllMain(_In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

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
float Bounce01 = 16.0f; //First bounce power value
float Bounce02 = 18.0f; //Second bounce power value
float Bounce03 = 20.0f; //Third bounce power value
float Bounce04 = 22.0f; //Fourth bounce power value
float Bounce05 = 24.0f; //Final bounce power value
float BounceDrop = 22.0f; //Initial downward force
float BounceMulti = 1.0f; //Downward acceleration multiplier
//float BounceWaterMulti = 0.564f; //Underwater bounce height multiplier
bool NoBounceEnemy = false; //Bounce goes through enemies toggle
bool BounceTricking = false; //Bouncing makes sonic trick instead of curling into a ball
int GroundType = 0; //Ground action type seletor

HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto IsGrounded = This->GetContext()->m_Grounded;
	bool IsStomping = This->m_StateMachine.GetCurrentState()->GetStateName() == "Stomping";
	bool IsJumping = This->m_StateMachine.GetCurrentState()->GetStateName() == "Jump";
	bool IsFalling = This->m_StateMachine.GetCurrentState()->GetStateName() == "Fall";
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedB = input.IsTapped(Sonic::eKeyState_B);
	
	if (!IsStomping)
	{
		Common::fCGlitterEnd(sonic, BounceBallVfxHandle, true); //Destroy Ball VFX
		Common::fCGlitterEnd(sonic, BounceTrailVfxHandle, false); //Stop Creating Ball Trail VFX
	}

	if (IsGrounded && !IsStomping && !IsJumping && !IsFalling)
	{
		BounceCount = 0;
	}

	if (Sonic::Player::CSonicClassicContext::GetInstance() != nullptr)
	{
		if (PressedB && sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping] == 0 && !IsGrounded)
		{
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
	//printf("\n");

	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);
}

HOOK(void, __fastcall, EnterStompBounce, 0x01254CA0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	//originalEnterStompBounce(This);

	if (!NoBounceEnemy) //Check config setting
	{
		WRITE_MEMORY(0x16D6474, size_t, 0x11BCAB0); //Replace stomp's ProcessMessage with jump's (makes sonic bounce off of enemies)
	}
	else
	{
		WRITE_MEMORY(0x16D6474, size_t, 0x01254870); //Set Stomp's ProcessMessage to the original one (in order to check in real-time)
	}

	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto player = sonic->m_pPlayer;
	bool IsSuper = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
	//auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity; //Determine local axis velocity
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30); //Set up center matrix for VFX
	Common::SonicContextSetCollision(TypeSonicStomping, true, sonic); //Set sonic's collision type to stomping
	//sonic->ChangeAnimation("SpinAttack"); //Play ball animation
	sonic->ChangeAnimation("JumpBall"); //Play ball animation

	if (IsSuper)
	{
		Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_bounceattack", 1);  //Create Super Ball VFX
	}
	else 
	{
		Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_bounceattack", 1);  //Create Normal Ball VFX
	}

	Common::SonicContextSpawnLocusEffect(sonic); //Spawn blue trail
	//sonic->PlaySound(2002420, true); //Play bounce SFX

	//localVelocity.y() = -22; //Set Sonic's vertical local velocity
	sonic->m_Velocity.y() = -BounceDrop; //Set Sonic's vertical global velocity

	//sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Apply local axis velocity instead of global
}

HOOK(void, __fastcall, StompBounce, 0x012548C0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	//originalStompBounce(This);
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
	bool IsInWater = Flags->m_Flags[sonic->eStateFlag_OnWater];
	bool IsSuper = Flags->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
	auto IsGrounded = sonic->m_Grounded;
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30);
	void* groundmatrixNode = (void*)((uint32_t)sonic + 0x10);

	sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EnableHomingAttack] = 1; //Set allow homing attacking flag
	sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping] = 1; //Set stomp disable flag

	FUNCTION_PTR(bool, __thiscall, CommonActSwitch, 0x00E012A0, Sonic::Player::CPlayerSpeedContext * a1, hh::fnd::CStateMachineBase::CStateBase * a2);
	if (CommonActSwitch(sonic, This))
	{
		Common::fCGlitterEnd(sonic, BounceBallVfxHandle, true); //Destroy Ball VFX
		//Common::fCGlitterEnd(*PLAYER_CONTEXT, BounceTrailVfxHandle, false); //Stop Creating Ball Trail VFX
		return;
	}

	sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableStomping] = OrigStompCond; //Set the stomp flag back to what it originally was

	if (IsGrounded)  //Check if sonic is touching the ground and check config option
	{
		Common::fCGlitterEnd(sonic, BounceBallVfxHandle, true); //Destroy Ball VFX
		//Common::fCGlitterEnd(sonic, BounceTrailVfxHandle, false); //Stop Creating Ball Trail VFX

		if ((GroundType == 1 && !HoldB) || (GroundType == 2 && HoldB)) //Check ground type and if holding B
		{
			sonic->PlaySound(2002043, true); //Play stomp land SFX
			//sonic->PlaySound(2002420, true); //Play bounce SFX
			Common::fCGlitterCreate(sonic, BounceLandVfxHandle, groundmatrixNode, "ef_ch_sng_yh1_stomping2", 1);  //Create Stomp Land VFX
			//sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EnableHomingAttack] = 1; //Set allow homing attack flag

			if (localVelocity.z() > 15.0f) //Check forward speed
			{
				player->m_StateMachine.ChangeState("Sliding"); //Change state to slide state
			}
			else
			{
				localVelocity.z() > 0.0f;
				localVelocity.x() > 0.0f;
				player->m_StateMachine.ChangeState("StompingLand"); //Change state to stomping land state
			}
		}
		else
		{
			//sonic->PlaySound(2002043, true); //Play stomp land SFX
			sonic->PlaySound(2002420, true); //Play bounce SFX
			if (IsSuper)
			{
				Common::fCGlitterCreate(sonic, BounceLandVfxHandle, groundmatrixNode, "ef_ch_sps_yh1_bounceland", 1);  //Create Super Stomp Land VFX
			}
			else
			{
				Common::fCGlitterCreate(sonic, BounceLandVfxHandle, groundmatrixNode, "ef_ch_sng_yh1_bounceland", 1);  //Create Normal Stomp Land VFX
			}

			//player->m_StateMachine.ChangeState("Jump"); //Change state to jump state

			if (IsInWater)
			{
				player->m_StateMachine.ChangeState("HomingAttackAfter"); //Change state to homing attack tricking state

				switch (BounceCount) //Check how many times sonic has bounced
				{
				case 0:
					localVelocity.y() = (Bounce01/* * BounceWaterMulti*/); //Config First bounce height
					//localVelocity.y() = 16; //First bounce height
					break;
				case 1:
					localVelocity.y() = (Bounce02/* * BounceWaterMulti*/); //Config Second bounce height
					//localVelocity.y() = 18; //Second bounce height
					break;
				case 2:
					localVelocity.y() = (Bounce03/* * BounceWaterMulti*/); //Config Third bounce height
					//localVelocity.y() = 20; //Third bounce height
					break;
				case 3:
					localVelocity.y() = (Bounce04/* * BounceWaterMulti*/); //Config Fourth bounce height
					//localVelocity.y() = 22; //Fourth bounce height
					break;
				default:
					localVelocity.y() = (Bounce05/* * BounceWaterMulti*/); //Config Final bounce height
					//localVelocity.y() = 24; //Final bounce height
					break;
				}
			}
			else
			{
				if (BounceTricking) //Check for tricking option
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
					localVelocity.y() = Bounce01; //Config First bounce height
					//localVelocity.y() = 16; //First bounce height
					break;
				case 1:
					localVelocity.y() = Bounce02; //Config Second bounce height
					//localVelocity.y() = 18; //Second bounce height
					break;
				case 2:
					localVelocity.y() = Bounce03; //Config Third bounce height
					//localVelocity.y() = 20; //Third bounce height
					break;
				case 3:
					localVelocity.y() = Bounce04; //Config Fourth bounce height
					//localVelocity.y() = 22; //Fourth bounce height
					break;
				default:
					localVelocity.y() = Bounce05; //Config Final bounce height
					//localVelocity.y() = 24; //Final bounce height
					break;
				}
			}

			//player->m_StateMachine.ChangeState("Jump"); //Change state to jump state
			//player->m_StateMachine.ChangeState("Fall"); //Change state to jump state

			sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EnableHomingAttack] = 1; //Set allow homing attack flag
			BounceCount++; //Add to bounce int after bouncing
		}

	}

	localVelocity.y() += (-1.022 * BounceMulti); //Add downward acceleration

	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
}

HOOK(void, __fastcall, EnterClassicStompBounce, 0x012555D0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	//originalEnterStompBounce(This);

	if (!NoBounceEnemy) //Check config setting
	{
		WRITE_MEMORY(0x16D9268, size_t, 0x1114EB0); //Replace stomp's ProcessMessage with jump's (makes sonic bounce off of enemies)
	}
	else
	{
		WRITE_MEMORY(0x16D6474, size_t, 0x01254870); //Set Stomp's ProcessMessage to the original one (in order to check in real-time)
		return;
	}

	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto player = sonic->m_pPlayer;
	//auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity; //Determine local axis velocity
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30); //Set up center matrix for VFX
	Common::SonicContextSetCollision(TypeSonicStomping, true, sonic); //Set sonic's collision type to stomping
	//sonic->ChangeAnimation("SpinAttack"); //Play ball animation
	sonic->ChangeAnimation("JumpBall"); //Play ball animation

	Common::fCGlitterCreate(sonic, ClassicBounceBallVfxHandle, middlematrixNode, "ef_ch_snc_yh1_spindash1", 1);  //Create Ball VFX
	Common::SonicContextSpawnLocusEffect(sonic); //Spawn blue trail
	//sonic->PlaySound(2002420, true); //Play bounce SFX

	//localVelocity.y() = -22; //Set Sonic's vertical local velocity
	sonic->m_Velocity.y() = -BounceDrop; //Set Sonic's vertical global velocity

	//sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Apply local axis velocity instead of global
}

HOOK(void, __fastcall, ClassicStompBounce, 0x01255240, hh::fnd::CStateMachineBase::CStateBase* This)
{
	//originalStompBounce(This);
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto player = sonic->m_pPlayer;
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;
	auto Flags = sonic->m_pStateFlag;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	//auto inputState = Sonic::CInputState::GetInstance();
	//auto inputPtr = &inputState->m_PadStates[inputState->m_CurrentPadStateIndex];
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
		//Common::fCGlitterEnd(sonic, BounceBallVfxHandle, true); //Destroy Ball VFX

			//sonic->PlaySound(2002043, true); //Play stomp land SFX
			sonic->PlaySound(2001027, true); //Play bounce SFX
			//Common::fCGlitterCreate(sonic, BounceLandVfxHandle, groundmatrixNode, "ef_ch_sng_yh1_bounceland", 1);  //Create Stomp Land VFX

			//player->m_StateMachine.ChangeState("Jump"); //Change state to jump state

			if (IsInWater)
			{
				player->m_StateMachine.ChangeState("HomingAttackAfter"); //Change state to homing attack tricking state

				switch (BounceCount) //Check how many times sonic has bounced
				{
				case 0:
					localVelocity.y() = (Bounce01/* * BounceWaterMulti*/); //Config First bounce height
					//localVelocity.y() = 16; //First bounce height
					break;
				case 1:
					localVelocity.y() = (Bounce02/* * BounceWaterMulti*/); //Config Second bounce height
					//localVelocity.y() = 18; //Second bounce height
					break;
				case 2:
					localVelocity.y() = (Bounce03/* * BounceWaterMulti*/); //Config Third bounce height
					//localVelocity.y() = 20; //Third bounce height
					break;
				case 3:
					localVelocity.y() = (Bounce04/* * BounceWaterMulti*/); //Config Fourth bounce height
					//localVelocity.y() = 22; //Fourth bounce height
					break;
				default:
					localVelocity.y() = (Bounce05/* * BounceWaterMulti*/); //Config Final bounce height
					//localVelocity.y() = 24; //Final bounce height
					break;
				}
			}
			else
			{
				if (BounceTricking) //Check for tricking option
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
					localVelocity.y() = Bounce01; //Config First bounce height
					//localVelocity.y() = 16; //First bounce height
					break;
				case 1:
					localVelocity.y() = Bounce02; //Config Second bounce height
					//localVelocity.y() = 18; //Second bounce height
					break;
				case 2:
					localVelocity.y() = Bounce03; //Config Third bounce height
					//localVelocity.y() = 20; //Third bounce height
					break;
				case 3:
					localVelocity.y() = Bounce04; //Config Fourth bounce height
					//localVelocity.y() = 22; //Fourth bounce height
					break;
				default:
					localVelocity.y() = Bounce05; //Config Final bounce height
					//localVelocity.y() = 24; //Final bounce height
					break;
				}

			//player->m_StateMachine.ChangeState("Jump"); //Change state to jump state
			//player->m_StateMachine.ChangeState("Fall"); //Change state to jump state

			sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_EnableHomingAttack] = 1; //Set allow homing attack flag
			BounceCount++; //Add to bounce int after bouncing
		}

	}

	localVelocity.y() += (-1.022 * BounceMulti); //Add downward acceleration

	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
}

HOOK(void, __fastcall, ExitClassicStompBounce, 0x012554C0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto SparkEffectManager = sonic->m_pSparkEffectManager;
	FUNCTION_PTR(void*, __stdcall, StopLocusEffect, 0x00E8C940, void* a1, const hh::base::CSharedString & sharedString);

	Common::SonicContextSetCollision(TypeSonicStomping, false, sonic);
	Common::fCGlitterEnd(sonic, ClassicBounceBallVfxHandle, true); //Destroy Ball VFX
	if (SparkEffectManager)
	{
		StopLocusEffect(SparkEffectManager, "HomingAttack");
	}
}

HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicBounce</Archive>\n";
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

HOOK(void, __cdecl, InitializeApplicationParams_LUNA, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CParameterCategory* cat_Bounce = parameterGroup->CreateParameterCategory("SA2 Bounce", "Parameters for SA2 style bounce");

	//cat_Bounce->CreateParamInt(&someInt, "Integer");
	//cat_Bounce->CreateParamBool(&someBool, "Bool");
	//cat_Bounce->CreateParamFloat(&someFloat, "Float");
	 
	//cat_Bounce->CreateParamBool(&StompGround, "Stomps on ground on B hold");
	//cat_Bounce->CreateParamBool(&BounceGround, "Bounce on ground on B hold");
	cat_Bounce->CreateParamBool(&NoBounceEnemy, "Bounce goes through enemies");
	cat_Bounce->CreateParamTypeList((uint32_t*)&GroundType, "Ground Action Type", "The behavior for bouncing on the ground",
		{
			{ "Always Bounce", 0},
			{ "Hold B to Bounce", 1},
			{ "Hold B to Stomp/Slide", 2},
		});
	cat_Bounce->CreateParamBool(&BounceTricking, "Bouncing makes sonic trick instead of curling into a ball");
	cat_Bounce->CreateParamFloat(&Bounce01, "First bounce power value");
	cat_Bounce->CreateParamFloat(&Bounce02, "Second bounce power value");
	cat_Bounce->CreateParamFloat(&Bounce03, "Third bounce power value");
	cat_Bounce->CreateParamFloat(&Bounce04, "Fourth bounce power value");
	cat_Bounce->CreateParamFloat(&Bounce05, "Final bounce power value");
	cat_Bounce->CreateParamFloat(&BounceDrop, "Initial downward force");
	cat_Bounce->CreateParamFloat(&BounceMulti, "Downward acceleration multiplier");
	//cat_Bounce->CreateParamFloat(&BounceWaterMulti, "Underwater bounce height multiplier");

	parameterGroup->Flush();

	originalInitializeApplicationParams_LUNA(This);
}

EXPORT void Init()
{
	INIReader reader("SA2Bounce.ini");
	NoBounceEnemy = reader.GetBoolean("Option", "NoBounceEnemy", NoBounceEnemy);
	GroundType = reader.GetInteger("Option", "GroundType", GroundType);
	BounceTricking = reader.GetBoolean("Option", "BounceTricking", BounceTricking);
	Bounce01 = reader.GetFloat("Main", "Bounce01", Bounce01);
	Bounce02 = reader.GetFloat("Main", "Bounce02", Bounce02);
	Bounce03 = reader.GetFloat("Main", "Bounce03", Bounce03);
	Bounce04 = reader.GetFloat("Main", "Bounce04", Bounce04);
	Bounce05 = reader.GetFloat("Main", "Bounce05", Bounce05);
	BounceDrop = reader.GetFloat("Main", "BounceDrop", BounceDrop);
	BounceMulti = reader.GetFloat("Main", "BounceMulti", BounceMulti);
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
	INSTALL_HOOK(EnterStompBounce);
	INSTALL_HOOK(StompBounce);
	INSTALL_HOOK(EnterClassicStompBounce);
	INSTALL_HOOK(ClassicStompBounce);
	INSTALL_HOOK(ExitClassicStompBounce);
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(InitializeApplicationParams_LUNA);
}