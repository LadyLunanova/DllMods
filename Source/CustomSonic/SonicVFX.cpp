#include "SonicVFX.h"

SharedPtrTypeless WildfireVfxHandle;
SharedPtrTypeless JumpballVfxHandle;
bool WildFireActive = false;
void MsgWildFire(int Enabled)
{
	if (Enabled == 1)
		WildFireActive = true;
	else
		WildFireActive = false;
}
bool ParticleLightActive = false;
void ParticleLightGet(int Enabled)
{
	if (Enabled == 1)
		ParticleLightActive = true;
	else
		ParticleLightActive = false;
}
int JumpBallType = 0;
void MsgJumpBall(int BallType)
{
	JumpBallType = BallType;
}
bool JumpBlueTrailActive = false;
void JumpBlueTrailGet(int Enabled)
{
	if (Enabled == 1)
		JumpBlueTrailActive = true;
	else
		JumpBlueTrailActive = false;
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

	//if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
	//{
	//	auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

	//	const auto& up = playerContext->m_ModelUpDirection;
	//	const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

	//	if (stateName == "Sliding" || playerContext->StateFlag(eStateFlag_Boost))
	//		position += fwd * 0.5f + up * 0.25f;

	//	if (!superPLight)
	//		superPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
	//	else
	//		superPLight->SetPosition(position);
	//}
	//else
	//	superPLight = nullptr;
}
void SuperParticleLight(Sonic::Player::CPlayer* player)
{
	auto gameDocument = Sonic::CGameDocument::GetInstance();
	if (!gameDocument.get().get() || !gameDocument->m_pMember->m_spLightManager)
		return;

	auto playerContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!playerContext)
		return;

	if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
	{
		auto positionSS = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& upSS = playerContext->m_ModelUpDirection;
		const auto fwdSS = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		positionSS += fwdSS * 0.064f + upSS * 0.7f;

		if (!superPLight)
			superPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(positionSS, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 7.0f });
		else
			superPLight->SetPosition(positionSS);
	}
	else
		superPLight = nullptr;
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
	bool PressedLeft = input.IsTapped(Sonic::eKeyState_DpadLeft);
	bool PressedRight = input.IsTapped(Sonic::eKeyState_DpadRight);
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

	if (ParticleLightActive == true)
	{
		CustomParticleLight(0.0, 0.4, 2.0, 1.0, 4.2, This);
		SuperParticleLight(This);
	}
		
}

HOOK(void, __fastcall, CSonicStateJumpBallStartState, 0x011BCBE0, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalCSonicStateJumpBallStartState(This);
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	bool IsSuper = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_InvokeSuperSonic];
	void* middlematrixNode = (void*)((uint32_t)sonic + 0x30); //Set up center matrix for VFX

	if (JumpBlueTrailActive == true)
		SonicContextSpawnLocusEffect(sonic); //Spawn blue trail

	if (!IsSuper)
	{
		switch (JumpBallType)
		{
		case 1:
		case 2:
			return;
			break;
		case 3:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_bounceattack", 1);  //Create Ball VFX
			break;
		case 4:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_forcesspinattack", 1);  //Create Ball VFX
			break;
		case 5:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_swaspinattack", 1);  //Create Ball VFX
			break;
		case 6:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_swaretailspinattack", 1);  //Create Ball VFX
			break;
		case 7:
			printf("SPAWN SA1 JUMPBALL NOW\n");
			break;
		case 8:
			printf("SPAWN LW JUMPBALL NOW\n");
			break;
		default:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sng_yh1_spinattack", 1);  //Create Ball VFX
			break;
		}
	}
	else
	{
		switch (JumpBallType)
		{
		case 1:
		case 2:
			return;
			break;
		case 3:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sps_yh1_bounceattack", 1);  //Create Ball VFX
			break;
		case 4:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sps_yh1_forcesspinattack", 1);  //Create Ball VFX
			break;
		case 5:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sps_yh1_swaspinattack", 1);  //Create Ball VFX
			break;
		case 6:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sps_yh1_swaretailspinattack", 1);  //Create Ball VFX
			break;
		case 7:
			printf("SPAWN SUPER SA1 JUMPBALL NOW\n");
			break;
		case 8:
			printf("SPAWN SUPER LW JUMPBALL NOW\n");
			break;
		default:
			Common::fCGlitterCreate(sonic, JumpballVfxHandle, middlematrixNode, "ef_ch_sps_yh1_spinattack", 1);  //Create Ball VFX
			break;
		}
	}

}
HOOK(void, __fastcall, CSonicStateJumpBallEndState, 0x011BCB60, hh::fnd::CStateMachineBase::CStateBase* This)
{
	originalCSonicStateJumpBallEndState(This);
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	Common::fCGlitterEnd(sonic, JumpballVfxHandle, true); //Destroy Ball VFX

	auto SparkEffectManager = sonic->m_pSparkEffectManager;
	FUNCTION_PTR(void*, __stdcall, StopLocusEffect, 0x00E8C940, void* a1, const hh::base::CSharedString & sharedString);
	if (SparkEffectManager)
	{
		StopLocusEffect(SparkEffectManager, "HomingAttack");
	}
}

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationVFXParams, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce = parameterGroup->CreateParameterCategory("Customizable Sonic", "Parameters for customizable Sonic");

	cat_Bounce->CreateParamTypeList((uint32_t*)&JumpBallType, "Jumpball VFX", "Choose which jumpball",
		{
			{ "None", 0},
			{ "Default", 1},
			{ "Bounce Attack+", 2},
			{ "Forces", 3},
			{ "Beta Unleashed", 4},
			{ "Custom Beta Unleashed", 5},
			{ "Unleashed", 6},
			{ "Adventure", 7},
			{ "Lost World", 8},
		});
	cat_Bounce->CreateParamBool(&JumpBlueTrailActive, "Create a blue trail when Sonic jumps.");

	parameterGroup->Flush();

	originalInitializeApplicationVFXParams(This);
}

//Install WildFire
void InstallSonicVFX::applyPatches()
{
	INSTALL_HOOK(MsgRestartStage);
	WRITE_MEMORY(0x16D4B4C, void*, CSonicRemoveCallback);
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(CSonicStateJumpBallStartState);
	INSTALL_HOOK(CSonicStateJumpBallEndState);
	INSTALL_HOOK(InitializeApplicationVFXParams);
}