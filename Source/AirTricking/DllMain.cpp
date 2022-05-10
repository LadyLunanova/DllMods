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



bool GlobalReleaseBumper = 0;

// Original code by Skyth: https://github.com/blueskythlikesclouds
struct MsgGetAnimationInfo
{
    INSERT_PADDING(0x14);
    char* name;
    float frame;
};
inline FUNCTION_PTR(void, __thiscall, CSonicSpeedProcMsgGetAnimationInfo, 0xE6A370, void* This, void* pMessage);

HOOK(void, __fastcall, CPlayerSpeedUpdateParallel, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
    auto input = Sonic::CInputState::GetInstance()->GetPadState();
    auto inputState = Sonic::CInputState::GetInstance();
    auto inputPtr = &inputState->m_PadStates[inputState->m_CurrentPadStateIndex];
    bool IsTricking = This->m_StateMachine.GetCurrentState()->GetStateName() == "AdlibTrick";
    bool PressedBumper = input.IsTapped(Sonic::eKeyState_RightBumper) || input.IsTapped(Sonic::eKeyState_LeftBumper);
    bool ReleaseBumper = input.IsReleased(Sonic::eKeyState_RightBumper) || input.IsReleased(Sonic::eKeyState_LeftBumper);
    bool HoldBumper = input.IsDown(Sonic::eKeyState_RightBumper) || input.IsDown(Sonic::eKeyState_LeftBumper);
    bool PressedFace = input.IsTapped(Sonic::eKeyState_A) || input.IsTapped(Sonic::eKeyState_B) || input.IsTapped(Sonic::eKeyState_X) || input.IsTapped(Sonic::eKeyState_Y);
    auto Flags = sonic->m_pStateFlag;
    bool IsTrickingBlocked = 
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardJump" || 
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardJumpShort" || 
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardJumpSpring" || 
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardFall" || 
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardAirboost" || 
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardGrindJumpShort" || 
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardGrindJumpSide" || 
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardGetOn" || 
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardGetOff" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardAdlibTrick" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardGrind" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardNormalDamage" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardGrindLandJumpShort" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardLandJumpShort" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "BoardDrift" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "CPlayerSpeedStateBoardTrickJump" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "TransformRocket" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "TransformSp" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "TransformStandard" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "RocketEnd" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "RocketIdle" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "RocketLaunch" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "RocketOverHeat" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "HomingAttack" ||
        This->m_StateMachine.GetCurrentState()->GetStateName() == "Stomping";
    bool IsOutOfControl = Flags->m_Flags[sonic->eStateFlag_OutOfControl];
    auto GetAnim = sonic->GetCurrentAnimationName();
    bool IsTrickAnim = 
        GetAnim == "Trick_d0" ||        // 44 Frames
        GetAnim == "Trick_d0sv" ||      // 44 Frames
        GetAnim == "Trick_d1" ||        // 53 Frames
        GetAnim == "Trick_d1sv" ||      // 53 Frames
        GetAnim == "Trick_d2" ||        // 56 Frames
        GetAnim == "Trick_d2sv" ||      // 56 Frames
        GetAnim == "Trick_l0" ||        // 47 Frames
        GetAnim == "Trick_l0sv" ||      // 47 Frames
        GetAnim == "Trick_l1" ||        // 53 Frames
        GetAnim == "Trick_l1sv" ||      // 53 Frames
        GetAnim == "Trick_l2" ||        // 65 Frames
        GetAnim == "Trick_l2sv" ||      // 65 Frames
        GetAnim == "Trick_r0" ||        // 53 Frames
        GetAnim == "Trick_r0sv" ||      // 53 Frames
        GetAnim == "Trick_r1" ||        // 53 Frames
        GetAnim == "Trick_r1sv" ||      // 53 Frames
        GetAnim == "Trick_r2" ||        // 63 Frames
        GetAnim == "Trick_r2sv" ||      // 63 Frames
        GetAnim == "Trick_u0" ||        // 43 Frames
        GetAnim == "Trick_u0sv" ||      // 43 Frames
        GetAnim == "Trick_u1" ||        // 44 Frames
        GetAnim == "Trick_u1sv" ||      // 44 Frames
        GetAnim == "Trick_u2" ||        // 56 Frames
        GetAnim == "Trick_u2sv" ||      // 56 Frames
        GetAnim == "Trick_FinishB" ||   // 45 Frames
        GetAnim == "Trick_FinishF" ||   // 45 Frames
        GetAnim == "Trick_FinishSV" ||  // 45 Frames
        GetAnim == "Trick_Prepare";     //Pre-Tricking anim
    bool IsTrickD0 =
        GetAnim == "Trick_d0" ||
        GetAnim == "Trick_d0sv";
    bool IsTrickD1 =
        GetAnim == "Trick_d1" ||
        GetAnim == "Trick_d1sv";
    bool IsTrickD2 =
        GetAnim == "Trick_d2" ||
        GetAnim == "Trick_d2sv";
    bool IsTrickL0 =
        GetAnim == "Trick_l0" ||
        GetAnim == "Trick_l0sv";
    bool IsTrickL1 =
        GetAnim == "Trick_l1" ||
        GetAnim == "Trick_l1sv";
    bool IsTrickL2 =
        GetAnim == "Trick_l2" ||
        GetAnim == "Trick_l2sv";
    bool IsTrickR0 =
        GetAnim == "Trick_r0" ||
        GetAnim == "Trick_r0sv";
    bool IsTrickR1 =
        GetAnim == "Trick_r1" ||
        GetAnim == "Trick_r1sv";
    bool IsTrickR2 =
        GetAnim == "Trick_r2" ||
        GetAnim == "Trick_r2sv";
    bool IsTrickU0 =
        GetAnim == "Trick_u0" ||
        GetAnim == "Trick_u0sv";
    bool IsTrickU1 =
        GetAnim == "Trick_u1" ||
        GetAnim == "Trick_u1sv";
    bool IsTrickU2 =
        GetAnim == "Trick_u2" ||
        GetAnim == "Trick_u2sv";

    // Original code by Skyth: https://github.com/blueskythlikesclouds
    void* sonicContext = *(void**)0x1E5E2F0;
    MsgGetAnimationInfo msgGetAnimationInfo{};
    CSonicSpeedProcMsgGetAnimationInfo(*(void**)((uint8_t*)sonicContext + 0x110), &msgGetAnimationInfo);

    if (PressedBumper/* && !sonic->m_Grounded && !IsTricking && !IsTrickingBlocked*/)
    {
        if (!sonic->m_Grounded && !IsTricking && !IsTrickingBlocked)
        {
            inputPtr->LeftStickVertical = 0.0f;
            inputPtr->LeftStickHorizontal = 0.0f;
            This->m_StateMachine.ChangeState("AdlibTrick");
        }
        else if (IsTricking)
        {
            inputPtr->LeftStickVertical = 0.0f;
            inputPtr->LeftStickHorizontal = 0.0f;
        }
    }

    if (ReleaseBumper)
    {
        //if (!HoldBumper)
        //{
        //    if (IsTricking && !IsTrickAnim)
        //    {
        //        This->m_StateMachine.ChangeState("Fall");
        //        GlobalReleaseBumper = 0;
        //    }
        //    else if (IsTricking && IsTrickAnim)
        //    {
        //        GlobalReleaseBumper = 1;
        //    }
        //}
        if (IsTricking && !IsTrickAnim)
        {
            This->m_StateMachine.ChangeState("Fall");
            GlobalReleaseBumper = 0;
        }
        else if (IsTricking && IsTrickAnim && !HoldBumper)
        {
            GlobalReleaseBumper = 1;
        }
    }

    if ((
        ((IsTrickD0 || IsTrickU1) && msgGetAnimationInfo.frame > 42) ||
        ((IsTrickD1 || IsTrickL1 || IsTrickR0 || IsTrickR1) && msgGetAnimationInfo.frame > 51) ||
        ((IsTrickD2 || IsTrickU2) && msgGetAnimationInfo.frame > 54) ||
        (IsTrickL0 && msgGetAnimationInfo.frame > 45) ||
        (IsTrickL2 && msgGetAnimationInfo.frame > 63) ||
        (IsTrickR2 && msgGetAnimationInfo.frame > 61) ||
        (IsTrickU0 && msgGetAnimationInfo.frame > 41)
        ) && (GlobalReleaseBumper == 1))
    {
        This->m_StateMachine.ChangeState("Fall");
        GlobalReleaseBumper = 0;
    }

    if (!IsTricking)
    {
        GlobalReleaseBumper = 0;
    }

    if (PressedFace && IsTricking && !PressedBumper)
    {
        This->m_StateMachine.ChangeState("Fall");
        GlobalReleaseBumper = 0;
    }

    //if (PressedBumper && IsTricking)
    //{
    //    inputPtr->LeftStickVertical = 0.0f;
    //    inputPtr->LeftStickHorizontal = 0.0f;
    //}

    //printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
    //printf("\n");

    //printf(sonic->GetCurrentAnimationName().c_str());
    printf("%d", GlobalReleaseBumper);
    printf(" - ");
    printf(msgGetAnimationInfo.name);
    printf(" - ");
    printf("%f", msgGetAnimationInfo.frame);
    printf("\n");

	originalCPlayerSpeedUpdateParallel(This, _, updateInfo);
}

HOOK(void, __fastcall, EnterJumpSpring, 0x11BE9F0, hh::fnd::CStateMachineBase::CStateBase* This)
{
    originalEnterJumpSpring(This);
    auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
    auto player = sonic->m_pPlayer;
    player->m_StateMachine.ChangeState("AdlibTrick");
}

EXPORT void Init()
{
	INSTALL_HOOK(CPlayerSpeedUpdateParallel);
	INSTALL_HOOK(EnterJumpSpring);
}