#include <Common.h>
#include "UpdateDirector.h"
#include "InstallCustomUI.h"
#include "WildFireVFX.h"

//Setup
std::mutex mutex;
std::map<boost::shared_ptr<hh::mr::CModelData>, std::vector<boost::shared_ptr<hh::mr::CNodeGroupModelData>>> models;
void ItemVisibilityHandler(const boost::shared_ptr<hh::mr::CModelData>& model, std::vector<boost::shared_ptr<hh::mr::CNodeGroupModelData>>& nodeGroupModels);
bool WildFireActive = false;
void WildFireEnable(int Enabled)
{
	if (Enabled == 1)
		WildFireActive = true;
	else
		WildFireActive = false;
}
void SpawnFireParticle(Sonic::Player::CPlayer* player);
void KillFireParticle(Sonic::Player::CPlayer* player);
void ReadINI();

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

	if (playerContext->StateFlag(eStateFlag_InvokeSuperSonic))
	{
		auto position = playerContext->m_spMatrixNode->m_Transform.m_Position;

		const auto& up = playerContext->m_ModelUpDirection;
		const auto fwd = playerContext->m_spMatrixNode->m_Transform.m_Rotation * hh::math::CVector::UnitZ();

		if (stateName == "Sliding" || playerContext->StateFlag(eStateFlag_Boost))
			position += fwd * 0.5f + up * 0.25f;

		if (!superPLight)
			superPLight = gameDocument->m_pMember->m_spLightManager->AddLocalLight(position, { 1.0f, 0.7f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 5.0f });
		else
			superPLight->SetPosition(position);
	}
	else
		superPLight = nullptr;
}

//Handle Sonic
HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
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

	CustomParticleLight(0.0, 0.4, 2.0, 1.0, 4.2, This);
}
HOOK(void*, __fastcall, CMirageDatabaseWrapperGetModelData, hh::mr::fpCMirageDatabaseWrapperGetModelData, Hedgehog::Mirage::CMirageDatabaseWrapper* This, void* Edx, boost::shared_ptr<Hedgehog::Mirage::CModelData>& out_spModelData, const Hedgehog::Base::CSharedString& in_rName, size_t in_Unknown)
{
	void* result = originalCMirageDatabaseWrapperGetModelData(This, Edx, out_spModelData, in_rName, in_Unknown);

	if (out_spModelData && (strstr(in_rName.c_str(), "chr_Sonic_HD") || strstr(in_rName.c_str(), "chr_SuperSonic_HD")))
	{
		std::lock_guard lock(mutex);

		const auto pair = models.find(out_spModelData);
		if (pair == models.end())
			models[out_spModelData] = {};
	}

	return result;
}

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationParams, 0x00D65180, Sonic::CParameterFile* This)
{
	//boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	//This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	//Sonic::CParameterCategory* cat_Bounce = parameterGroup->CreateParameterCategory("Customizable Sonic", "Parameters for customizable Sonic");

	//cat_Bounce->CreateParamBool(&ConfigDecoEnable, "Show the fitting room UI decorations.");
	//cat_Bounce->CreateParamBool(&EyelidSkin, "Use skin eyelids.");

	//parameterGroup->Flush();

	originalInitializeApplicationParams(This);
}

//Handle Files
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicWildFire</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>SonicActionCommonHud</Name>\n";
		stream << "    <Archive>FittingRoom</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam_cmn</Name>\n";
		stream << "    <Archive>FittingRoom</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam000</Name>\n";
		stream << "    <Archive>FittingRoom</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam001</Name>\n";
		stream << "    <Archive>FittingRoom</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>SonicSp</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam000</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>Title</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev021</Name>\n";
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
		stream << "    <Name>ev051</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev053</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev061</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev062</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev071</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev081</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev082</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev091</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev121</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev210</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev220</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev221</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev230</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev240</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev241</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev250</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev251</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev260</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev261</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev270</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev271</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev280</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev281</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev290</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev291</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev310</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev311</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev320</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev321</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev330</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev331</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev340</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev341</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev410</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev411</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev420</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>ev430</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>evt150</Name>\n";
		stream << "    <Archive>evSonic</Archive>\n";
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

	return originalLoadArchive(This, Edx, A3, A4, name, archiveInfo, A7, A8);
}
HOOK(void, __fastcall, LoadArchiveList, 0x69C270, void* This, void* Edx, void* A3, void* A4, const hh::base::CSharedString& name, void* archiveInfo)
{
	if (strstr(name.c_str(), "evSonic") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchiveList(This, Edx, A3, A4, name, archiveInfo);
}

//OnFrame
EXPORT void OnFrame()
{
	////Model Visibility////
	std::lock_guard lock(mutex);

	for (auto it = models.begin(); it != models.end();)
	{
		if (it->first.unique())
		{
			it = models.erase(it);
			printf("SONIC MODEL DESTROYED\n");
		}
		else
		{
			ItemVisibilityHandler(it->first, it->second);
			++it;
		}
	}
}

//Init
EXPORT void Init()
{
	InstallCustomUI::applyPatches();
	InstallWildFireVFX::applyPatches();
	ReadINI();
	INIReader reader("Config.ini");
	//ConfigDecoEnable = reader.GetBoolean("Mod", "ConfigDecoEnable", ConfigDecoEnable);
	//EyelidSkin = reader.GetBoolean("Mod", "EyelidSkin", EyelidSkin);
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(CMirageDatabaseWrapperGetModelData);
	INSTALL_HOOK(InitializeApplicationParams);
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(LoadArchive);
	INSTALL_HOOK(LoadArchiveList);
}