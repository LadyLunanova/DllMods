#include <Common.h>
#include "PlayerTimerFunc.h"
#include "ChaosUI.h"
#include "ChaosActivators.h"

int ChaosTimer = 300;
int CustomChaosTimer = 300;
int RandomizedInt = 0;

//Dynamic Omnis
// Original code by Skyth: https://github.com/blueskythlikesclouds
hh::base::CRefPtr<Sonic::CLocalLight> flashbangPLight;
void ParticleLight(float ColorR, float ColorG, float ColorB, float ColorA, float LightRange)
{
	auto gameDocument = Sonic::CGameDocument::GetInstance();
	if (!gameDocument.get().get() || !gameDocument->m_pMember->m_spLightManager)
		return;

	auto playerContext = Sonic::Player::CPlayerSpeedContext::GetInstance();
	if (!playerContext)
		return;

	const auto& stateName = playerContext->m_pPlayer->m_StateMachine.GetCurrentState()->GetStateName();
	const auto& animName = playerContext->GetCurrentAnimationName();

	if (ChSonicFlashbangEnable)
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		//if (ChFlashbangEnabled)
		//	position += fwd * 0.5f + up * 0.5f;

		if (!flashbangPLight)
			flashbangPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { ColorR, ColorG, ColorB, ColorA }, { 0.0f, 0.0f, 0.0f, LightRange });
		else
			flashbangPLight->SetPosition(position);
	}
	else
		flashbangPLight = nullptr;

}

//OnFrame
EXPORT void OnFrame()
{
	int randomizer = std::rand() % 18;
	RandomizedInt = randomizer;

	printf("%d", RandomizedInt);
	printf("\n");
}

//Sonic Function
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	//---------------------------------------------------
	// Locals/Setup
	//---------------------------------------------------
	//int randomizer = std::rand() % 16;
	auto sonic = This->GetContext();
	auto localVelocity = sonic->m_spMatrixNode->m_Transform.m_Rotation.inverse() * sonic->m_Velocity;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	bool HoldRight = input.IsDown(Sonic::eKeyState_DpadRight);
	bool HoldLeft = input.IsDown(Sonic::eKeyState_DpadLeft);
	bool HoldUp = input.IsDown(Sonic::eKeyState_DpadUp);
	bool HoldDown = input.IsDown(Sonic::eKeyState_DpadDown);
	bool PressedRight = input.IsTapped(Sonic::eKeyState_DpadRight);
	

	//---------------------------------------------------
	// Main Run Code
	//---------------------------------------------------

	ChaosTimer++;

	if (ChaosTimer <= 0)
	{
		ChaosTimer = CustomChaosTimer;
		printf("!!!!!!!-------CHAOS EFFECT-------!!!!!!!"); printf("\n");
		switch (RandomizedInt)
		{
		case 1:
			ChActMoonjump(This);
			break;
		case 2:
			ChActSlowMaxSpd();
			break;
		case 3:
			ChActFlashbang();
			break;
		case 4:
			ChActDrainBoost(This);
			break;
		case 5:
			ChAct69Boost(This);
			break;
		case 6:
			ChActStumble(This);
			break;
		case 7:
			ChAct69Rings(This);
			break;
		case 8:
			ChActDropRings(This);
			break;
		case 9:
			ChActSmallSonic();
			break;
		case 10:
			ChActBigSonic();
			break;
		case 11:
			ChActSlapped(This);
			break;
		case 12:
			ChActDrop(This);
			break;
		case 13:
			ChActHideSonic();
			break;
		case 14:
			ChActBoard(This);
			break;
		case 15:
			ChActInfMaxSpd();
			break;
		case 16:
			ChActKill(This);
			break;
		case 17:
			ChActHurt(This);
			break;
		case 18:
			ChActSonicFrozen();
			break;
		}
	}

	//if (PressedY)
	//{
	//	ChActSonicFrozen();
	//}

	////Timer Functions////
	ChSonicBigScale();
	ChSonicSmallScale();
	ChSonicFlashbang();
	ChSonicBoard(This);
	ChSonicHide(This);
	ChSonicSlowMaxSpd(This);
	ChSonicInfMaxSpd(This);
	ChSonicFrozen(This);


	////Appliers////
	ParticleLight(10000, 10000, 10000, 10000, 1000000000); //Flashbang
	Common::SetPlayerModelScale(ChSonicModelScale);

	//---------------------------------------------------
	// Console Stuff
	//---------------------------------------------------

	//printf(This->m_StateMachine.GetCurrentState()->GetStateName().c_str());
	//printf(sonic->GetCurrentAnimationName().c_str());
	//printf("%d", flags->m_Flags[sonic->eStateFlag_Boost]);
	//printf("%d", sonic->m_Field164);
	//printf("%f,%f,%f",sonic->m_Velocity.x(),sonic->m_Velocity.y(),sonic->m_Velocity.z());
	//printf("%f",GetPos);
	//printf("%d",sonic->m_VelocityDirty);
	//printf(" - ");
	//printf("%f",sonic->m_ChaosEnergy);
	//printf("%d",sonic->m_Is2DMode);
	//printf(" - ");
	//printf("%d", ChaosTimer);
	//printf("\n");

	//---------------------------------------------------
	// Call Original Player Update
	//---------------------------------------------------

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}

//Handle Archives
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>SonicActionCommonHud</Name>\n";
		stream << "    <Archive>ChaosUI</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam_cmn</Name>\n";
		stream << "    <Archive>ChaosUI</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>PhantomRocket</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicBoard</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicClassic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicDiving</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicMission</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicPam</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicPla</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicSph</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicSsh</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicSsz</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicWater</Archive>\n";
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
	if (strstr(name.c_str(), "Sonic") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchive(This, Edx, A3, A4, name, archiveInfo, A7, A8);
}
HOOK(void, __fastcall, LoadArchiveList, 0x69C270, void* This, void* Edx, void* A3, void* A4, const hh::base::CSharedString& name, void* archiveInfo)
{
	if (strstr(name.c_str(), "Sonic") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchiveList(This, Edx, A3, A4, name, archiveInfo);
}

EXPORT void Init()
{
	ChaosUI::applyPatches();
	INIReader reader("ChaosConfig.ini");
	CustomChaosTimer = reader.GetInteger("Main", "CustomChaosTimer", CustomChaosTimer);
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(LoadArchive);
	INSTALL_HOOK(LoadArchiveList);
	// Ignore D-pad input for Sonic's control
	WRITE_JUMP(0xD97B56, (void*)0xD97B9E);
}