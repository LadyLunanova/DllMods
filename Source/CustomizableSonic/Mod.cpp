#include <Common.h>

/*
Meshes:
 0 = Default Shoes
 1 = Glove R
 2 = Body
 3 = Soaps
 4 = Pumas
 5 = Riders Glasses
 6 = Crystal Ring
 7 = Sunglasses
 8 = Scarf
 9 = Soap Lightspeed
10 = Glove L
*/

int ShoeType = 0;
/*
0 = Default
1 = Soaps
2 = Soaps+Lightspd
3 = Pumas
*/

int HeadType = 0;
/*
0 = None
1 = Riders Glasses
2 = Sunglasses
*/

int HandRType = 0;
/*
0 = 
1 = 
2 = 
*/

int HandLType = 0;
/*
0 =
1 =
2 =
*/

bool Scarf = false;


Chao::CSD::RCPtr<Chao::CSD::CProject> rcPlayScreen;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcCustomize;
Chao::CSD::RCPtr<Chao::CSD::CScene> rcCustomBG;
boost::shared_ptr<Sonic::CGameObjectCSD> rcControls;
float offset = 0.0f;
int MenuOption = 0;

HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	auto model = Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel;
	auto ShowDefaultShoes = Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel->m_NodeGroupModels[0];
	auto ShowSoaps =		Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel->m_NodeGroupModels[3];
	auto ShowPumas =		Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel->m_NodeGroupModels[4];
	auto ShowLightspd =		Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel->m_NodeGroupModels[9];
	auto ShowScarf =		Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel->m_NodeGroupModels[8];
	auto ShowCRing =		Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel->m_NodeGroupModels[6];
	auto ShowRiders =		Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel->m_NodeGroupModels[5];
	auto ShowSunGlass =		Sonic::Player::CPlayerSpeedContext::GetInstance()->m_pPlayer->m_spCharacterModel->m_spModel->m_NodeGroupModels[7];

	if (PressedY)
	{
		//model->m_NodeGroupModels[1]->m_Visible = 0;
		//MessageBoxA(nullptr, "You Pressed Y", "Success", MB_OK);
		if (ShoeType < 3)
			ShoeType++;
		else
			ShoeType = 0;
	}

	//switch (ShoeType) //Mesh ID testing
	//{
	//case 0:
	//	model->m_NodeGroupModels[0]->m_Visible = 1;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 1:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 1;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 2:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 1;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 3:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 1;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 4:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 1;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 5:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 1;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 6:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 1;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 7:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 1;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 8:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 1;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 9:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 1;
	//	model->m_NodeGroupModels[10]->m_Visible = 0;
	//	break;
	//case 10:
	//	model->m_NodeGroupModels[0]->m_Visible = 0;
	//	model->m_NodeGroupModels[1]->m_Visible = 0;
	//	model->m_NodeGroupModels[2]->m_Visible = 0;
	//	model->m_NodeGroupModels[3]->m_Visible = 0;
	//	model->m_NodeGroupModels[4]->m_Visible = 0;
	//	model->m_NodeGroupModels[5]->m_Visible = 0;
	//	model->m_NodeGroupModels[6]->m_Visible = 0;
	//	model->m_NodeGroupModels[7]->m_Visible = 0;
	//	model->m_NodeGroupModels[8]->m_Visible = 0;
	//	model->m_NodeGroupModels[9]->m_Visible = 0;
	//	model->m_NodeGroupModels[10]->m_Visible = 1;
	//	break;
	//}

	//Default Shoes visibility
	if (ShoeType == 0) 
	{
		ShowDefaultShoes->m_Visible = 1;
	}
	else
	{
		ShowDefaultShoes->m_Visible = 0;
	}

	//Soap Shoes visibility
	if (ShoeType == 1 || ShoeType == 2)
	{
		ShowSoaps->m_Visible = 1;
	}
	else
	{
		ShowSoaps->m_Visible = 0;
	}

	//Soap Lightspeed visibility
	if (ShoeType == 2)
	{
		ShowLightspd->m_Visible = 1;
	}
	else
	{
		ShowLightspd->m_Visible = 0;
	}

	//Puma Shoes visibility
	if (ShoeType == 3)
	{
		ShowPumas->m_Visible = 1;
	}
	else
	{
		ShowPumas->m_Visible = 0;
	}

	//Scarf visibility
	if (Scarf == true)
	{
		ShowScarf->m_Visible = 1;
	}
	else
	{
		ShowScarf->m_Visible = 0;
	}

	//Crystal Ring visibility
	if (HandLType == 1)
	{
		ShowCRing->m_Visible = 1;
	}
	else
	{
		ShowCRing->m_Visible = 0;
	}

	//Riders Glasses visibility
	if (HeadType == 1)
	{
		ShowRiders->m_Visible = 1;
	}
	else
	{
		ShowRiders->m_Visible = 0;
	}

	//Sunglasses visibility
	if (HeadType == 2)
	{
		ShowSunGlass->m_Visible = 1;
	}
	else
	{
		ShowSunGlass->m_Visible = 0;
	}

	printf("Shoe Type: ");
	printf("%d", ShoeType);
	printf(" ");
	printf("Headwear Type: ");
	printf("%d", HeadType);
	printf(" ");
	printf("Hand L Type: ");
	printf("%d", HandLType);
	printf(" ");
	printf("Hand R Type: ");
	printf("%d", HandRType);
	printf(" ");
	printf("Scarf: ");
	printf("%d", Scarf);
	printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);
}

HOOK(void, __cdecl, InitializeApplicationParams, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CParameterCategory* cat_Bounce = parameterGroup->CreateParameterCategory("Customizable Sonic", "Parameters for customizable Sonic");

	//cat_Bounce->CreateParamInt(&someInt, "Integer");
	//cat_Bounce->CreateParamBool(&someBool, "Bool");
	//cat_Bounce->CreateParamFloat(&someFloat, "Float");

	cat_Bounce->CreateParamTypeList((uint32_t*)&ShoeType, "Shoe Selection", "Select which shoes Sonic should wear.",
		{
			{ "Default", 0},
			{ "Soaps", 1},
			{ "Soaps+Lightspeed", 2},
			{ "Pumas", 3},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&HeadType, "Head Selection", "Select what headwear Sonic should wear.",
		{
			{ "None", 0},
			{ "Riders Glasses", 1},
			{ "Sunglasses", 2},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&HandLType, "Left Hand Selection", "Select what accessory Sonic should wear on his left hand.",
		{
			{ "None", 0},
			{ "Crystal Ring", 1},
			{ "Flame Ring", 2},
		});
	cat_Bounce->CreateParamBool(&Scarf, "Choose if Sonic should wear his scarf.");

	parameterGroup->Flush();

	originalInitializeApplicationParams(This);
}

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

HOOK(void, __fastcall, HUDControlsStart, 0x0042A810, hh::fnd::CStateMachineBase::CStateBase* This)
{
	//return;
	auto cast = (Sonic::CGameObject*)This->GetContextBase();
	Sonic::CCsdDatabaseWrapper wrapper(cast->m_pMember->m_pGameDocument->m_pMember->m_spDatabase.get());
	auto spCsdProject = wrapper.GetCsdProject("ui_pause_control");
	rcPlayScreen = spCsdProject->m_rcProject;
	rcControls = boost::make_shared<Sonic::CGameObjectCSD>(rcPlayScreen, 0.5f, "HUD_Pause", true);
	Sonic::CGameDocument::GetInstance()->AddGameObject(rcControls, "main", cast);

	//rcHowTo = rcPlayScreen->CreateScene("pause_mission");
	rcCustomize = rcPlayScreen->CreateScene("pause_act");
	rcCustomize->SetPosition(0, offset);
	if (!Scarf)
	{
		rcCustomize->SetMotion("zero_life");
		rcCustomize->SetMotionFrame(0.0f);
		rcCustomize->m_MotionSpeed = 0.0f;
		rcCustomize->Update();
	}
	rcCustomize->SetMotion("Intro_Anim");
	rcCustomize->SetMotionFrame(0.0f);
	rcCustomize->m_MotionDisableFlag = false;
	rcCustomize->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	rcCustomize->m_MotionSpeed = 1.0f;
	rcCustomize->Update();

	rcCustomBG = rcPlayScreen->CreateScene("pause_mission");
	rcCustomBG->SetPosition(0, offset);
	rcCustomBG->SetMotion("Intro_Anim");
	rcCustomBG->SetMotionFrame(0.0f);
	rcCustomBG->m_MotionDisableFlag = false;
	rcCustomBG->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
	rcCustomBG->m_MotionSpeed = 1.0f;
	rcCustomBG->Update();
	//originalHUDControlsStart(This);
}

HOOK(void, __fastcall, HUDControlsUpdate, 0x0042AAF0, Sonic::CGameObject* This)
{
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	auto inputState = Sonic::CInputState::GetInstance();
	auto inputPtr = &inputState->m_PadStates[inputState->m_CurrentPadStateIndex];
	bool PressedY = input.IsTapped(Sonic::eKeyState_Y);
	bool PressedX = input.IsTapped(Sonic::eKeyState_X);
	bool PressedB = input.IsTapped(Sonic::eKeyState_B);
	bool PressedA = input.IsTapped(Sonic::eKeyState_A);
	bool PressedLB = input.IsTapped(Sonic::eKeyState_LeftBumper);
	bool PressedRB = input.IsTapped(Sonic::eKeyState_RightBumper);
	bool PressedLT = input.IsTapped(Sonic::eKeyState_LeftTrigger);
	bool PressedRT = input.IsTapped(Sonic::eKeyState_RightTrigger);
	bool PressedUp = input.IsTapped(Sonic::eKeyState_DpadUp) || inputPtr->LeftStickVertical >= 0.5f;
	bool PressedDw = input.IsTapped(Sonic::eKeyState_DpadDown) || inputPtr->LeftStickVertical <= -0.5f;
	static SharedPtrTypeless menuSoundHandle;

	//rcCustomBG->GetNode("txt_box")->SetText(text);

	if (PressedA)
	{
		switch (MenuOption)
		{
		case 0:
			
			rcCustomize->SetMotion("select_Anim_1");
			rcCustomize->SetMotionFrame(0.0f);
			rcCustomize->m_MotionDisableFlag = false;
			rcCustomize->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
			rcCustomize->m_MotionSpeed = 1.0f;
			rcCustomize->Update();
			Common::PlaySoundStatic(menuSoundHandle, 1000005);
			break;
		case 1:
			if (!Scarf)
			{
				Scarf = true;
				rcCustomize->SetMotion("some_life");
				rcCustomize->SetMotionFrame(0.0f);
				rcCustomize->m_MotionSpeed = 0.0f;
				rcCustomize->Update();
				rcCustomize->SetMotion("select_Anim_2");
				rcCustomize->SetMotionFrame(0.0f);
				rcCustomize->m_MotionDisableFlag = false;
				rcCustomize->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				rcCustomize->m_MotionSpeed = 1.0f;
				rcCustomize->Update();
				Common::PlaySoundStatic(menuSoundHandle, 1000005);
			}
			else if (Scarf)
			{
				Scarf = false;
				rcCustomize->SetMotion("zero_life");
				rcCustomize->SetMotionFrame(0.0f);
				rcCustomize->m_MotionSpeed = 0.0f;
				rcCustomize->Update();
				rcCustomize->SetMotion("select_Anim_2");
				rcCustomize->SetMotionFrame(0.0f);
				rcCustomize->m_MotionDisableFlag = false;
				rcCustomize->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
				rcCustomize->m_MotionSpeed = 1.0f;
				rcCustomize->Update();
				Common::PlaySoundStatic(menuSoundHandle, 1000007);
			}
			break;
		case 2:
			break;
		case 3:
			break;
		}
	}

	if (PressedB)
	{
		switch (MenuOption)
		{
		case 0:
			rcCustomize->SetMotion("close_Anim_1");
			break;
		case 1:
			rcCustomize->SetMotion("close_Anim_2");
			break;
		case 2:
			rcCustomize->SetMotion("close_Anim_3");
			break;
		case 3:
			rcCustomize->SetMotion("close_Anim_4");
			break;
		}
		//rcCustomize->SetMotion("close_Anim_1");
		rcCustomize->SetMotionFrame(0.0f);
		rcCustomize->m_MotionDisableFlag = false;
		rcCustomize->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcCustomize->m_MotionSpeed = 1.0f;
		rcCustomize->Update();
		rcCustomBG->SetMotion("close_Anim_1");
		rcCustomBG->SetMotionFrame(0.0f);
		rcCustomBG->m_MotionDisableFlag = false;
		rcCustomBG->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcCustomBG->m_MotionSpeed = 1.0f;
		rcCustomBG->Update();
		Common::PlaySoundStatic(menuSoundHandle, 1000007);
		//FUNCTION_PTR(void, __stdcall, ControlMenuPlayOuttro, 0x010AF250, int hudHowTo);
		//ControlMenuPlayOuttro(*(uint32_t*)((uint32_t)This + 0x24));
		//*(uint8_t*)((uint32_t)This + 44) = 1;
	}
	
	if (PressedUp)
	{
		if (MenuOption == 0)
		{
			rcCustomize->SetMotion("select_Anim_4");
			MenuOption = 3;
		}
		else if (MenuOption == 1)
		{
			rcCustomize->SetMotion("select_Anim_1");
			MenuOption = 0;
		}
		else if (MenuOption == 2)
		{
			rcCustomize->SetMotion("select_Anim_2");
			MenuOption = 1;
		}
		else if (MenuOption == 3)
		{
			rcCustomize->SetMotion("select_Anim_3");
			MenuOption = 2;
		}
		rcCustomize->SetMotionFrame(0.0f);
		rcCustomize->m_MotionDisableFlag = false;
		rcCustomize->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcCustomize->m_MotionSpeed = 1.0f;
		rcCustomize->Update();
		Common::PlaySoundStatic(menuSoundHandle, 1000004);
		//return;
	}

	if (PressedDw)
	{
		if (MenuOption == 0)
		{
			rcCustomize->SetMotion("select_Anim_2");
			MenuOption = 1;
		}
		else if (MenuOption == 1)
		{
			rcCustomize->SetMotion("select_Anim_3");
			MenuOption = 2;
		}
		else if (MenuOption == 2)
		{
			rcCustomize->SetMotion("select_Anim_4");
			MenuOption = 3;
		}
		else if (MenuOption == 3)
		{
			rcCustomize->SetMotion("select_Anim_1");
			MenuOption = 0;
		}
		rcCustomize->SetMotionFrame(0.0f);
		rcCustomize->m_MotionDisableFlag = false;
		rcCustomize->m_MotionRepeatType = Chao::CSD::eMotionRepeatType_PlayOnce;
		rcCustomize->m_MotionSpeed = 1.0f;
		rcCustomize->Update();
		Common::PlaySoundStatic(menuSoundHandle, 1000004);
	}

	if (PressedRB)
	{
		if (ShoeType < 3)
		{
			ShoeType += 1;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
		else if (ShoeType == 3)
		{
			ShoeType = 0;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
	}

	if (PressedLB)
	{
		if (ShoeType > 0)
		{
			ShoeType -= 1;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
		else if (ShoeType == 0)
		{
			ShoeType = 3;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
	}

	if (PressedRT)
	{
		if (HandRType < 3)
		{
			HandRType += 1;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
		else if (HandRType == 3)
		{
			HandRType = 0;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
	}

	if (PressedLT)
	{
		if (HandLType < 3)
		{
			HandLType += 1;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
		else if (HandLType == 3)
		{
			HandLType = 0;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
	}

	if (PressedY)
	{
		if (HeadType < 2)
		{
			HeadType += 1;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
		else if (HeadType == 2)
		{
			HeadType = 0;
			Common::PlaySoundStatic(menuSoundHandle, 1000006);
		}
	}

	if (PressedX && !Scarf)
	{
		Scarf = true;
		Common::PlaySoundStatic(menuSoundHandle, 1000005);
	}
	else if (PressedX && Scarf)
	{
		Scarf = false;
		Common::PlaySoundStatic(menuSoundHandle, 1000007);
	}
	
	printf("Shoe Type: ");
	printf("%d", ShoeType);
	printf(" ");
	printf("Headwear Type: ");
	printf("%d", HeadType);
	printf(" ");
	printf("Hand L Type: ");
	printf("%d", HandLType);
	printf(" ");
	printf("Hand R Type: ");
	printf("%d", HandRType);
	printf(" ");
	printf("Scarf: ");
	printf("%d", Scarf);
	printf(" - Controls Menu");
	printf("\n");
	
	//originalHUDControlsUpdate(This);
}

EXPORT void Init()
{
	//INIReader reader("Mod.ini");
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(InitializeApplicationParams);
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(LoadArchive);
	INSTALL_HOOK(LoadArchiveList);
	INSTALL_HOOK(HUDControlsStart);
	INSTALL_HOOK(HUDControlsUpdate);
	//WRITE_NOP(0x0042AB50, 5);
}