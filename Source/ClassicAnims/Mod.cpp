#include "AnimationSetPatcher.h"

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
	AnimationSetPatcher::applyPatches();
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(EnterClassicFalling);
	INSTALL_HOOK(ParseArchiveTree);

	WRITE_NOP(0x01115C3B, 2)
	WRITE_NOP(0x01115C3D, 3)
	WRITE_NOP(0x01115C40, 4)
	WRITE_NOP(0x01115C44, 1)
	WRITE_NOP(0x01115C45, 2)
	WRITE_NOP(0x01115C47, 2)
}