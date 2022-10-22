#include "WildFireVFX.h"

SharedPtrTypeless WildfireVfxHandle;

void SpawnFireParticle(Sonic::Player::CPlayer* player)
{
	if (player == nullptr)
		return;
	auto ChestNode = player->m_spCharacterModel->GetNode("Spine1"); //Set up Chest bone matrix for VFX
	Eigen::Affine3f affine;
	affine = ChestNode->m_WorldMatrix;

	if (!WildfireVfxHandle)
		Common::fCGlitterCreate(player->m_spContext.get(), WildfireVfxHandle, &ChestNode, "ef_ch_sng_wildfire", 1);  //Create Fire VFX
}

void KillFireParticle(Sonic::Player::CPlayer* player)
{
	if (!WildfireVfxHandle)
		return;

	Common::fCGlitterEnd(player->m_spContext.get(), WildfireVfxHandle, true);
	WildfireVfxHandle = nullptr;
}

HOOK(void, __fastcall, MsgRestartStage, 0xE76810, Sonic::Player::CPlayer* This, void* Edx, hh::fnd::Message& message)
{
	KillFireParticle(This);
	return originalMsgRestartStage(This, Edx, message);
}

void __fastcall CSonicRemoveCallback(Sonic::Player::CPlayer* This, void* Edx, void* A1)
{
	KillFireParticle(This);
}

//Install WildFire
void InstallWildFireVFX::applyPatches()
{
	INSTALL_HOOK(MsgRestartStage);
	WRITE_MEMORY(0x16D4B4C, void*, CSonicRemoveCallback);
}