
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

	if ((FallTimer <= 0.0 && IsFalling) || (IsInWater && (ConfWaterFlail == true) && IsFalling))
	{
		if (!IsFlailing)
		{
			//sonic->ChangeAnimation("WallJumpA");
			//sonic->ChangeAnimation("WallJumpB");
			//sonic->ChangeAnimation("WallJumpC");
			switch (ConfAnimType)
			{
			case 1:
				sonic->ChangeAnimation("WallJumpB");
				break;
			default:
				sonic->ChangeAnimation("WallJumpC");
				break;
			}
		}
	}

	if (FallTimer >= 0.0)
	{
		FallTimer -= updateInfo.DeltaTime;
	}

	if (!IsFalling)
	{
		FallTimer = 0.0;
	}

	//printf(sonic->GetCurrentAnimationName().c_str()); //WallJumpC FallFast Fall FallLarge 01118F20
	//printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	//printf(" - ");
	//printf("%f", FallTimer);
	//printf("\n");

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
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CParameterCategory* cat_Bounce = parameterGroup->CreateParameterCategory("Fail Falling", "Parameters for Flail Falling");

	//cat_Bounce->CreateParamInt(&someInt, "Integer");
	//cat_Bounce->CreateParamBool(&someBool, "Bool");
	//cat_Bounce->CreateParamFloat(&someFloat, "Float");

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