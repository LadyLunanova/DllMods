bool CustomTimerEnable = false;
float CustomTimer = 0.25f;
int SoundType = 0;
float KickTimer = 0.0f;

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
SharedPtrTypeless KickVfxHandle; //VFX handler

//State Stuff
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto IsGrounded = This->GetContext()->m_Grounded;
	bool IsAllowKick = 
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Stand" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Walk" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Sliding" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "SlidingEnd" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "StompingLand" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "SquatCharge" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Squat";
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedB = input.IsTapped(Sonic::eKeyState_B);
	auto IsOutOfControl = This->GetContext()->m_pStateFlag->m_Flags[sonic->eStateFlag_OutOfControl];

	if (PressedB && !IsOutOfControl && IsGrounded && IsAllowKick)
	{
		if (KickTimer > 0.0f)
		{
			sonic->ChangeState("SquatKick");
			KickTimer = 0.0f;
		}
		else
		{
			if (CustomTimerEnable)
				KickTimer = CustomTimer;
			else
				KickTimer = 0.25f;
		}
	}
	else if (KickTimer > 0.0f)
	{
		KickTimer -= updateInfo.DeltaTime;
	}

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}
HOOK(void, __fastcall, SquatKickBegin, 0x012526D0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	void* groundmatrixNode = (void*)((uint32_t)sonic + 0x10); //Set up ground matrix for VFX
	Common::fCGlitterCreate(sonic, KickVfxHandle, groundmatrixNode, "sonic_sliding_kick", 1);  //Create particle effect "sonic_sliding_kick"
	//sonic->PlaySound(2002033, true); //Play SFX
	switch (SoundType)
	{
	case 1:
		sonic->PlaySound(9000084, false); //Play SFX
		break;
	case 2:
		sonic->PlaySound(9000085, false); //Play SFX
		break;
	default:
		sonic->PlaySound(9000083, false); //Play SFX
		break;
	}
	originalSquatKickBegin(This);
}
HOOK(void, __fastcall, SquatKickEnd, 0x012527B0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	Common::fCGlitterEnd(sonic, KickVfxHandle, false); //Kill particle effect
	originalSquatKickEnd(This);
}

//Archive Stuff
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicSquatKick</Archive>\n";
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

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationParams, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CParameterCategory* cat_Bounce = parameterGroup->CreateParameterCategory("Unleashed Foot Sweep", "Parameters for the Unleashed Foot Sweep");

	//cat_Bounce->CreateParamInt(&someInt, "Integer");
	//cat_Bounce->CreateParamBool(&someBool, "Bool");
	//cat_Bounce->CreateParamFloat(&someFloat, "Float");

	cat_Bounce->CreateParamBool(&CustomTimerEnable, "Enable Custom Timer");
	cat_Bounce->CreateParamFloat(&CustomTimer, "Custom Timer");
	cat_Bounce->CreateParamTypeList((uint32_t*)&SoundType, "Sound Type", "Choose what homing trail",
		{
			{ "Unleashed Foot Sweep", 0},
			{ "06 Spinkick", 1},
			{ "SA2 Summersault", 2},
		});

	parameterGroup->Flush();

	originalInitializeApplicationParams(This);
}

EXPORT void Init()
{
	INIReader reader("UnleashedSpinkick.ini");
	CustomTimerEnable = reader.GetBoolean("Config", "CustomTimerEnable", CustomTimerEnable);
	CustomTimer = reader.GetFloat("Config", "CustomTimer", CustomTimer);
	SoundType = reader.GetInteger("Config", "SoundType", SoundType);
	INSTALL_HOOK(SquatKickBegin);
	INSTALL_HOOK(SquatKickEnd);
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(InitializeApplicationParams);
}