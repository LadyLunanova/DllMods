#include "WildFireVFX.h"

SharedPtrTypeless WildfireVfxHandle;
bool ConfigParticleLights = false;
bool WildFireActive = false;
void WildFireEnable(int Enabled)
{
	if (Enabled == 1)
		WildFireActive = true;
	else
		WildFireActive = false;
}

//Dynamic Omnis
// Original code by Skyth: https://github.com/blueskythlikesclouds
hh::base::CRefPtr<Sonic::CLocalLight> genericPLight;
hh::base::CRefPtr<Sonic::CLocalLight> superPLight;
hh::base::CRefPtr<Sonic::CLocalLight> grindPLight;
hh::base::CRefPtr<Sonic::CLocalLight> wildfirePLight;
void CustomParticleLight(float ColorR, float ColorG, float ColorB, float ColorA, float LightRange, Sonic::Player::CPlayer* player)
{
	auto gameDocument = Sonic::CGameDocument::GetInstance();
	if (!gameDocument.get().get() || !gameDocument->m_pMember->m_spLightManager)
		return;

	auto playerContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!playerContext)
		return;

	const auto& stateName = playerContext->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	const auto& animName = playerContext->GetCurrentAnimationName();

	auto FootNode = player->m_spCharacterModel->GetNode("Toe_R"); //Set up Chest bone matrix for VFX
	Eigen::Affine3f affineFootNode;
	affineFootNode = FootNode->m_WorldMatrix;
	hh::math::CVector FootVector;
	FootVector = affineFootNode.translation();

	if ((animName == "SpinAttack" && stateName == "Jump") || stateName == "Stomping" || playerContext->StateFlag(eStateFlag_Boost))
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		//if (stateName == "Sliding" || playerContext->StateFlag(eStateFlag_Boost))
		//	position += fwd * 0.5f + up * 0.25f;

		if (!genericPLight)
			if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
				genericPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
			else
				genericPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { ColorR, ColorG, ColorB, ColorA }, { 0.0f, 0.0f, 0.0f, LightRange });
		else
			genericPLight->SetPosition(position);
	}
	else if (stateName == "SquatKick" || stateName == "Sliding")
	{
		if (!genericPLight)
			if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
				genericPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(FootVector, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
			else
				genericPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(FootVector, { ColorR, ColorG * 1.23f, ColorB * 1.5f, ColorA }, { 0.0f, 0.0f, 0.0f, 2.2f });
		else
			genericPLight->SetPosition(FootVector);
	}
	else
		genericPLight = nullptr;

	if (stateName == "Grind" || stateName == "GrindSquat" || stateName == "GrindLandJumpShort")
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		position += up * 0.06f;

		if (!grindPLight)
			grindPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 2.0f, 0.666f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.5f });
		else
			grindPLight->SetPosition(position);
	}
	else
		grindPLight = nullptr;

	if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		if (stateName == "Sliding" || playerContext->StateFlag(eStateFlag_Boost))
			position += fwd * 0.5f + up * 0.25f;

		if (!superPLight)
			superPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
		else
			superPLight->SetPosition(position);
	}
	else
		superPLight = nullptr;
}

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

HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);

	if (IsModernSonic)
	{
		if (WildFireActive == true)
		{
			SpawnFireParticle(This);
			//sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_JumpShortReleaseTime] = 256.0f;
		}
		if (WildFireActive == false)
		{
			KillFireParticle(This);
			//sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_JumpShortReleaseTime);
		}

		//if (SelectShoes == ShSUWallJmp)
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_JumpPower] = 22.0f;
		//else if (SelectShoes == ShSUStomp)
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_JumpPower] = 12.3f;
		//else
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_JumpPower);

		//if (SelectShoes == ShSUAirboost)
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostTime] = 1000.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_BoostInAirKeepTime] = 1000.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostGravityRate] = 0.5f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostLevitationTime] = 0.25f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostFirstDecrease] = 0.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AirBoostEnableChaosEnergy] = 0.0f;
		//}
		//else
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostTime);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_BoostInAirKeepTime);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostGravityRate);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostLevitationTime);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostFirstDecrease);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AirBoostEnableChaosEnergy);
		//}

		//if (SelectShoes == ShSA1LightSpd || SelectShoes == ShSoaps || SelectShoes == ShSoapsLightSpd || SelectShoes == ShSoapsRacing)
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationSpeedMax] = 5000.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationSpeedMaxShoesSliding] = 5000.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationForce0] = 100.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationForce1] = 100.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_RotationForce2] = 100.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_VertVelocityBallToFall] = -1000.0f;
		//}
		//else
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationSpeedMax);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationSpeedMaxShoesSliding);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationForce0);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationForce1);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_RotationForce2);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_VertVelocityBallToFall);
		//}

		//if (SelectShoes == Sh06GemDefault ||
		//	SelectShoes == Sh06GemRed ||
		//	SelectShoes == Sh06GemYellow ||
		//	SelectShoes == Sh06GemWhite ||
		//	SelectShoes == Sh06GemGreen ||
		//	SelectShoes == Sh06GemPurple ||
		//	SelectShoes == Sh06GemSky ||
		//	SelectShoes == Sh06GemBlue)
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity] = 30.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicParameter_MotionRunToDash] = 48.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicParameter_MotionDashToJet] = 84.0f;
		//	sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicParameter_MotionJetToBoost] = 100.0f;
		//	
		//}
		//else
		//{
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicParameter_MotionRunToDash);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicParameter_MotionDashToJet);
		//	sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicParameter_MotionJetToBoost);
		//}
	}

	//printf(sonic->GetCurrentAnimationName().c_str());
	//printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	//printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);

	if (ConfigParticleLights)
		CustomParticleLight(0.0, 0.4, 2.0, 1.0, 4.2, This);
}

//Install WildFire
void InstallWildFireVFX::applyPatches()
{
	INIReader reader("CustomSonic.ini");
	ConfigParticleLights = reader.GetInteger("Mod", "ConfigParticleLights", ConfigParticleLights);
	INSTALL_HOOK(MsgRestartStage);
	WRITE_MEMORY(0x16D4B4C, void*, CSonicRemoveCallback);
	INSTALL_HOOK(CPlayerSpeedUpdate);
}