#include "Mod.h"
#include "NPCAnim.h"
#include "SonicPlayer.h"

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
		switch (SelectBounceBall)
		{
		case (enum SelectJumpBallVFXType)BounceBallNoVFX:
			return;
			break;
		case (enum SelectJumpBallVFXType)BounceBallBAP:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_bounceattack", 1);  //Create Ball VFX
			break;
		case (enum SelectJumpBallVFXType)BounceBallForces:
			obj_SonicJumpBallWarsRenderable = boost::make_shared<JumpballWarsAnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallWarsRenderable);
			pPlayer->m_spCharacterModel->m_Enabled = false;
			break;
		case (enum SelectJumpBallVFXType)BounceBallBetaSWA:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_swaspinattack", 1);  //Create Ball VFX
			break;
		case (enum SelectJumpBallVFXType)BounceBallSWA:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sng_yh1_swaretailspinattack", 1);  //Create Ball VFX
			break;
		case (enum SelectJumpBallVFXType)BounceBallSA1:
			obj_SonicJumpBallSA1Renderable = boost::make_shared<JumpballSA1AnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallSA1Renderable);
			pPlayer->m_spCharacterModel->m_Enabled = false;
			break;
		case (enum SelectJumpBallVFXType)BounceBallLW:
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
		switch (SelectBounceBall)
		{
		case (enum SelectJumpBallVFXType)BounceBallNoVFX:
			return;
			break;
		case (enum SelectJumpBallVFXType)BounceBallBAP:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_bounceattack", 1);
			break;
		case (enum SelectJumpBallVFXType)BounceBallForces:

			break;
		case (enum SelectJumpBallVFXType)BounceBallBetaSWA:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_swaspinattack", 1);
			break;
		case (enum SelectJumpBallVFXType)BounceBallSWA:
			Common::fCGlitterCreate(sonic, BounceBallVfxHandle, middlematrixNode, "ef_ch_sps_yh1_swaretailspinattack", 1);
			break;
		case (enum SelectJumpBallVFXType)BounceBallSA1:
			obj_SonicJumpBallSA1Renderable = boost::make_shared<JumpballSA1AnimRenderable>();
			Sonic::CGameDocument::GetInstance()->AddGameObject(obj_SonicJumpBallSA1Renderable);
			break;
		case (enum SelectJumpBallVFXType)BounceBallLW:
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
				if (SelectBounceBall != 2)
					Common::fCGlitterCreate(sonic, BounceLandVfxHandle, groundmatrixNode, "ef_ch_sps_yh1_bounceland", 1);  //Create Super Stomp Land VFX
			}
			else
			{
				if (SelectBounceBall != 2)
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
		originalEnterClassicStompBounce(This);
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
		originalClassicStompBounce(This);
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
		originalExitClassicStompBounce(This);
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
	cat_Bounce_Gen->CreateParamTypeList((uint32_t*)&SelectBounceBall, "Ball VFX Type", "Choose what VFX gets used when bouncing",
		{
			{ "Bounce Attack+", (enum SelectJumpBallVFXType)BounceBallBAP},
			{ "Main Jumpball", (enum SelectJumpBallVFXType)BounceBallDefault},
			{ "Unleashed Ball", (enum SelectJumpBallVFXType)BounceBallSWA},
			{ "Beta Unleashed Ball", (enum SelectJumpBallVFXType)BounceBallBetaSWA},
			{ "Lost World Ball", (enum SelectJumpBallVFXType)BounceBallLW},
			{ "Forces Ball", (enum SelectJumpBallVFXType)BounceBallForces},
			{ "Adventure Ball", (enum SelectJumpBallVFXType)BounceBallSA1},
			{ "No VFX", (enum SelectJumpBallVFXType)BounceBallNoVFX},
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

//Install Sonic
void InstallSonicPlayer::applyPatches()
{
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(EnterStompBounce);
	INSTALL_HOOK(StompBounce);
	INSTALL_HOOK(ExitStompBounce);
	INSTALL_HOOK(EnterClassicStompBounce);
	INSTALL_HOOK(ClassicStompBounce);
	INSTALL_HOOK(ExitClassicStompBounce);
	INSTALL_HOOK(InitializeApplicationParams_BOUNCE);

	ReadINI("BAPBounce.ini");
}