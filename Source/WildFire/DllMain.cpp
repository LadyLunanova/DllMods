BOOL WINAPI DllMain(_In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

// Original code by Brianuuu: https://github.com/brianuuu
// Original code by Skyth: https://github.com/blueskythlikesclouds
namespace Hedgehog::Mirage
{
	class CMatrixNodeSingleElementNode
	{
	public:
		CMatrixNode m_node;
		Eigen::Matrix4f local;
		Eigen::Matrix4f world;
	};
}
using SharedPtrTypeless = boost::shared_ptr<void>;
namespace Common
{
	static void* fCGlitterCreate
	(
		void* pContext,
		SharedPtrTypeless& handle,
		void* pMatrixTransformNode,
		Hedgehog::Base::CSharedString const& name,
		uint32_t flag
	)
	{
		static void* const pCGlitterCreate = (void*)0xE73890;
		__asm
		{
			push    flag
			push    name
			push    pMatrixTransformNode
			mov     eax, pContext
			mov     esi, handle
			call[pCGlitterCreate]
		}
	}
	static void fCGlitterEnd
	(
		void* pContext,
		SharedPtrTypeless& handle,
		bool instantStop
	)
	{
		static void* const pCGlitterEnd = (void*)0xE72650;
		static void* const pCGlitterKill = (void*)0xE72570;
		__asm
		{
			mov     eax, [handle]
			mov     ebx, [eax + 4]
			push    ebx
			test	ebx, ebx
			jz		noIncrement
			mov		edx, 1
			add		ebx, 4
			lock xadd[ebx], edx

			noIncrement :
			mov     ebx, [eax]
				push    ebx
				mov     eax, pContext
				cmp     instantStop, 0
				jnz     jump
				call[pCGlitterEnd]
				jmp     end

				jump :
			call[pCGlitterKill]

				end :
		}
	}
	inline void SonicContextRequestLocusEffect(Sonic::Player::CPlayerSpeedContext* pSonicContext)
	{
		// 1 seems to not stop? Force it to be 0
		WRITE_MEMORY(0xE178E5, uint32_t, 0);

		struct MsgRequestLocusEffect
		{
			INSERT_PADDING(0x10);
			uint32_t flag;
		};
		MsgRequestLocusEffect message{};
		message.flag = 0;

		FUNCTION_PTR(int, __thiscall, processMsgRequestLocusEffect, 0xE178D0, void* This, void* pMessage);
		//void* player = *(void**)((uint32_t)pSonicContext + 0x110);
		processMsgRequestLocusEffect(pSonicContext->m_pPlayer, &message);
	}
}
namespace Eigen
{
	template<typename Scalar>
	Matrix<Scalar, 4, 4> CreatePerspectiveMatrix(const Scalar fieldOfView, const Scalar aspectRatio, const Scalar zNear, const Scalar zFar)
	{
		const Scalar yScale = (Scalar)1 / std::tan(fieldOfView / (Scalar)2);
		const Scalar xScale = yScale / aspectRatio;

		Matrix<Scalar, 4, 4> matrix;

		matrix <<
			xScale, 0, 0, 0,
			0, yScale, 0, 0,
			0, 0, -(zFar + zNear) / (zFar - zNear), -2 * zNear * zFar / (zFar - zNear),
			0, 0, -1, 0;

		return matrix;
	}
}

float TimeBreakStartYPressDeadzone = 0.0f;

SharedPtrTypeless WildfireVfxHandle; //Fire VFX handler

void SpawnFireParticle(Sonic::Player::CPlayer* player)
{
	auto ChestNode = player->m_spCharacterModel->GetNode("Spine1"); //Set up Chest bone matrix for VFX
	Eigen::Affine3f affine;
	affine = ChestNode->world;

	if (!WildfireVfxHandle)
		Common::fCGlitterCreate(player->m_spContext.get(), WildfireVfxHandle, &ChestNode, "ef_ch_sng_wildfire", 1);  //Create Fire VFX
}

void KillFireParticle(Sonic::Player::CPlayer* player)
{
	if (!WildfireVfxHandle)
		return;

	Common::fCGlitterEnd(player->m_spContext.get(), WildfireVfxHandle, true);
	WildfireVfxHandle = nullptr;
}

HOOK(void, __fastcall, CPlayerSpeedUpdate, 0xE6BF20, Sonic::Player::CPlayerSpeed* This, void* _, const hh::fnd::SUpdateInfo& updateInfo)
{
	auto sonic = This->GetContext();
	//auto ChestNode = This->m_spCharacterModel->GetNode("Spine1"); //Set up Chest bone matrix for VFX
	//Eigen::Affine3f affine;
	//affine = ChestNode->world;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool HoldY = input.IsDown(Sonic::eKeyState_Y);
	bool PressY = input.IsTapped(Sonic::eKeyState_Y);

	//sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableBoost] = 1;
	//sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_DisableAirBoost] = 1;

	//printf("%d", TimeBreakStartYPressDeadzone);
	//printf("\n");

	originalCPlayerSpeedUpdate(This, _, updateInfo);
	SpawnFireParticle(This);
}

HOOK(void, __fastcall, MsgRestartStage, 0xE76810, Sonic::Player::CPlayer* This, void* Edx, hh::fnd::Message& message)
{
	KillFireParticle(This);
	return originalMsgRestartStage(This, Edx, message);
}

void __fastcall SonicObjectRemoveCallback(Sonic::CGameObject* This, void*, Sonic::CGameDocument* pGameDocument)
{
	printf("Sonic Removed\n");
	printf("Sonic Removed\n");
	printf("Sonic Removed\n");
	printf("Sonic Removed\n");
	printf("Sonic Removed\n");
}

void __fastcall CSonicRemoveCallback(Sonic::Player::CPlayer* This, void* Edx, void* A1)
{
	KillFireParticle(This);
}

HOOK(void, __fastcall, TimeBreakStart, 0x0111AF10, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;

	TimeBreakStartYPressDeadzone = 5;
	sonic->m_ChaosEnergy -= 7;
	originalTimeBreakStart(This);
}

HOOK(void, __fastcall, TimeBreakUpdate, 0x0111B030, hh::fnd::CStateMachineBase::CStateBase* This)
{
	auto sonic = (Sonic::Player::CPlayerSpeedContext*)This->m_pContext;
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool HoldY = input.IsDown(Sonic::eKeyState_Y);
	bool ReleaseY = input.IsReleased(Sonic::eKeyState_Y);
	bool PressY = input.IsTapped(Sonic::eKeyState_Y);

	//sonic->m_Velocity.y() = 10;
	sonic->m_ChaosEnergy -= 0.22;
	TimeBreakStartYPressDeadzone--;

	if (sonic->m_ChaosEnergy <= 0.0 || (PressY && TimeBreakStartYPressDeadzone < 1))
	{
		//sonic->m_pStateFlag->m_Flags[sonic->eStateFlag_InvokeTimeBreak] = 0;
		originalTimeBreakUpdate(This);
	}

	//originalTimeBreakUpdate(This);
}

HOOK(void, __fastcall, CHudSonicStageDelayProcessImp, 0x109A8D0, Sonic::CGameObject* This)
{
	originalCHudSonicStageDelayProcessImp(This);
	auto sonic = Sonic::Player::CPlayerSpeedContext::GetInstance();
	auto input = Sonic::CInputState::GetInstance()->GetPadState();
	bool PressY = input.IsTapped(Sonic::eKeyState_Y);
	size_t& flags = ((size_t*)This)[151];

	//if (sonic->m_ChaosEnergy <= 0.0)
	//{
	//	flags |= ~(0x40);
	//}
	//else
	//{
	//	flags &= ~(0x40);
	//}
}

HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicWildFire</Archive>\n";
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

EXPORT void Init()
{
	WRITE_MEMORY(0x16D4B4C, void*, SonicObjectRemoveCallback);
	WRITE_MEMORY(0x16D4B4C, void*, CSonicRemoveCallback);
	WRITE_NOP(0x0111B04E, 6); //Remove timer check for time break skill
	//MessageBoxA(nullptr, "AttachToProcess", "temp", MB_OK);
	INSTALL_HOOK(CPlayerSpeedUpdate);
	INSTALL_HOOK(MsgRestartStage);
	INSTALL_HOOK(TimeBreakStart);
	INSTALL_HOOK(TimeBreakUpdate);
	//INSTALL_HOOK(CHudSonicStageDelayProcessImp);
	INSTALL_HOOK(ParseArchiveTree);
}