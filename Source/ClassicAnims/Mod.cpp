#include "AnimationSetPatcher.h"

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

//Classic State Stuff
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();

	//printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	//printf(sonic->GetCurrentAnimationName().c_str());
	//printf("%f,%f,%f",sonic->m_Velocity.x(),sonic->m_Velocity.y(),sonic->m_Velocity.z());
	//printf("%d",sonic->m_VelocityDirty);
	//printf("%f",sonic->m_ChaosEnergy);
	//printf("%d",sonic->m_Is2DMode);
	//printf("%d",BounceCount);
	//printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}
HOOK(void, __fastcall, EnterClassicFalling, 0x01115A60, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalEnterClassicFalling(This);

	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto player = sonic->m_pPlayer;

	sonic->ChangeAnimation("ExtraFall"); //Play animation
}
HOOK(void, __fastcall, EnterClassicSpin, 0x011BB0E0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalEnterClassicSpin(This);
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	Common::SonicContextSpawnLocusEffect(sonic); //Spawn blue trail
}
HOOK(void, __fastcall, ExitClassicSpin, 0x011BAFD0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto SparkEffectManager = sonic->m_pSparkEffectManager;
	FUNCTION_PTR(void*, __stdcall, StopLocusEffect, 0x00E8C940, void* a1, const hh::base::CSharedString & sharedString);
	if (SparkEffectManager)
	{
		StopLocusEffect(SparkEffectManager, "HomingAttack");
	}
	originalExitClassicSpin(This);
}
HOOK(void, __fastcall, EnterClassicSliding, 0x01251760, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalEnterClassicSliding(This);
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	Common::SonicContextSpawnLocusEffect(sonic); //Spawn blue trail
}
HOOK(void, __fastcall, ExitClassicSliding, 0x01251690, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto SparkEffectManager = sonic->m_pSparkEffectManager;
	FUNCTION_PTR(void*, __stdcall, StopLocusEffect, 0x00E8C940, void* a1, const hh::base::CSharedString & sharedString);
	if (SparkEffectManager)
	{
		StopLocusEffect(SparkEffectManager, "HomingAttack");
	}
	originalExitClassicSliding(This);
}

//Handle Archives
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>SonicClassic</Name>\n";
		stream << "    <Archive>SonicClassicExtra</Archive>\n";
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
	//AnimationSetPatcher::applyPatches();
	//INSTALL_HOOK(CPlayerSpeedUpdate);
	//INSTALL_HOOK(EnterClassicFalling);
	INSTALL_HOOK(EnterClassicSpin);
	INSTALL_HOOK(ExitClassicSpin);
	INSTALL_HOOK(EnterClassicSliding);
	INSTALL_HOOK(ExitClassicSliding);
	INSTALL_HOOK(ParseArchiveTree);
	WRITE_MEMORY(0x01282860, char*, "sc_jump_d_s_loop")
	WRITE_MEMORY(0x012828C7, char*, "sc_jump_d_loop")
	//WRITE_NOP(0x01115C3B, 2)
	//WRITE_NOP(0x01115C3D, 3)
	//WRITE_NOP(0x01115C40, 4)
	//WRITE_NOP(0x01115C44, 1)
	//WRITE_NOP(0x01115C45, 2)
	//WRITE_NOP(0x01115C47, 2)
}