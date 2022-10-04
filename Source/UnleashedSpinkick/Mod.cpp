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
}

float KickTimer = 0.0f;
SharedPtrTypeless KickVfxHandle; //VFX handler

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
			KickTimer = 0.25f;
		}
	}
	else if (KickTimer > 0.0f)
	{
		KickTimer -= updateInfo.DeltaTime;
	}

	//printf("%f",KickTimer);
	//printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}

HOOK(void, __fastcall, SquatKickBegin, 0x012526D0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	void* groundmatrixNode = (void*)((uint32_t)sonic + 0x10); //Set up ground matrix for VFX
	Common::fCGlitterCreate(sonic, KickVfxHandle, groundmatrixNode, "sonic_sliding_kick", 1);  //Create particle effect "sonic_sliding_kick"
	sonic->PlaySound(2002033, true); //Play SFX
	originalSquatKickBegin(This);
}

HOOK(void, __fastcall, SquatKickEnd, 0x012527B0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	Common::fCGlitterEnd(sonic, KickVfxHandle, false); //Kill particle effect
	originalSquatKickEnd(This);
}

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

EXPORT void Init()
{
	INSTALL_HOOK(SquatKickBegin);
	INSTALL_HOOK(SquatKickEnd);
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(ParseArchiveTree);
}