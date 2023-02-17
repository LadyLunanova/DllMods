#include <Common.h>

//Normal Stage UI
Chao::CSD::RCPtr<Chao::CSD::CProject> prPlayScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> scPlayerCount;
boost::shared_ptr<Sonic::CGameObjectCSD> obPlayUI;
static SharedPtrTypeless menuSoundHandle;

size_t prevRingCount;
bool scoreEnabled;
float xAspectOffset = 0.0f;
float yAspectOffset = 0.0f;

//Setup Functions
void CreateScreen(Sonic::CGameObject* pParentGameObject)
{
	Sonic::CCsdDatabaseWrapper wrapper(pParentGameObject->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
	auto spCsdProject = wrapper.GetCsdProject("ui_gameplay");
	prPlayScreen = spCsdProject->m_rcProject;
	obPlayUI = boost::make_shared<Sonic::CGameObjectCSD>(prPlayScreen, 0.5f, "HUD_B1", false);
	Sonic::CGameDocument::GetInstance()->AddGameObject(obPlayUI, "main", pParentGameObject);

	//if (rcMissionScreen && !spMissionScreen)
	//	pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spMissionScreen = boost::make_shared<Sonic::CGameObjectCSD>(rcMissionScreen, 0.5f, "HUD_B1", false), "main", pParentGameObject);

	//if (rcPlayScreenEv && !spPlayScreenEv)
	//	pParentGameObject->m_pMember->m_pGameDocument->AddGameObject(spPlayScreenEv = boost::make_shared<Sonic::CGameObjectCSD>(rcPlayScreenEv, 0.5f, "HUD_B1", false), "main", pParentGameObject);
}
void KillScreen()
{
	if (obPlayUI)
	{
		obPlayUI->SendMessage(obPlayUI->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
		obPlayUI = nullptr;
	}

	//if (spMissionScreen)
	//{
	//	spMissionScreen->SendMessage(spMissionScreen->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
	//	spMissionScreen = nullptr;
	//}

	//if (spPlayScreenEv)
	//{
	//	spPlayScreenEv->SendMessage(spPlayScreenEv->m_ActorID, boost::make_shared<Sonic::Message::MsgKill>());
	//	spPlayScreenEv = nullptr;
	//}
}
void ToggleScreen(const bool visible, Sonic::CGameObject* pParentGameObject)
{
	if (visible)
		CreateScreen(pParentGameObject);
	else
		KillScreen();
}
void FreezeMotion(Chao::CSD::CScene* pScene, bool end = true)
{
	pScene->SetMotionFrame(end ? pScene->m_MotionEndFrame : 0.0f);
	pScene->m_MotionSpeed = 0.0f;
	pScene->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	pScene->m_MotionDisableFlag = true;
}
const Chao::CSD::RCPtr<Chao::CSD::CScene>& GetGpSonicSafeScene(void* This)
{
	return *(Chao::CSD::RCPtr<Chao::CSD::CScene>*)(*(char**)((char*)This + 0xAC) + 0x14);
}
void __declspec(naked) GetScoreEnabled()
{
	static uint32_t returnAddress = 0x109C254;
	__asm
	{
		mov	scoreEnabled, 1
		jmp[returnAddress]
	}
}
void __fastcall CHudSonicStageRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	KillScreen();

	if (scPlayerCount)
		Chao::CSD::CProject::DestroyScene(prPlayScreen.Get(), scPlayerCount);

	prPlayScreen = nullptr;
}

//Stage States
HOOK(void, __fastcall, CHudSonicStageDelayProcessImp, 0x109A8D0, Sonic::CGameObject* This)
{
	originalCHudSonicStageDelayProcessImp(This);
	//CHudSonicStageRemoveCallback(This, nullptr, nullptr);
	if (!obPlayUI)
		CreateScreen(This);

	//size_t& flags = ((size_t*)This)[151];

	//float offset = 0.0f;

	//scPlayerCount = prWerePlayScreen->CreateScene("player_count");
	//scPlayerCount->SetPosition(0.0f, 0.0f);

	//if (flags & 0x200) // Boost Gauge
	//{
	//	rcSpeedGauge = rcPlayScreen->CreateScene("so_speed_gauge");
	//	rcRingEnergyGauge = rcPlayScreen->CreateScene("so_ringenagy_gauge");
	//	rcGaugeFrame = rcPlayScreen->CreateScene("gauge_frame");

	//	rcSpeedGauge->SetPosition(0, yAspectOffset);
	//	rcRingEnergyGauge->SetPosition(0, yAspectOffset);
	//	rcGaugeFrame->SetPosition(0, yAspectOffset);

	//	FreezeMotion(rcSpeedGauge.Get());
	//	FreezeMotion(rcRingEnergyGauge.Get());
	//	FreezeMotion(rcGaugeFrame.Get());
	//}

	//scScoreCount = prWerePlayScreen->CreateScene("score_count");
	//scScoreCount->SetPosition(0.0f, 0.0f);

	//if (flags & 0x400204 || Common::GetCurrentStageID() == bsd) // Rings
	//{
	//	rings
	//}

	//flags &= ~(0x1 | 0x2 | 0x400004 | 0x200 | 0x800 | 0x1000000); // Mask to prevent crash when game tries accessing the elements we disabled later on

	
}

HOOK(void, __fastcall, CHudSonicStageUpdateParallel, 0x1098A50, Sonic::CGameObject* This, void* Edx, const hh::fnd::SUpdateInfo& in_rUpdateInfo)
{
	originalCHudSonicStageUpdateParallel(This, Edx, in_rUpdateInfo);

	if (!obPlayUI)
		CreateScreen(This);

	//char text[256];
	//size_t rowIndex = 1;

	//const size_t flags = ((size_t*)This)[151];

	//if (scPlayerCount)
	//{
	//	const size_t liveCountAddr = Common::GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x7C, 0x9FDC });
	//	if (liveCountAddr)
	//	{
	//		sprintf(text, "%02d", *(size_t*)liveCountAddr);
	//		scPlayerCount->GetNode("player")->SetText(text);
	//	}
	//}

	//const auto playerContext = Sonic::Player::CPlayerSpeedContext::GetInstance();

	//if (scRingCount && playerContext)
	//{
	//	sprintf(text, "%03d", playerContext->m_RingCount);
	//	scRingCount->GetNode("ring")->SetText(text);
	//}

	//if (scRingGet&& playerContext&& prevRingCount < playerContext->m_RingCount&& scRingGet->m_MotionDisableFlag)
	//{
	//	scRingGet->SetMotion("get_Anim");
	//	scRingGet->SetMotionFrame(0.0f);
	//	scRingGet->m_MotionDisableFlag = false;
	//	scRingGet->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	//	scRingGet->m_MotionSpeed = 1.0f;
	//	scRingGet->Update();
	//}

	//if (playerContext)
	//	prevRingCount = playerContext->m_RingCount;
	//else
	//	prevRingCount = 0;

}


//Install Hooks
EXPORT void Init()
{
	//INSTALL_HOOK(ProcMsgGetMissionLimitTime);
	//INSTALL_HOOK(ProcMsgGetMissionCondition);
	//INSTALL_HOOK(ProcMsgNotifyLapTimeHud);
	//INSTALL_HOOK(CHudSonicStageDelayProcessImp);
	//INSTALL_HOOK(CCountdownUpdate);
	//INSTALL_HOOK(CLastBossGaugeNewUpdate);
	//INSTALL_HOOK(ProcMsgChangeCustomHud);
	INSTALL_HOOK(CHudSonicStageUpdateParallel);
	//WRITE_JUMP(0x109C1DC, GetScoreEnabled);
	//INSTALL_HOOK(ProcMsgSetPinballHud);
	//WRITE_MEMORY(0x16A467C, void*, CHudSonicStageRemoveCallback);
	//INSTALL_HOOK(CObjRingProcMsgHitEventCollision);
	//WRITE_MEMORY(0xDEBCA4, uint8_t, 0xEB);

	//WRITE_MEMORY(0x109B1A4, uint8_t, 0xE9, 0xDC, 0x02, 0x00, 0x00); // Disable lives (patched differently to not clash with Disable Lives patch)
	//WRITE_MEMORY(0x109B490, uint8_t, 0x90, 0xE9); // Disable time
	//WRITE_MEMORY(0x109B5AD, uint8_t, 0x90, 0xE9); // Disable rings
	//WRITE_MEMORY(0x109B8F5, uint8_t, 0x90, 0xE9); // Disable boost gauge
	//WRITE_MEMORY(0x109BC88, uint8_t, 0x90, 0xE9); // Disable boost button
	//WRITE_MEMORY(0x109BEF0, uint8_t, 0x90, 0xE9); // Disable mission countdown
	//WRITE_MEMORY(0x109C3E2, uint8_t, 0x90, 0xE9); // Disable mission rank
	//WRITE_MEMORY(0x109B6A7, uint8_t, 0x90, 0xE9); // Disable Time Eater rings
}