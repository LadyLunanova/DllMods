#include <Common.h>
#include <iostream>
#include <thread>
//HMODULE hm;

//---------------------------------------------------
// Globals
//---------------------------------------------------

float SonicScale = 1.0f;
float SonicTrailSpeed = 33.333f;
float ChaosEnergyTrickJumpDrain = 33.333f;
float VelocityTrickJumpForce = 19.0;
float VelocityJumpQuickStepForce = 25.0;
int JumpQuickStepSpeedCancelTimer = 6;
int JumpQuickStepSpeedCancelTime = 6;
int JumpQuickStepControlTimer = 0;
int JumpQuickStepControlTime = 10;
int SonicRedTrailType = 0;
int SonicWhiteTrailType = 0;
int SonicBlueTrailType = 4;
bool SonicNoBallJump = false;
bool hasJumpQuickStepped = false;
bool hasJumpQuickSteppedSpeedCancel = false;


// Original code by Brianuuu: https://github.com/brianuuu
// Original code by Skyth: https://github.com/blueskythlikesclouds
//namespace Hedgehog::Mirage
//{
//	class CMatrixNodeSingleElementNode
//	{
//	public:
//		CMatrixNode m_node;
//		Eigen::Matrix4f local;
//		Eigen::Matrix4f world;
//	};
//}

//---------------------------------------------------
// Particle Effect Handlers
//---------------------------------------------------

SharedPtrTypeless BoostJump_VfxHandle; //VFX handler
SharedPtrTypeless HandR_VfxHandle; //VFX handler
SharedPtrTypeless HandL_VfxHandle; //VFX handler
SharedPtrTypeless FootR_VfxHandle; //VFX handler
SharedPtrTypeless FootL_VfxHandle; //VFX handler
SharedPtrTypeless HairUR_VfxHandle; //VFX handler
SharedPtrTypeless HairUC_VfxHandle; //VFX handler
SharedPtrTypeless HairUL_VfxHandle; //VFX handler
SharedPtrTypeless HairBR_VfxHandle; //VFX handler
SharedPtrTypeless HairBC_VfxHandle; //VFX handler
SharedPtrTypeless HairBL_VfxHandle; //VFX handler

void sonicNodeGlitterCreate(Sonic::Player::CPlayerSpeed* This, SharedPtrTypeless& VFXHandle, Hedgehog::Base::CSharedString boneNode, Hedgehog::Base::CSharedString GTEFile)
{
	auto sonic = This->GetContext();
	auto BoneNodeMatrix = This->m_spCharacterModel->GetNode(boneNode); //Set up bone matrix for VFX
	Eigen::Affine3f affine;
	affine = BoneNodeMatrix->m_WorldMatrix;

	//Common::fCGlitterCreate(sonic, VFXHandle, &BoneNodeMatrix, GTEFile, 1);
	if (VFXHandle == nullptr)
	{
		Common::fCGlitterCreate(sonic, VFXHandle, &BoneNodeMatrix, GTEFile, 1);  //Create VFX //"ef_ch_sng_yh1_footlocus"
	}
}
void sonicRootGlitterCreate(Sonic::Player::CPlayerSpeed* This, SharedPtrTypeless& VFXHandle, Hedgehog::Base::CSharedString boneNode, Hedgehog::Base::CSharedString GTEFile)
{
	auto sonic = This->GetContext();
	auto BoneNodeMatrix = This->m_spCharacterModel->GetNode(boneNode); //Set up bone matrix for VFX
	Eigen::Affine3f affine;
	affine = BoneNodeMatrix->m_WorldMatrix;

	//Common::fCGlitterCreate(sonic, VFXHandle, &BoneNodeMatrix, GTEFile, 1);
	if (VFXHandle == nullptr)
	{
		Common::fCGlitterCreate(sonic, VFXHandle, &BoneNodeMatrix, GTEFile, 1);  //Create VFX //"ef_ch_sng_yh1_footlocus"
	}
}
void sonicNodeGlitterEnd(Sonic::Player::CPlayerSpeed* This, SharedPtrTypeless& VFXHandle, bool Kill)
{
	auto sonic = This->GetContext();
	Common::fCGlitterEnd(sonic, VFXHandle, Kill); //Destroy VFX
	VFXHandle = nullptr;
}


void sonicCreateRedTrail(Sonic::Player::CPlayerSpeed* This)
{
	sonicNodeGlitterCreate(This, FootR_VfxHandle, "Toe_R", "ef_ch_sng_yh1_footlocus"); //"ef_ch_sng_yh1_footlocus"
	sonicNodeGlitterCreate(This, FootL_VfxHandle, "Toe_L", "ef_ch_sng_yh1_footlocus"); //"ef_ch_sng_yh1_footlocus"
}
void sonicEndRedTrail(Sonic::Player::CPlayerSpeed* This)
{
	sonicNodeGlitterEnd(This, FootR_VfxHandle, false);
	sonicNodeGlitterEnd(This, FootL_VfxHandle, false);
}

void sonicCreateWhiteTrail(Sonic::Player::CPlayerSpeed* This)
{
	sonicNodeGlitterCreate(This, HandR_VfxHandle, "Middle1_R", "ef_ch_sng_yh1_handlocus"); //"ef_ch_sng_yh1_handlocus"
	sonicNodeGlitterCreate(This, HandL_VfxHandle, "Middle1_L", "ef_ch_sng_yh1_handlocus"); //"ef_ch_sng_yh1_handlocus"
}
void sonicEndWhiteTrail(Sonic::Player::CPlayerSpeed* This)
{
	sonicNodeGlitterEnd(This, HandR_VfxHandle, false);
	sonicNodeGlitterEnd(This, HandL_VfxHandle, false);
}

void sonicCreateBlueTrail(Sonic::Player::CPlayerSpeed* This)
{
	sonicNodeGlitterCreate(This, HairUR_VfxHandle, "Needle1_U_R", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	sonicNodeGlitterCreate(This, HairUC_VfxHandle, "Needle1_U_C", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	sonicNodeGlitterCreate(This, HairUL_VfxHandle, "Needle1_U_L", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	sonicNodeGlitterCreate(This, HairBR_VfxHandle, "Needle1_B_R", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	sonicNodeGlitterCreate(This, HairBC_VfxHandle, "Needle1_B_C", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	sonicNodeGlitterCreate(This, HairBL_VfxHandle, "Needle1_B_L", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
}
void sonicEndBlueTrail(Sonic::Player::CPlayerSpeed* This)
{
	sonicNodeGlitterEnd(This, HairUR_VfxHandle, true);
	sonicNodeGlitterEnd(This, HairUC_VfxHandle, true);
	sonicNodeGlitterEnd(This, HairUL_VfxHandle, true);
	sonicNodeGlitterEnd(This, HairBR_VfxHandle, true);
	sonicNodeGlitterEnd(This, HairBC_VfxHandle, true);
	sonicNodeGlitterEnd(This, HairBL_VfxHandle, true);
}

//---------------------------------------------------
// Main Func Shit
//---------------------------------------------------

HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	//---------------------------------------------------
	// Locals/Setup
	//---------------------------------------------------

	auto sonic = This->GetContext();
	auto IsGrounded = This->GetContext()->m_Grounded;
	auto IsBoosting = sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_Boost];
	bool sonicStillState = 
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Goal" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "GoalAir" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Stand" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Squat" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "SquatCharge" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "StompingLand" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "StartCrouching";
	bool sonicActionState =
		This->m_StateMachine.GetCurrentState()->GetStateName() == "AdlibTrick" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "AirBoost" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "DivingDive" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Drift" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "LightSpeedDash" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "HomingAttack" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "HomingAttackAfter" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "RunQuickStep" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Sliding" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "SlidingEnd" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "StompingLand" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Stomping" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "SquatKick";
	bool sonicStateJump = 
		This->m_StateMachine.GetCurrentState()->GetStateName() == "HomingAttackAfter" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "HomingAttackAfter" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Fall" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "WallJump" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "JumpHurdle" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "JumpShort" ||
		This->m_StateMachine.GetCurrentState()->GetStateName() == "Jump";
	
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	bool PressedRB = input.IsTapped(Sonic::eKeyState_RightBumper);
	bool PressedLB = input.IsTapped(Sonic::eKeyState_LeftBumper);
	bool HoldRT = input.IsDown(Sonic::eKeyState_RightTrigger);
	bool HoldRight = input.IsDown(Sonic::eKeyState_DpadRight);
	bool HoldLeft = input.IsDown(Sonic::eKeyState_DpadLeft);
	bool HoldLT = input.IsDown(Sonic::eKeyState_LeftTrigger);
	bool HoldUp = input.IsDown(Sonic::eKeyState_DpadUp);
	bool HoldDown = input.IsDown(Sonic::eKeyState_DpadDown);
	bool PressedRight = input.IsTapped(Sonic::eKeyState_DpadRight);
	bool PressedLeft = input.IsTapped(Sonic::eKeyState_DpadLeft);
	bool PressedUp = input.IsTapped(Sonic::eKeyState_DpadUp);
	bool PressedDown = input.IsTapped(Sonic::eKeyState_DpadDown);
	bool PressedLStick = input.IsTapped(Sonic::eKeyState_LeftStick);

	const auto GetAnimInfo = boost::make_shared<Sonic::Message::MsgGetAnimationInfo>();
	sonic->m_pPlayer->SendMessageImm(sonic->m_pPlayer->m_ActorID, GetAnimInfo);
	auto GetAnim = sonic->GetCurrentAnimationName();
	bool IsQuickStepAnim =
		GetAnim == "RunQuickStepL" ||
		GetAnim == "RunQuickStepR"; // 9 Frames

	//FUNCTION_PTR(void, __cdecl, ButtplugSetVibration, PROC_ADDRESS("LunarSharpPlayground.dll", "SetVibration"), double Strength);
	

	//---------------------------------------------------
	// Main Run Code
	//---------------------------------------------------

	//ButtplugSetVibration(1.0f);

	switch (SonicRedTrailType)
	{
	case 1:
		if (localVelocity.norm() >= 0.001 || sonicStillState || sonicActionState)
			sonicCreateRedTrail(This);
		else
			sonicEndRedTrail(This);
		break;
	case 2:
		if (localVelocity.norm() >= SonicTrailSpeed)
			sonicCreateRedTrail(This);
		else
			sonicEndRedTrail(This);
		break;
	case 3:
		if (sonicActionState)
			sonicCreateRedTrail(This);
		else
			sonicEndRedTrail(This);
		break;
	case 4:
		if (IsBoosting)
			sonicCreateRedTrail(This);
		else
			sonicEndRedTrail(This);
		break;
	case 5:
		if (sonicActionState || IsBoosting)
			sonicCreateRedTrail(This);
		else
			sonicEndRedTrail(This);
		break;
	}
	switch (SonicWhiteTrailType)
	{
	case 1:
		if (localVelocity.norm() >= 0.001 || sonicStillState || sonicActionState)
			sonicCreateWhiteTrail(This);
		else
			sonicEndWhiteTrail(This);
		break;
	case 2:
		if (localVelocity.norm() >= SonicTrailSpeed)
			sonicCreateWhiteTrail(This);
		else
			sonicEndWhiteTrail(This);
		break;
	case 3:
		if (sonicActionState)
			sonicCreateWhiteTrail(This);
		else
			sonicEndWhiteTrail(This);
		break;
	case 4:
		if (IsBoosting)
			sonicCreateWhiteTrail(This);
		else
			sonicEndWhiteTrail(This);
		break;
	case 5:
		if (sonicActionState || IsBoosting)
			sonicCreateWhiteTrail(This);
		else
			sonicEndWhiteTrail(This);
		break;
	}
	switch (SonicBlueTrailType)
	{
	case 1:
		if (localVelocity.norm() >= 0.001 || sonicStillState || sonicActionState)
			sonicCreateBlueTrail(This);
		else
			sonicEndBlueTrail(This);
		break;
	case 2:
		if (localVelocity.norm() >= SonicTrailSpeed)
			sonicCreateBlueTrail(This);
		else
			sonicEndBlueTrail(This);
		break;
	case 3:
		if (sonicActionState)
			sonicCreateBlueTrail(This);
		else
			sonicEndBlueTrail(This);
		break;
	case 4:
		if (IsBoosting)
			sonicCreateBlueTrail(This);
		else
			sonicEndBlueTrail(This);
		break;
	case 5:
		if (sonicActionState || IsBoosting)
			sonicCreateBlueTrail(This);
		else
			sonicEndBlueTrail(This);
		break;
	}

	//if (IsBoosting && (localVelocity.norm() >= 15.0) || sonicActionState)
	//{
	//	sonicNodeGlitterCreate(This, HairUR_VfxHandle, "Needle1_U_R", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	//	sonicNodeGlitterCreate(This, HairUC_VfxHandle, "Needle1_U_C", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	//	sonicNodeGlitterCreate(This, HairUL_VfxHandle, "Needle1_U_L", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	//	sonicNodeGlitterCreate(This, HairBR_VfxHandle, "Needle1_B_R", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	//	sonicNodeGlitterCreate(This, HairBC_VfxHandle, "Needle1_B_C", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	//	sonicNodeGlitterCreate(This, HairBL_VfxHandle, "Needle1_B_L", "ef_ch_sng_yh1_hairlocus"); //"ef_ch_sng_yh1_hairlocus"
	//}
	//else
	//{
	//	sonicNodeGlitterEnd(This, HairUR_VfxHandle, true);
	//	sonicNodeGlitterEnd(This, HairUC_VfxHandle, true);
	//	sonicNodeGlitterEnd(This, HairUL_VfxHandle, true);
	//	sonicNodeGlitterEnd(This, HairBR_VfxHandle, true);
	//	sonicNodeGlitterEnd(This, HairBC_VfxHandle, true);
	//	sonicNodeGlitterEnd(This, HairBL_VfxHandle, true);
	//}
	//if (localVelocity.norm() >= 33.333)
	//{
	//	sonicNodeGlitterCreate(This, HandR_VfxHandle, "Middle1_R", "ef_ch_sng_yh1_handlocus"); //"ef_ch_sng_yh1_handlocus"
	//	sonicNodeGlitterCreate(This, HandL_VfxHandle, "Middle1_L", "ef_ch_sng_yh1_handlocus"); //"ef_ch_sng_yh1_handlocus"
	//}
	//else
	//{
	//	sonicNodeGlitterEnd(This, HandR_VfxHandle, false);
	//	sonicNodeGlitterEnd(This, HandL_VfxHandle, false);
	//}
	//if (localVelocity.norm() >= 0.5 || sonicActionState)
	//{
	//	sonicNodeGlitterCreate(This, FootR_VfxHandle, "Toe_R", "ef_ch_sng_yh1_footlocus"); //"ef_ch_sng_yh1_footlocus"
	//	sonicNodeGlitterCreate(This, FootL_VfxHandle, "Toe_L", "ef_ch_sng_yh1_footlocus"); //"ef_ch_sng_yh1_footlocus"
	//}
	//else
	//{
	//	sonicNodeGlitterEnd(This, FootR_VfxHandle, false);
	//	sonicNodeGlitterEnd(This, FootL_VfxHandle, false);
	//}

	if (PressedY && (sonic->m_ChaosEnergy >= 0.01))
	{
		//sonic->m_spMatrixNode->m_Transform.m_Position.z() += 2;
		//sonic->m_spMatrixNode->m_Transform.m_Position.x() += 1;
		//sonic->m_spMatrixNode->m_Transform.m_Position.y() += 6.4;
		sonic->PlaySound(2002050, true);
		sonicNodeGlitterCreate(This, BoostJump_VfxHandle, "Reference", "ef_ch_sng_yh1_jump_trick1");
		sonic->ChangeState("HomingAttackAfter");
		localVelocity.y() = VelocityTrickJumpForce;
		sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
		sonic->m_ChaosEnergy -= ChaosEnergyTrickJumpDrain;
		//sonic->m_spMatrixNode->NotifyChanged();
	}

	if (sonicStateJump && JumpQuickStepControlTimer <= 0 && (PressedRB || PressedLB))
	{
		//sonic->PlaySound(2002040, true);
		sonic->ChangeState("Fall");
		localVelocity.z() *= 0.842f;
		localVelocity.y() *= 0.1f;
		JumpQuickStepControlTimer = JumpQuickStepControlTime;
		hasJumpQuickStepped = true;
		hasJumpQuickSteppedSpeedCancel = true;
		if (PressedRB)
		{
			localVelocity.x() = -VelocityJumpQuickStepForce;
			sonic->ChangeAnimation("RunQuickStepR");
		}
		if (PressedLB)
		{
			localVelocity.x() = VelocityJumpQuickStepForce;
			sonic->ChangeAnimation("RunQuickStepL");
		}
		sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
	}

	if (IsQuickStepAnim && (GetAnimInfo->m_Frame >= 7.9999999f) && sonicStateJump)
		sonic->ChangeAnimation("Fall"); //Play animation

	if (This->m_StateMachine.GetCurrentState()->GetStateName() == "Fall" && JumpQuickStepSpeedCancelTimer <= 0 && hasJumpQuickSteppedSpeedCancel == true)
	{
		JumpQuickStepSpeedCancelTimer = JumpQuickStepSpeedCancelTime;
		hasJumpQuickSteppedSpeedCancel = false;
		localVelocity.x() *= 0.15f;
		sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity
	}

	if (hasJumpQuickSteppedSpeedCancel == true && JumpQuickStepSpeedCancelTimer >= 1)
		JumpQuickStepSpeedCancelTimer--;

	//if (hasJumpQuickStepped == true && JumpQuickStepAnimTimer >= 1)
	//	JumpQuickStepAnimTimer--;

	if (JumpQuickStepControlTimer >= 0)
		JumpQuickStepControlTimer--;

	//if (PressedLStick)
	//	ButtplugSetVibration(1.0f);

	if (SonicNoBallJump == true)
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_JumpShortReleaseTime] = 100000.0f;
	else
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_JumpShortReleaseTime);

	if (HoldRT && PressedUp)
	{
		sonic->ChangeState("BoardGetOn");
	}
	if (HoldRT && PressedDown)
	{
		sonic->ChangeState("BoardGetOff");
	}
	if (HoldLT && PressedRight && /*Common::GetPlayerModelScale()*/ SonicScale != 1.0f)
	{
		SonicScale = 1.0f;
		//sonicFootTrail(This, "ef_ch_sng_yh1_footlocus", true, true);
		sonicNodeGlitterEnd(This, FootR_VfxHandle, true);
		sonicNodeGlitterEnd(This, FootL_VfxHandle, true);
	}
	if (HoldLT && HoldUp)
	{
		SonicScale *= 1.1;
	}
	if (HoldLT && HoldDown)
	{
		SonicScale *= 0.9;
	}
	Common::SetPlayerModelScale(SonicScale);

	//sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity; //Determine sonic's local velocity

	//---------------------------------------------------
	// Console Stuff
	//---------------------------------------------------
	 
	//printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	//printf(sonic->GetCurrentAnimationName().c_str());
	//printf(" - ");
	//printf(GetAnimInfo->m_Name.c_str());
	//printf(" - ");
	//printf("%f", GetAnimInfo->m_Frame);
	//printf("%d", flags->m_Flags[sonic->eStateFlag_Boost]);
	//printf("%d", sonic->m_Field164);
	//printf("%f,%f,%f",sonic->m_Velocity.x(),sonic->m_Velocity.y(),sonic->m_Velocity.z());
	//printf("%f",GetPos);
	//printf("%d",sonic->m_VelocityDirty);
	//printf(" - ");
	//printf("%f",sonic->m_ChaosEnergy);
	//printf("%d",sonic->m_Is2DMode);
	//printf("%d",BounceCount);
	//printf("\n");

	//---------------------------------------------------
	// Call Original Player Update
	//---------------------------------------------------

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}

HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicEff</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>cmn200</Name>\n";
		stream << "    <Archive>cmn100</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicDiving</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicBoard</Archive>\n";
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
HOOK(void, __cdecl, InitializeApplicationPlaygroundParams, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce = parameterGroup->CreateParameterCategory("Lunar Playground", "Parameters for lunar playground");

	cat_Bounce->CreateParamTypeList((uint32_t*)&SonicRedTrailType, "Red Trail Type", "Choose trail type",
		{
			{ "None", 0},
			{ "Permanent", 1},
			{ "Speed", 2},
			{ "Action States", 3},
			{ "Boost", 4},
			{ "Boost and Action States", 5},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&SonicWhiteTrailType, "White Trail Type", "Choose trail type",
		{
			{ "None", 0},
			{ "Permanent", 1},
			{ "Speed", 2},
			{ "Action States", 3},
			{ "Boost", 4},
			{ "Boost and Action States", 5},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&SonicBlueTrailType, "Blue Trail Type", "Choose trail type",
		{
			{ "None", 0},
			{ "Permanent", 1},
			{ "Speed", 2},
			{ "Action States", 3},
			{ "Boost", 4},
			{ "Boost and Action States", 5},
		});
	cat_Bounce->CreateParamFloat(&SonicTrailSpeed, "Speed type trail activation speed");
	cat_Bounce->CreateParamFloat(&ChaosEnergyTrickJumpDrain, "Chaos Energy trick jump drain");
	cat_Bounce->CreateParamFloat(&VelocityTrickJumpForce, "Trick jump force");
	cat_Bounce->CreateParamFloat(&VelocityJumpQuickStepForce, "Jump Quickstep force");
	cat_Bounce->CreateParamInt(&JumpQuickStepControlTime, "Jump Quickstep no control time");
	//cat_Bounce->CreateParamInt(&JumpQuickStepAnimTime, "Jump Quickstep anim time");
	cat_Bounce->CreateParamInt(&JumpQuickStepSpeedCancelTime, "Jump Quickstep speed cancel time");
	cat_Bounce->CreateParamBool(&SonicNoBallJump, "No ball jump");

	parameterGroup->Flush();

	originalInitializeApplicationPlaygroundParams(This);
}

//void SharpPlayground()
//{
//	printf("bitchin\n");
//
//	//hm = LoadLibraryA("LunarSharpPlayground.dll");
//	//if (hm == NULL)
//	//{
//	//	MessageBoxA(nullptr, "Buttplug: Failed to load DLL, buttplug will not function!", "Error", 0);
//	//	return;
//	//}
//	//GetProcAddress(hm, "Main")();
//}

EXPORT void Init()
{
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(InitializeApplicationPlaygroundParams);
	// Ignore D-pad input for Sonic's control
	WRITE_JUMP(0xD97B56, (void*)0xD97B9E);

	//MessageBoxA(nullptr, "Init Halt", "Window", 0);
	
	//std::thread sharpThread(SharpPlayground);

	//HMODULE hm = LoadLibraryA("LunarSharpPlayground.dll");
	//if (hm == NULL)
	//{
	//	MessageBoxA(nullptr, "SharpPlayground: Failed to load DLL, mod will not function!", "Error", 0);
	//	return;
	//}
	//GetProcAddress(hm, "Main")();
}