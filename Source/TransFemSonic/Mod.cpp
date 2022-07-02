#include <Common.h>

// Original code by Brianuuu: https://github.com/brianuuu
enum ResultRankType : int
{
	E = -1,
	D,
	C,
	B,
	A,
	S
};
struct ResultData
{
	int m_score;
	ResultRankType m_rank;
	ResultRankType m_perfectRank;
	int m_nextRankTime;
	float m_totalProp;	// result progress bar (time prop + ring prop) 
	float m_timeProp;	// result progress bar (time prop)
};
inline uint32_t GetMultiLevelAddress(uint32_t initAddress, std::vector<uint32_t> offsets)
{
	uint32_t address = *(uint32_t*)initAddress;
	for (uint32_t i = 0; i < offsets.size(); i++)
	{
		uint32_t const& offset = offsets[i];
		address += offset;

		if (i < offsets.size() - 1)
		{
			address = *(uint32_t*)address;
		}
	}
	return address;
}
inline uint32_t GetCurrentStageID()
{
	uint32_t stageIDAddress = GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x80, 0x0 });
	return *(uint32_t*)stageIDAddress;
}

int DoubleVOFix = 0;
int DoubleTornadoVOFix = 0;
//Mod Compatibility Checks
bool IsUnleashedProject = false;
bool IsShiveryMountain = false;
bool IsWaterPalace = false;
bool Is06Project = false;
bool IsGHZStageMod = false;
bool IsCPZStageMod = false;
bool IsSSZStageMod = false;
bool IsEUCStageMod = false;
bool IsPLAStageMod = false;
bool IsPAMStageMod = false;
bool IsTransTails = false;
//Config Option
bool IsSWAMDL = false;
bool IsIdleChatVO = true;
bool IsRankChatVO = true;
bool IsBossTauntVO = true;
bool IsTransRankVO = false;
bool IsSuperYellVO = true;
bool IsQuickstepVO = true;
bool IsTrickFinishVO = true;
bool IsCheckpointVO = true;
bool IsTornadoVO = true;
bool IsFootTapSFX = true;
static SharedPtrTypeless bossVoiceHandle;

//Gameplay Voicelines
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto GetAnim = sonic->GetCurrentAnimationName();
	bool IsYawnAnim = GetAnim == "IdleD";     //Yawn Anim
	bool IsTapAnim = GetAnim == "IdleE";     //Foot Tap Anim
	bool IsIdleAnim = GetAnim == "IdleA" || GetAnim == "IdleB" || GetAnim == "IdleC" || GetAnim == "IdleD" || GetAnim == "IdleE";     //idle Anim
	bool IsFinishAnim = GetAnim == "Trick_FinishF" || GetAnim == "Trick_FinishB" || GetAnim == "Trick_FinishSV";     //Trick Finish Anim
	bool IsRankAnim = GetAnim == "ResultRankS" || GetAnim == "ResultRankA" || GetAnim == "ResultRankB" || GetAnim == "ResultRankC" || GetAnim == "ResultRankD";		//Result Start Anim
	bool IsTransformAnim = GetAnim == "TrnsStSp";		//Transform Start Anim
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	const auto GetAnimInfo = boost::make_shared<Sonic::Message::MsgGetAnimationInfo>();
	//sonic->m_pPlayer->SendMessageImm(sonic->m_pPlayer->m_ActorID, GetAnimInfo);
	auto sonicpos = sonic->m_spMatrixNode->m_Transform;
	int rand1to5 = std::rand() % 5;

	if (Sonic::Player::CSonicSpContext::GetInstance() == nullptr)
	{
		sonic->m_pPlayer->SendMessageImm(sonic->m_pPlayer->m_ActorID, GetAnimInfo);

		//Play random Idle anim
		//if (PressedY)
		//{
		//	//sonic->ChangeAnimation("IdleA");
		//	switch (rand1to5)
		//	{
		//	case 0:
		//		sonic->ChangeAnimation("IdleA");
		//		break;
		//	case 1:
		//		sonic->ChangeAnimation("IdleB");
		//		break;
		//	case 2:
		//		sonic->ChangeAnimation("IdleC");
		//		break;
		//	case 3:
		//		sonic->ChangeAnimation("IdleD");
		//		break;
		//	case 4:
		//		sonic->ChangeAnimation("IdleE");
		//		break;
		//	}
		//}
	}

	//Idle Chatter
	if (IsIdleAnim && (GetAnimInfo->m_Frame >= 1 && GetAnimInfo->m_Frame < 2) && !IsUnleashedProject && !IsShiveryMountain && !IsWaterPalace && !Is06Project && (DoubleVOFix <= 0) && IsIdleChatVO)
	{
		switch (GetCurrentStageID())
		{
		case ghz100:
		case ghz200:
			if (IsGHZStageMod)
				sonic->PlaySound(3002047, false); //Play voiceline
			else
				sonic->PlaySound(3002026, false); //Play voiceline
			//printf("Green Hill");
			break;
		case cpz100:
		case cpz200:
			if (IsCPZStageMod)
				sonic->PlaySound(3002047, false); //Play voiceline
			else
			sonic->PlaySound(3002027, false); //Play voiceline
			//printf("Chemical Plant");
			break;
		case ssz100:
		case ssz200:
			if (IsSSZStageMod)
				sonic->PlaySound(3002047, false); //Play voiceline
			else
				sonic->PlaySound(3002028, false); //Play voiceline
			//printf("Sky Sanctuary");
			break;
		case sph100:
		case sph200:
			sonic->PlaySound(3002029, false); //Play voiceline
			//printf("Speed Highway");
			break;
		case cte100:
		case cte200:
			sonic->PlaySound(3002030, false); //Play voiceline
			//printf("City Escape");
			break;
		case ssh100:
		case ssh200:
			sonic->PlaySound(3002031, false); //Play voiceline
			//printf("Seaside Hill");
			break;
		case csc100:
		case csc200:
			sonic->PlaySound(3002032, false); //Play voiceline
			//printf("Crisis City");
			break;
		case euc100:
		case euc200:
			if (IsEUCStageMod)
				sonic->PlaySound(3002047, false); //Play voiceline
			else
				sonic->PlaySound(3002033, false); //Play voiceline
			//printf("Rooftop Run");
			break;
		case pla200:
			if (IsPLAStageMod)
				sonic->PlaySound(3002047, false); //Play voiceline
			else
			{
				if (sonicpos.m_Position.y() > 1.6 && sonicpos.m_Position.z() < -1550)
				{
					sonic->PlaySound(3002035, false); //Play voiceline
				}
				else
				{
					sonic->PlaySound(3002034, false); //Play voiceline
				}
			}
			//printf("Planet Wisp");
			break;
		case pam000:
			//printf("Playable Menu");
			if (!IsPAMStageMod)
			{
				if (sonicpos.m_Position.x() < -57.379)
				{
					//printf("Collection Room");
					sonic->PlaySound(3002036, false); //Play voiceline
				}
				else
				{
					//printf("Playable Menu");
					sonic->PlaySound(3002024, false); //Play voiceline
				}
			}
			break;
		case fig000:
			sonic->PlaySound(3002025, false); //Play voiceline
			//printf("Figure Room");
			break;
		}
		DoubleVOFix = 5;
	}

	//Unleashed Project Idle Chatter
	if (IsIdleAnim && (GetAnimInfo->m_Frame >= 1 && GetAnimInfo->m_Frame < 2) && IsUnleashedProject && !IsWaterPalace && !IsShiveryMountain && !Is06Project && (DoubleVOFix <= 0) && IsIdleChatVO)
	{
		switch (GetCurrentStageID())
		{
		case ghz200:
			sonic->PlaySound(3002040, false); //Play voiceline
			//printf("Windmill Isle Act 2");
			break;
		case cpz200:
			sonic->PlaySound(3002041, false); //Play voiceline
			//printf("Savannah Citadel");
			break;
		case ssz200:
			sonic->PlaySound(3002042, false); //Play voiceline
			//printf("Rooftop Run");
			break;
		case sph200:
			sonic->PlaySound(3002043, false); //Play voiceline
			//printf("Dragon Road");
			break;
		case cte200:
			sonic->PlaySound(3002044, false); //Play voiceline
			//printf("Cool Edge");
			break;
		case ssh200:
			sonic->PlaySound(3002045, false); //Play voiceline
			//printf("Arid Sands");
			break;
		case csc200:
			sonic->PlaySound(3002029, false); //Play voiceline
			//printf("Skyscraper Scamper");
			break;
		case euc200:
			sonic->PlaySound(3002046, false); //Play voiceline
			//printf("Jungle Joyride");
			break;
		case pla200:
			sonic->PlaySound(3002039, false); //Play voiceline
			//printf("Windmill Isle Act 1");
			break;
		}
		DoubleVOFix = 5;
	}

	//Shivery Mountainsides Idle Chatter
	if (IsIdleAnim && (GetAnimInfo->m_Frame >= 1 && GetAnimInfo->m_Frame < 2) && !IsUnleashedProject && !IsWaterPalace && IsShiveryMountain && !Is06Project && (DoubleVOFix <= 0) && IsIdleChatVO)
	{
		switch (GetCurrentStageID())
		{
		case ghz200:
			sonic->PlaySound(3002044, false); //Play voiceline
			//printf("Stage");
			break;
		case pam000:
			//printf("Playable Menu");
			sonic->PlaySound(3002036, false); //Play voiceline
			break;
		}
		DoubleVOFix = 5;
	}

	//Water Palace Idle Chatter
	if (IsIdleAnim && (GetAnimInfo->m_Frame >= 1 && GetAnimInfo->m_Frame < 2) && !IsUnleashedProject && IsWaterPalace && !IsShiveryMountain && !Is06Project && (DoubleVOFix <= 0) && IsIdleChatVO)
	{
		switch (GetCurrentStageID())
		{
		case ghz200:
			sonic->PlaySound(3002047, false); //Play voiceline
			//printf("Stage");
			break;
		case pam000:
			//printf("Playable Menu");
			sonic->PlaySound(3002047, false); //Play voiceline
			break;
		}
		DoubleVOFix = 5;
	}

	//06 Project Idle Chatter
	if (IsIdleAnim && (GetAnimInfo->m_Frame >= 1 && GetAnimInfo->m_Frame < 2) && !IsUnleashedProject && !IsWaterPalace && !IsShiveryMountain && Is06Project && (DoubleVOFix <= 0) && IsIdleChatVO)
	{
		switch (GetCurrentStageID())
		{
		case ghz200:
			sonic->PlaySound(3002031, false); //Play voiceline
			//printf("Wave Ocean");
			break;
		case cpz200:
			sonic->PlaySound(3002045, false); //Play voiceline
			//printf("Dusty Desert");
			break;
		case ssz200:
			sonic->PlaySound(3002044, false); //Play voiceline
			//printf("White Acropolis");
			break;
		case sph200:
			sonic->PlaySound(3002032, false); //Play voiceline
			//printf("Crisis City");
			break;
		case cte200:
			sonic->PlaySound(3002047, false); //Play voiceline
			//printf("Flame Core");
			break;
		case ssh200:
			sonic->PlaySound(3002047, false); //Play voiceline
			//printf("Radical Train");
			break;
		case csc200:
			sonic->PlaySound(3002034, false); //Play voiceline
			//printf("Tropical Jungle");
			break;
		case euc200:
			sonic->PlaySound(3002046, false); //Play voiceline
			//printf("Kingdom Valley");
			break;
		case pla200:
			sonic->PlaySound(3002035, false); //Play voiceline
			//printf("Aquatic Base");
			break;
		}
		DoubleVOFix = 5;
	}

	//Trick Finisher voice line
	if (IsFinishAnim && (GetAnimInfo->m_Frame > 10 && GetAnimInfo->m_Frame < 11) && (DoubleVOFix <= 0) && IsTrickFinishVO)
	{
		sonic->PlaySound(3002013, false); //Play voiceline
		DoubleVOFix = 5;
	}

	//Yawn voice line
	if (IsYawnAnim && (GetAnimInfo->m_Frame > 260 && GetAnimInfo->m_Frame < 261) && (DoubleVOFix <= 0) && !IsSWAMDL)
	{
		sonic->PlaySound(3002012, false); //Play voiceline
		DoubleVOFix = 5;
	}

	//Super Sonic Transform Yell
	if (IsTransformAnim && (GetAnimInfo->m_Frame >= 2 && GetAnimInfo->m_Frame < 3) && (DoubleVOFix <= 0) && IsSuperYellVO)
	{
		sonic->PlaySound(3002037, false); //Play voiceline
		sonic->PlaySound(3002038, false); //Play voiceline
		DoubleVOFix = 5;
	}

	//Foot Tap sfx
	if (IsTapAnim && ((GetAnimInfo->m_Frame > 38 && GetAnimInfo->m_Frame < 39) || (GetAnimInfo->m_Frame > 50 && GetAnimInfo->m_Frame < 51) || (GetAnimInfo->m_Frame > 66 && GetAnimInfo->m_Frame < 67) || (GetAnimInfo->m_Frame > 94 && GetAnimInfo->m_Frame < 95)) && (DoubleVOFix <= 0) && !IsSWAMDL && IsFootTapSFX)
	{
		switch (sonic->m_Field164)
		{
		case 0: //Stone/Normal
		case 1:
		case 3:
			sonic->PlaySound(2002000, true); //Play SFX
			break;
		case 2: //Water
		case 11:
		case 14:
			sonic->PlaySound(2002020, true); //Play SFX
			break;
		case 4: //Dirt
		case 9:
			sonic->PlaySound(2002008, true); //Play SFX
			break;
		case 5: //Wood
			sonic->PlaySound(2002012, true); //Play SFX
			break;
		case 6: //Grass
			sonic->PlaySound(2002016, true); //Play SFX
			break;
		case 8: //Metal
			sonic->PlaySound(2002004, true); //Play SFX
			break;
		case 10: //Glass
			sonic->PlaySound(2002085, true); //Play SFX
			break;
		default:
			break;
		}
		DoubleVOFix = 5;
	}

	if (DoubleVOFix > 0)
		DoubleVOFix--;

	if (DoubleTornadoVOFix > 0)
		DoubleTornadoVOFix--;

	//printf(sonic->GetCurrentAnimationName().c_str());
	//printf(" - ");
	//printf("%f", GetAnimInfo->m_Frame);
	//printf(" - ");
	/*
	switch (GetCurrentStageID())
	{
	case ghz100:
		printf("Green Hill Act 1 - ");
		break;
	case ghz200:
		printf("Green Hill Act 2 - ");
		break;
	case cpz100:
		printf("Chemical Plant Act 1 - ");
		break;
	case cpz200:
		printf("Chemical Plant Act 2 - ");
		break;
	case ssz100:
		printf("Sky Sanctuary Act 1 - ");
		break;
	case ssz200:
		printf("Sky Sanctuary Act 2 - ");
		break;
	case sph100:
		printf("Speed Highway Act 1 - ");
		break;
	case sph200:
		printf("Speed Highway Act 2 - ");
		break;
	case cte100:
		printf("City Escape Act 1 - ");
		break;
	case cte200:
		printf("City Escape Act 2 - ");
		break;
	case ssh100:
		printf("Seaside Hill Act 1 - ");
		break;
	case ssh200:
		printf("Seaside Hill Act 2 - ");
		break;
	case csc100:
		printf("Crisis City Act 1 - ");
		break;
	case csc200:
		printf("Crisis City Act 2 - ");
		break;
	case euc100:
		printf("Rooftop Run Act 1 - ");
		break;
	case euc200:
		printf("Rooftop Run Act 2 - ");
		break;
	case pla100:
		printf("Planet Wisp Act 1 - ");
		break;
	case pla200:
		printf("Planet Wisp Act 2 - ");
		break;
	case pam000:
		printf("Playable Menu - ");
		break;
	case fig000:
		printf("Figure Room - ");
		break;
	case bms:
		printf("Metal Sonic Rival - ");
		break;
	case bsd:
		printf("Shadow Rival - ");
		break;
	case bsl:
		printf("Silver Rival - ");
		break;
	case bde:
		printf("Death Egg Robot - ");
		break;
	case bpc:
		printf("Perfect Chaos - ");
		break;
	case bne:
		printf("Egg Dragoon - ");
		break;
	case blb:
		printf("Time Eater - ");
		break;
	}
	switch (GetCurrentStageID())
	{
	case Mission1:
		printf("Mission 1 / Hard Mode");
		break;
	case Mission2:
		printf("Mission 2");
		break;
	case Mission3:
		printf("Mission 3");
		break;
	case Mission4:
		printf("Mission 4");
		break;
	case Mission5:
		printf("Mission 5");
		break;
	default:
		printf("Default");
		break;
	}
	*/
	//printf("%f", sonicpos.m_Position.x());
	//printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}
HOOK(void, __fastcall, EnterRunQuickStep, 0x01231360, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	if (IsQuickstepVO)
		sonic->PlaySound(3002016, false); //Play voiceline
	originalEnterRunQuickStep(This);
}
HOOK(void, __fastcall, ProcMsgNotifyLapTimeHud, 0x1097640, Sonic::CGameObject* This, void* Edx, hh::fnd::Message& in_rMsg)
{
	auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (sonic->m_HorizontalVelocity.norm() >= 15.0f && IsCheckpointVO)
		sonic->PlaySound(3002001, false); //Play voiceline

	//printf("CHECKPOINT???"); printf("\n");
	//printf("CHECKPOINT???"); printf("\n");
	//printf("CHECKPOINT???"); printf("\n");
	//printf("CHECKPOINT???"); printf("\n");
	//printf("CHECKPOINT???"); printf("\n");
	//printf("CHECKPOINT???"); printf("\n");
	//printf("CHECKPOINT???"); printf("\n");
	originalProcMsgNotifyLapTimeHud(This, Edx, in_rMsg);
}

//Boss Taunt Voicelines
HOOK(void, __fastcall, CHudResultStart, 0x010B6840, hh::fnd::CStateMachineBase::CStateBase* This)
{
	if (IsBossTauntVO)
	{
		switch (GetCurrentStageID())
		{
		case bsd:
			Common::PlaySoundStatic(bossVoiceHandle, 3002022);
			break;
		case bsl:
			Common::PlaySoundStatic(bossVoiceHandle, 3002023);
			break;
		case bpc:
			Common::PlaySoundStatic(bossVoiceHandle, 3002019);
			break;
		case bne:
			Common::PlaySoundStatic(bossVoiceHandle, 3002020);
			break;
		case blb:
			Common::PlaySoundStatic(bossVoiceHandle, 3002021);
			break;
		}
	}
	originalCHudResultStart(This);
}

//Rank Voicelines
ResultData m_resultData;
HOOK(int, __fastcall, RankQuote_CStateGoalFadeBeforeBegin, 0xCFE080, uint32_t* This)
{
	int result = originalRankQuote_CStateGoalFadeBeforeBegin(This);
	{
		m_resultData = *(ResultData*)(This[2] + 0x16C);
	}
	return result;
}
void PlayRankQuote()
{
	uint32_t voiceCueID = -1;
	uint32_t slamCueID = -1;
	if (IsRankChatVO)
	{
		switch (m_resultData.m_perfectRank)
		{
		case ResultRankType::S:
			if (IsTransRankVO || IsShiveryMountain || IsWaterPalace)
				voiceCueID = 40006;
			else
				voiceCueID = 40000;
			slamCueID = 1000041;
			printf("[Rank Quote] Rank = S");
			break;
		case ResultRankType::A: voiceCueID = 40001; slamCueID = 1000042; printf("[Rank Quote] Rank = A"); break;
		case ResultRankType::B: voiceCueID = 40002; slamCueID = 1000043; printf("[Rank Quote] Rank = B"); break;
		case ResultRankType::C: voiceCueID = 40003; slamCueID = 1000044; printf("[Rank Quote] Rank = C"); break;
		case ResultRankType::D: voiceCueID = 40004; slamCueID = 1000045; printf("[Rank Quote] Rank = D"); break;
		default: voiceCueID = 40005; slamCueID = 1000046; printf("[Rank Quote] Rank = E"); break;
		}
	}
	else
	{
		switch (m_resultData.m_perfectRank)
		{
		case ResultRankType::S: slamCueID = 1000041; printf("[Rank Quote] Rank = S"); break;
		case ResultRankType::A: slamCueID = 1000042; printf("[Rank Quote] Rank = A"); break;
		case ResultRankType::B: slamCueID = 1000043; printf("[Rank Quote] Rank = B"); break;
		case ResultRankType::C: slamCueID = 1000044; printf("[Rank Quote] Rank = C"); break;
		case ResultRankType::D: slamCueID = 1000045; printf("[Rank Quote] Rank = D"); break;
		default: slamCueID = 1000046; printf("[Rank Quote] Rank = E"); break;
		}
	}

	static SharedPtrTypeless rankVoiceHandle;
	Common::PlaySoundStatic(rankVoiceHandle, voiceCueID);

	static SharedPtrTypeless rankSoundHandle;
	Common::PlaySoundStatic(rankSoundHandle, slamCueID);
}
HOOK(void, __fastcall, RankQuote_ChangeRank, 0x10B76D0, uint32_t* This)
{
	WRITE_MEMORY(0x11D237A, int, -1);
	PlayRankQuote();

	originalRankQuote_ChangeRank(This);
	WRITE_MEMORY(0x11D237A, uint32_t, 1010002);
}
HOOK(void, __fastcall, RankQuote_ShowRank, 0x10B7800, uint32_t* This)
{
	FUNCTION_PTR(bool, __cdecl, IsPerfectBonus, 0x10B8A90);
	if (!IsPerfectBonus())
	{
		WRITE_MEMORY(0x11D237A, int, -1);
		PlayRankQuote();
	}

	originalRankQuote_ShowRank(This);
	WRITE_MEMORY(0x11D237A, uint32_t, 1010002);
}

//Tornado Voiceline
void CscGenericTornadoNull()
{
	if (DoubleTornadoVOFix <= 0 && IsTornadoVO)
	{
		Common::PlaySoundStatic(bossVoiceHandle, 3002048);
		DoubleTornadoVOFix = 420;
	}
	//printf("TORNADO SPAWNED"); printf("\n");
}

//Handle Archives
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>SonicSp</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Title</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev031</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev041</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev042</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev091</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam_cmn</Name>\n";
		stream << "    <Archive>FEMpam</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>blb</Name>\n";
		stream << "    <Archive>SonicVoice</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>blb001</Name>\n";
		stream << "    <Archive>SonicVoice</Archive>\n";
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
HOOK(void, __fastcall, LoadArchive, 0x69AB10, void* This, void* Edx, void* A3, void* A4, const hh::base::CSharedString& name, void* archiveInfo, void* A7, void* A8)
{
	if (strstr(name.c_str(), "evSonic") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority
	if (strstr(name.c_str(), "FEM") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchive(This, Edx, A3, A4, name, archiveInfo, A7, A8);
}
HOOK(void, __fastcall, LoadArchiveList, 0x69C270, void* This, void* Edx, void* A3, void* A4, const hh::base::CSharedString& name, void* archiveInfo)
{
	if (strstr(name.c_str(), "evSonic") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority
	if (strstr(name.c_str(), "FEM") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchiveList(This, Edx, A3, A4, name, archiveInfo);
}

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationParams, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CParameterCategory* cat_Bounce = parameterGroup->CreateParameterCategory("Trans Fem Sonic", "Parameters for Trans Fem Sonic");

	//cat_Bounce->CreateParamInt(&someInt, "Integer");
	//cat_Bounce->CreateParamBool(&someBool, "Bool");
	//cat_Bounce->CreateParamFloat(&someFloat, "Float");

	cat_Bounce->CreateParamBool(&IsIdleChatVO, "Idle Chatter");
	cat_Bounce->CreateParamBool(&IsRankChatVO, "Rank Quotes");
	cat_Bounce->CreateParamBool(&IsBossTauntVO, "Boss Taunts");
	cat_Bounce->CreateParamBool(&IsTransRankVO, "Trans rights on S-Rank");
	cat_Bounce->CreateParamBool(&IsSuperYellVO, "Super Transform Yell");
	cat_Bounce->CreateParamBool(&IsQuickstepVO, "Quickstep Grunts");
	cat_Bounce->CreateParamBool(&IsTrickFinishVO, "Trick Finisher Voicelines");
	cat_Bounce->CreateParamBool(&IsCheckpointVO, "Checkpoint Voicelines");
	cat_Bounce->CreateParamBool(&IsTornadoVO, "Crisis City Tornado");
	cat_Bounce->CreateParamBool(&IsFootTapSFX, "Foot Tapping SFX");

	parameterGroup->Flush();

	originalInitializeApplicationParams(This);
}

EXPORT void Init()
{
	INIReader reader("mod.ini");
	std::string SelectModel = reader.Get("Main", "IncludeDir1", "disk_swa");
	//if (SelectModel == "disk_swa")
	//	IsSWAMDL = true;
	//else
	//	IsSWAMDL = false;
	IsSWAMDL = SelectModel == "disk_swa";
	IsIdleChatVO = reader.GetBoolean("Option", "IsIdleChatVO", IsIdleChatVO);
	IsRankChatVO = reader.GetBoolean("Option", "IsRankChatVO", IsRankChatVO);
	IsBossTauntVO = reader.GetBoolean("Option", "IsBossTauntVO", IsBossTauntVO);
	IsTransRankVO = reader.GetBoolean("Option", "IsTransRankVO", IsTransRankVO);
	IsSuperYellVO = reader.GetBoolean("Option", "IsSuperYellVO", IsSuperYellVO);
	IsQuickstepVO = reader.GetBoolean("Option", "IsQuickstepVO", IsQuickstepVO);
	IsTrickFinishVO = reader.GetBoolean("Option", "IsTrickFinishVO", IsTrickFinishVO);
	IsTornadoVO = reader.GetBoolean("Option", "IsTornadoVO", IsTornadoVO);
	IsFootTapSFX = reader.GetBoolean("Option", "IsFootTapSFX", IsFootTapSFX);
	if (IsSWAMDL) //Fix stray polys on unleashed skeleton
	{
		//Right Mouth
		WRITE_MEMORY(0x015E8FB4, const char*, "Jaw_LT")
		WRITE_MEMORY(0x015E8FBC, const char*, "Lip_C_LT")
		WRITE_MEMORY(0x015E8FCC, const char*, "Lip_L_LT")
		WRITE_MEMORY(0x015E8FD4, const char*, "Lip_R_LT")
		//Left Mouth
		WRITE_MEMORY(0x015E8FFC, const char*, "Jaw_LT1")
		WRITE_MEMORY(0x015E9004, const char*, "Lip_C_LT1")
		WRITE_MEMORY(0x015E9014, const char*, "Lip_L_LT1")
		WRITE_MEMORY(0x015E901C, const char*, "Lip_R_LT1")
	}
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(EnterRunQuickStep);
	INSTALL_HOOK(ProcMsgNotifyLapTimeHud);
	INSTALL_HOOK(CHudResultStart);
	INSTALL_HOOK(RankQuote_CStateGoalFadeBeforeBegin);
	INSTALL_HOOK(RankQuote_ChangeRank);
	INSTALL_HOOK(RankQuote_ShowRank);
	WRITE_MEMORY(0x016C73D8, void*, CscGenericTornadoNull);
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(LoadArchive);
	INSTALL_HOOK(LoadArchiveList);
	INSTALL_HOOK(InitializeApplicationParams);

	if (Common::IsModEnabled("Unleashed Project") || Common::IsModEnabled("Unleashed Project Encore"))
		IsUnleashedProject = true;

	if (Common::IsModEnabled("Shivery Mountainsides"))
		IsShiveryMountain = true;

	if (Common::IsModEnabled("Water Palace") || Common::IsModEnabled("Water Palace Revamped"))
		IsWaterPalace = true;

	if (Common::IsModEnabled("Sunset Heights")
		|| Common::IsModEnabled("Sonic Melponterations")
		|| Common::IsModEnabled("Metropolis")
		|| Common::IsModEnabled("CrashedCoveZone")
		|| Common::IsModEnabled("CamelotCastle")
		|| Common::IsModEnabled("EggmanLand")
		|| Common::IsModEnabled("Aquarium Park")
		|| Common::IsModEnabled("Crisis City '06"))
		IsGHZStageMod = true;

	if (Common::IsModEnabled("Mystic Cave")
		|| Common::IsModEnabled("EggmanLand"))
		IsCPZStageMod = true;

	if (Common::IsModEnabled("Grand Metropolis") || Common::IsModEnabled("Hang Castle"))
		IsSSZStageMod = true;

	if (Common::IsModEnabled("GC Galaxy Release 1") || Common::IsModEnabled("GameCube Galaxy"))
		IsEUCStageMod = true;

	if (Common::IsModEnabled("Quartz Cryolite"))
		IsPLAStageMod = true;

	if (Common::IsModEnabled("EggmanLand"))
		IsPAMStageMod = true;

	if (GetModuleHandle(L"STH2006Project.dll"))
		Is06Project = true;

	//if (Common::IsModEnabled("Trans Fem Tails"))
	//{
	//	WRITE_MEMORY(0x00588D6E, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x00589AD0, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x0058A5D0, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x00CF64E9, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x010608E4, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x010856F6, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x011717D3, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x01172904, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x01172C46, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x0117450D, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x01175F9D, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x01177835, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x0117890D, char*, "CollectionRoomTFTails_Cnv")
	//	WRITE_MEMORY(0x0117890D, char*, "CollectionRoomTFTails_Cnv")
	//}
}