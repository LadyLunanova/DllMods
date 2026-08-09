
//Classic State Stuff
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto GetAnim = sonic->GetCurrentAnimationName();
	auto IsClassicSonic = (Sonic::Player::CSonicClassicContext::GetInstance() != nullptr);
	auto IsGrounded = sonic->m_Grounded;
	if (IsClassicSonic && !IsGrounded && GetAnim == "Walk")
		sonic->ChangeAnimation("Fall"); //Play animation
	if (IsClassicSonic)
	{
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicClassicParameter_SpinVelocityWithChargeLv0] = 16.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicClassicParameter_SpinVelocityWithChargeLv1] = 48.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicClassicParameter_SpinVelocityWithChargeLv2] = 64.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicClassicParameter_SpinDynamicFrictionalForce] = 0.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicClassicParameter_SpinDynamicFrictionalForceBySkill] = 0.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicClassicParameter_SpinDecRate] = 0.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicClassicParameter_SpinDecRateBySkill] = 0.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_HomingSpeed] = 50.3f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_HomingSpeedOfDummy] = 18.1f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_HomingBackEnableSpeed] = 10.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AccelerationForce] = 12.5f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AccelerationForceBaseUp] = 25.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_AccelerationForceBySkill] = 23.2f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_DecelerationForceHigh] = 12.5f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_DecelerationForceLow] = 25.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxDownVelocity] = 30.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity] = 145.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxUpVelocity] = 70.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxVelocityBasis] = 35.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxVelocityBasisBaseUp] = 40.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxVelocityFinalMax] = 145.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::eSonicParameter_AdditinalMaxVelocityBySkill] = 5.0f;
	}
	else
	{
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicClassicParameter_SpinVelocityWithChargeLv0);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicClassicParameter_SpinVelocityWithChargeLv1);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicClassicParameter_SpinVelocityWithChargeLv2);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicClassicParameter_SpinDynamicFrictionalForce);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicClassicParameter_SpinDynamicFrictionalForceBySkill);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicClassicParameter_SpinDecRate);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicClassicParameter_SpinDecRateBySkill);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_HomingSpeed);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_HomingSpeedOfDummy);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_HomingBackEnableSpeed);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AccelerationForce);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AccelerationForceBaseUp);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_AccelerationForceBySkill);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_DecelerationForceHigh);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_DecelerationForceLow);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxDownVelocity);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxUpVelocity);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxVelocityBasis);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxVelocityBasisBaseUp);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxVelocityFinalMax);
		sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::eSonicParameter_AdditinalMaxVelocityBySkill);
	}
	//printf("Current Anim: %s\n", GetAnim.c_str());
	originalCPlayerSpeedUpdate(This, _, updateInfo);
}

EXPORT void Init()
{
	INSTALL_HOOK(CPlayerSpeedUpdate);
	WRITE_MEMORY(0x01282860, char*, "sc_jump_d_s_loop");
	WRITE_MEMORY(0x012828C7, char*, "sc_jump_d_loop");
}