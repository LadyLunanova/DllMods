#pragma once

float ChSonicModelScale = 1.0f;

bool ChSonicBigScaleEnable = false;
bool ChSonicSmallScaleEnable = false;
bool ChSonicFlashbangEnable = false;
bool ChSonicBoardEnable = false;
bool ChSonicHideEnable = false;
bool ChSonicSlowMaxSpdEnable = false;
bool ChSonicInfMaxSpdEnable = false;
bool ChSonicFrozenEnable = false;

int ChSonicBigScaleTimer = 300;
int ChSonicSmallScaleTimer = 300;
int ChSonicFlashbangTimer = 23;
int ChSonicBoardTimer = 300;
int ChSonicHideTimer = 180;
int ChSonicSlowMaxSpdTimer = 180;
int ChSonicInfMaxSpdTimer = 180;
int ChSonicFrozenTimer = 120;

void ChSonicBigScale()
{
	if (ChSonicBigScaleEnable)
	{

		ChSonicModelScale = 4.20f;
		ChSonicBigScaleTimer--;
		if (ChSonicBigScaleTimer <= 0)
		{
			ChSonicBigScaleEnable = false;
			ChSonicBigScaleTimer = 300;
			ChSonicModelScale = 1.0f;
		}
	}
	else
	{
		
		ChSonicBigScaleTimer = 300;
		if (!ChSonicSmallScaleEnable)
			ChSonicModelScale = 1.0f;
	}
}

void ChSonicSmallScale()
{
	if (ChSonicSmallScaleEnable)
	{
		ChSonicModelScale = 0.333f;
		ChSonicSmallScaleTimer--;
		if (ChSonicSmallScaleTimer <= 0)
		{
			ChSonicSmallScaleEnable = false;
			ChSonicSmallScaleTimer = 300;
			ChSonicModelScale = 1.0f;
		}
	}
	else
	{
		ChSonicSmallScaleTimer = 300;
		if (!ChSonicBigScaleEnable)
			ChSonicModelScale = 1.0f;
	}
}

void ChSonicFlashbang()
{
	if (ChSonicFlashbangEnable)
	{
		ChSonicFlashbangTimer--;
		if (ChSonicFlashbangTimer <= 0)
		{
			ChSonicFlashbangEnable = false;
			ChSonicFlashbangTimer = 23;
		}
	}
	else
		ChSonicFlashbangTimer = 23;
}

void ChSonicBoard(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	if (ChSonicBoardEnable)
	{
		ChSonicBoardTimer--;
		if (ChSonicBoardTimer <= 0)
		{
			sonic->ChangeState("BoardGetOff");
			ChSonicBoardEnable = false;
			ChSonicBoardTimer = 300;
		}
	}
	else
		ChSonicBoardTimer = 300;
}

void ChSonicHide(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	if (ChSonicHideEnable)
	{
		sonic->m_pPlayer->SendMessage(sonic->m_pPlayer->m_ActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(false));
		ChSonicHideTimer--;
		if (ChSonicHideTimer <= 0)
		{
			sonic->m_pPlayer->SendMessage(sonic->m_pPlayer->m_ActorID, boost::make_shared<Sonic::Message::MsgSetVisible>(true));
			ChSonicHideEnable = false;
			ChSonicHideTimer = 180;
		}
	}
	else
	{
		ChSonicHideTimer = 180;
	}
}

void ChSonicSlowMaxSpd(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	if (ChSonicSlowMaxSpdEnable)
	{
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity] = 10.0f;
		ChSonicSlowMaxSpdTimer--;
		if (ChSonicSlowMaxSpdTimer <= 0)
		{
			sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity);
			ChSonicSlowMaxSpdEnable = false;
			ChSonicSlowMaxSpdTimer = 180;
		}
	}
	else
	{
		ChSonicSlowMaxSpdTimer = 180;
	}
}

void ChSonicInfMaxSpd(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	if (ChSonicInfMaxSpdEnable)
	{
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity] = 99999.0f;
		ChSonicInfMaxSpdTimer--;
		if (ChSonicInfMaxSpdTimer <= 0)
		{
			sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity);
			ChSonicInfMaxSpdEnable = false;
			ChSonicInfMaxSpdTimer = 180;
		}
	}
	else
	{
		ChSonicInfMaxSpdTimer = 180;
	}
}

void ChSonicFrozen(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	if (ChSonicFrozenEnable)
	{
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity] = 0.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxDownVelocity] = 0.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_MaxUpVelocity] = 0.0f;
		sonic->m_spParameter->m_scpNode->m_ValueMap[Sonic::Player::ePlayerSpeedParameter_Gravity] = 0.0f;
		ChSonicFrozenTimer--;
		if (ChSonicFrozenTimer <= 0)
		{
			sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxHorzVelocity);
			sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxDownVelocity);
			sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_MaxUpVelocity);
			sonic->m_spParameter->m_scpNode->m_ValueMap.erase(Sonic::Player::ePlayerSpeedParameter_Gravity);
			ChSonicFrozenEnable = false;
			ChSonicFrozenTimer = 120;
		}
	}
	else
	{
		ChSonicFrozenTimer = 120;
	}
}