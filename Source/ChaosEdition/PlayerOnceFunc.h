#pragma once

void ChKillSonic(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	sonic->m_pPlayer->SendMessage(sonic->m_pPlayer->m_ActorID, boost::make_shared<Sonic::Message::MsgDead>(true));
	if (sonic->m_Grounded)
		sonic->ChangeState("NormalDamageDead");
	else
		sonic->ChangeState("NormalDamageDeadAir");
}
void ChHurtSonic(Sonic::Player::CPlayerSpeed* This)
{
	auto sonic = This->GetContext();

	sonic->ChangeState("DamageCancel");
	//sonic->m_pPlayer->SendMessage(sonic->m_pPlayer->m_ActorID, boost::make_shared<Sonic::Message::MsgDead>(false));
}