
float FallTimer = 0.0f;

////Config Options////
bool  ConfWaterFlail = false;
int   ConfAnimType = 0;
float ConfFallTimer = 0.75f;

HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	bool IsFalling = This->m_StateMachine.GetCurrentState()->GetStateName() == "Fall";
	bool IsFlailing = 
		sonic->GetCurrentAnimationName() == "WallJumpB" ||
		sonic->GetCurrentAnimationName() == "WallJumpC";
	auto IsInWater = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_OnWater];
	auto IsClassicSonic = (Sonic::Player::CSonicClassicContext::GetInstance() != nullptr);

	if (((FallTimer <= 0.0 && IsFalling) || (IsInWater && (ConfWaterFlail == true) && IsFalling)) && !IsFlailing && !IsClassicSonic)
	{
		if (ConfAnimType == 1)
			sonic->ChangeAnimation("WallJumpB");
		else
			sonic->ChangeAnimation("WallJumpC");
	}

	if (FallTimer >= 0.0)
		FallTimer -= updateInfo.DeltaTime;

	if (!IsFalling)
		FallTimer = 0.0;

	//printf("Anim Name: %s\n", sonic->GetCurrentAnimationName().c_str()); //WallJumpC FallFast Fall FallLarge 01118F20
	//printf("State Name: %s\n", This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	//printf("Fall Timer: %f\n", FallTimer);

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}

HOOK(void, __fastcall, StartFall, 0x01118FB0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	FallTimer = ConfFallTimer;
	originalStartFall(This);
}
HOOK(void, __fastcall, EndFall, 0x01118FB0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	FallTimer = 0.0;
	originalEndFall(This);
}

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationParams, 0x00D65180, Sonic::CParameterFile* This)
{
	auto parameterGroup = This->CreateParameterGroup("Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce = parameterGroup->CreateParameterCategory("Fail Falling", "Parameters for Flail Falling");

	cat_Bounce->CreateParamBool(&ConfWaterFlail, "Always Flail Underwater");
	cat_Bounce->CreateParamFloat(&ConfFallTimer, "Flail Timer");
	cat_Bounce->CreateParamTypeList((uint32_t*)&ConfAnimType, "Anim Type", "Choose what homing trail",
		{
			{ "Flailing", 0},
			{ "Alt Anim", 1},
		});

	parameterGroup->Flush();

	originalInitializeApplicationParams(This);
}

EXPORT void Init()
{
	INIReader reader("Flailing.ini");
	ConfWaterFlail = reader.GetBoolean("Config", "ConfWaterFlail", ConfWaterFlail);
	ConfFallTimer = reader.GetFloat("Config", "ConfFallTimer", ConfFallTimer);
	ConfAnimType = reader.GetInteger("Config", "ConfAnimType", ConfAnimType);
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(StartFall);
	INSTALL_HOOK(EndFall);
	INSTALL_HOOK(InitializeApplicationParams);
}