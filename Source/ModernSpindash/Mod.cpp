//---------------------------------------------------
// Globals
//---------------------------------------------------
float SpindashTimer = 0.0f;
bool SpindashCharging = false;
bool SpindashRolling = false;
// Macros by Ahremic
// Based on: https://stackoverflow.com/a/1045237
#define SET_BUTTON(state, buttonIndex)    *(uint32_t*)&state |=  ((uint32_t)buttonIndex)
#define UNSET_BUTTON(state, buttonIndex)  *(uint32_t*)&state &= ~((uint32_t)buttonIndex)
#define TOGGLE_BUTTON(state, buttonIndex) *(uint32_t*)&state ^=  ((uint32_t)buttonIndex)
//---------------------------------------------------
// Player Update State
//---------------------------------------------------
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	bool IsSquatting = This->m_StateMachine.GetCurrentState()->GetStateName() == "Squat";
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool BtnBHolding = input.IsDown(Sonic::eKeyState_B);

	if (!IsSquatting)
	{
		sonic->m_ChaosEnergy = 0;
		SpindashTimer = 0.0;
	}
	//if (This->m_StateMachine.GetCurrentState()->GetStateName() == "Squat")
	//{
	//	Sonic::SPadState* padState = const_cast<Sonic::SPadState*>(&Sonic::CInputState::GetInstance()->GetPadState());
	//	UNSET_BUTTON(padState->TappedState, Sonic::eKeyState_B);
	//	UNSET_BUTTON(padState->DownState, Sonic::eKeyState_B);
	//}
	

	printf("%f", SpindashTimer);
	printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}

//---------------------------------------------------
// Squat Begin State
//---------------------------------------------------
HOOK(void, __fastcall, SquatBeginState, 0x01230A30, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	

	sonic->ChangeAnimation("JumpBall"); //Play ball animation
	SpindashCharging = true;
	SpindashRolling = false;
	
	//originalSquatBeginState(This);
}

//---------------------------------------------------
// Squat Update State
//---------------------------------------------------
HOOK(void, __fastcall, SquatUpdateState, 0x01230B60, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto player = sonic->m_pPlayer;
	auto IsGrounded = sonic->m_Grounded;
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity; //Determine local axis velocity
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	auto inputState = Sonic::CInputState::GetInstance();
	auto inputPtr = &inputState->m_PadStates[inputState->m_CurrentPadStateIndex];
	bool BtnBHolding = input.IsDown(Sonic::eKeyState_B);
	bool BtnBRelease = input.IsReleased(Sonic::eKeyState_B);
	bool BtnBPressed = input.IsTapped(Sonic::eKeyState_B);

	if (BtnBHolding && (SpindashRolling == false))
	{
		if (localVelocity.norm() <= 1)
		{
			localVelocity.z() = 0.0;
			localVelocity.x() = 0.0;
		}
		else
		{
			localVelocity.z() *= 0.25;
			localVelocity.x() *= 0.25;
		}
		
		SpindashTimer++;
	}

	if (BtnBRelease && (SpindashCharging == true))
	{
		SpindashRolling = true;
		SpindashCharging = false;
		if (SpindashTimer < 100)
		{
			localVelocity.z() += SpindashTimer;
		}
		else
		{
			localVelocity.z() += 100;
		}
		SpindashTimer = 0;
		//player->m_StateMachine.ChangeState("Walk");
	}

	if (BtnBPressed && (SpindashRolling == true))
	{
		player->m_StateMachine.ChangeState("Walk");
		Sonic::SPadState* padState = const_cast<Sonic::SPadState*>(&Sonic::CInputState::GetInstance()->GetPadState());
		UNSET_BUTTON(padState->TappedState, Sonic::eKeyState_B);
		UNSET_BUTTON(padState->DownState, Sonic::eKeyState_B);
	}

	if (SpindashRolling == true)
	{
		FUNCTION_PTR(bool, __thiscall, CommonActSwitch, 0x00E012A0, Sonic::Player::CPlayerSpeedContext * a1, hh::fnd::CStateMachineBase::CStateBase * a2);
		if (CommonActSwitch(sonic, This))
		{
			return;
		}
	}

	if (!IsGrounded)
	{
		SpindashRolling = false;
		SpindashCharging = false;
		player->m_StateMachine.ChangeState("Jump");
	}

	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Apply local axis velocity instead of global
	//originalSquatUpdateState(This);
}



//---------------------------------------------------
// Sliding Begin State
//---------------------------------------------------
HOOK(void, __fastcall, SlideBeginState, 0x011D7110, hh::fnd::CStateMachineBase::CStateBase* This)
{
	//auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	//auto player = sonic->m_pPlayer;
	//player->m_StateMachine.ChangeState("Squat");
	originalSlideBeginState(This);
}

//---------------------------------------------------
// Sliding Update State
//---------------------------------------------------
HOOK(void, __fastcall, SlideUpdateState, 0x011D69A0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalSlideUpdateState(This);
}



//---------------------------------------------------
// Export
//---------------------------------------------------
EXPORT void Init()
{
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(SquatBeginState);
	INSTALL_HOOK(SquatUpdateState);
	INSTALL_HOOK(SlideBeginState);
	INSTALL_HOOK(SlideUpdateState);
}