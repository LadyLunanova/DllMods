#pragma once
#include <Sonic/Message/MsgDamage.h>

void SetTextCast(const char* Text);
void SetBGWidth(float Width);

void ChActMoonjump(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;

	sonic->ChangeState("WallJump");
	localVelocity.y() = 123;
	localVelocity.z() = 0;
	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity;
	SetTextCast("Moon Jump");
	SetBGWidth(0.3);
}

void ChActSlowMaxSpd()
{
	ChSonicSlowMaxSpdEnable = true;
	SetTextCast("Slow Max Speed");
	SetBGWidth(0.45);
}

void ChActFlashbang()
{
	ChSonicFlashbangEnable = true;
	SetTextCast("Flashbang");
	SetBGWidth(0.25);
}

void ChActDrainBoost(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	sonic->m_ChaosEnergy = 0;
	SetTextCast("Drained Boost");
	SetBGWidth(0.333);
}

void ChAct69Boost(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	sonic->m_ChaosEnergy = 69;
	SetTextCast("69% Boost");
	SetBGWidth(0.25);
}

void ChActStumble(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	sonic->ChangeState("StumbleAir");
	SetTextCast("Stubbed Toe");
	SetBGWidth(0.28);
}

void ChAct69Rings(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	sonic->m_RingCount = 69;
	sonic->PlaySound(6002002, false);
	SetTextCast("Nice Rings");
	SetBGWidth(0.28);
}

void ChActDropRings(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	sonic->m_RingCount = 0;
	sonic->PlaySound(4002001, false);
	SetTextCast("Drop Rings");
	SetBGWidth(0.28);
}

void ChActSmallSonic()
{
	ChSonicSmallScaleEnable = true;
	SetTextCast("Tiny Sonic");
	SetBGWidth(0.28);
}

void ChActBigSonic()
{
	ChSonicBigScaleEnable = true;
	SetTextCast("Huge Sonic");
	SetBGWidth(0.28);
}

void ChActSlapped(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;

	sonic->ChangeState("Fall");
	localVelocity.y() = 25;
	localVelocity.z() = -32;
	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity;
	sonic->PlaySound(2002073, false);
	SetTextCast("Slapped");
	SetBGWidth(0.25);
}

void ChActDrop(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;

	sonic->ChangeState("Stomping");
	localVelocity.y() = -123;
	localVelocity.z() = 0;
	sonic->m_Velocity = sonic->m_spMatrixNode->m_Transform.m_Rotation * localVelocity;
	SetTextCast("Drop");
	SetBGWidth(0.1);
}

void ChActHideSonic()
{
	ChSonicHideEnable = true;
	SetTextCast("Hidden Sonic");
	SetBGWidth(0.29);
}

void ChActBoard(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	ChSonicBoardEnable = true;
	sonic->ChangeState("BoardGetOn");
	SetTextCast("Get on Board");
	SetBGWidth(0.4);
}

void ChActInfMaxSpd()
{
	ChSonicInfMaxSpdEnable = true;
	SetTextCast("Uncapped Speed");
	SetBGWidth(0.45);
}

void ChActKill(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	sonic->m_pPlayer->SendMessage(sonic->m_pPlayer->m_ActorID, boost::make_shared<Sonic::Message::MsgDead>(true));
	if (sonic->m_Grounded)
		sonic->ChangeState("NormalDamageDead");
	else
		sonic->ChangeState("NormalDamageDeadAir");
	sonic->PlaySound(4002008, false);
	SetTextCast("Death");
	SetBGWidth(0.12);
}

void ChActHurt(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	sonic->ChangeState("DamageCancel");
	if (sonic->m_RingCount >= 1)
	{
		sonic->m_RingCount = 0;
		sonic->PlaySound(4002001, false);
		//sonic->m_pPlayer->SendMessage(sonic->m_pPlayer->m_ActorID, boost::make_shared<Sonic::Message::MsgDamage>(true));
	}
	else
	{
		sonic->m_pPlayer->SendMessage(sonic->m_pPlayer->m_ActorID, boost::make_shared<Sonic::Message::MsgDead>(true));
		if (sonic->m_Grounded)
			sonic->ChangeState("NormalDamageDead");
		else
			sonic->ChangeState("NormalDamageDeadAir");
		sonic->PlaySound(4002008, false);
	}
	SetTextCast("Hurt");
	SetBGWidth(0.11);
}

void ChActSonicFrozen()
{
	ChSonicFrozenEnable = true;
	SetTextCast("Frozen Sonic");
	SetBGWidth(0.45);
}