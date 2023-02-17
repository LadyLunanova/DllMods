#include <Common.h>
#include "TheUINCP.h"

//Menu setup
Chao::CSD::RCPtr<Chao::CSD::CProject> prScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> scPauseAct;
boost::shared_ptr<Sonic::CGameObjectCSD> obPauseAct;
static SharedPtrTypeless menuSoundHandle;
bool UIExists = false;

void CreateChaosUI(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	Sonic::CCsdDatabaseWrapper wrapper(This->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());

	if (obPauseAct == nullptr)
	{
		auto spBGCsdProject = wrapper.GetCsdProject("TheUI");
		prScreen = spBGCsdProject->m_rcProject;
		obPauseAct = boost::make_shared<Sonic::CGameObjectCSD>(prScreen, 0.5f, "HUD_Pause", true);
		Sonic::CGameDocument::GetInstance()->AddGameObject(obPauseAct, "main", This);
	}

}
void KillScreen()
{
	if (scPauseAct)
		Chao::CSD::CProject::DestroyScene(prScreen.Get(), scPauseAct);

	if (obPauseAct)
	{
		obPauseAct->SendMessage(obPauseAct->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obPauseAct = nullptr;
	}

	UIExists = false;
}

void CHudChaosUpdate(Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto arX = 1280;
	auto arY = 720;

	if (!UIExists)
	{
		if (!obPauseAct) //Create UI project if it doesn't exist
			CreateChaosUI(This, Edx, in_rUpdateInfo);

		UIExists = true;

		scPauseAct = prScreen->CreateScene("pause_act");

		scPauseAct->SetMotion("Intro_Anim");
		scPauseAct->SetMotionFrame(10.0f);
		scPauseAct->m_MotionDisableFlag = false;
		scPauseAct->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		scPauseAct->m_MotionSpeed = 1.0f;
		scPauseAct->Update();
	}

	return;
}
HOOK(void, __fastcall, CHudSonicStageUpdate, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto speedContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	auto sonic = speedContext->m_pPlayer;
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);
	if (IsModernSonic)
		CHudChaosUpdate(This, Edx, in_rUpdateInfo);
	//printf("Gameplay HUD\n");
	originalCHudSonicStageUpdate(This, Edx, in_rUpdateInfo);
}
HOOK(void, __fastcall, CHudPlayableMenuUpdate, 0x0108D510, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	auto speedContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	auto sonic = speedContext->m_pPlayer;
	bool IsModernSonic = (Sonic::Player::CSonicClassicContext::GetInstance() == nullptr) && (Sonic::Player::CSonicSpContext::GetInstance() == nullptr);
	if (IsModernSonic)
		CHudChaosUpdate(This, Edx, in_rUpdateInfo);
	//printf("Hub world HUD\n");
	originalCHudPlayableMenuUpdate(This, Edx, in_rUpdateInfo);
}
HOOK(void, __fastcall, CHUDPauseUpdate, 0x0042A520, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	originalCHUDPauseUpdate(This);
}
void __fastcall CHudSonicStageRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreen();
}
HOOK(void, __fastcall, CHudResultStart, 0x010B6840, hh::fnd::CStateMachineBase::CStateBase* This)
{
	KillScreen();
	originalCHudResultStart(This);
}

void TheUINCP::applyPatches()
{
	INSTALL_HOOK(CHudSonicStageUpdate);
	INSTALL_HOOK(CHudPlayableMenuUpdate);
	INSTALL_HOOK(CHUDPauseUpdate);
	WRITE_MEMORY(0x16A467C, void*, CHudSonicStageRemoveCallback);
	WRITE_MEMORY(0x16A5598, void*, CHudSonicStageRemoveCallback);
	INSTALL_HOOK(CHudResultStart);
}