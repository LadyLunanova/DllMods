//---------------------------------------------------
// Particle Effects Depends
//---------------------------------------------------
#include <Common.h>
HMODULE hm;


//---------------------------------------------------
// Main Shit
//---------------------------------------------------


HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicEff</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>cmn200</Name>\n";
		stream << "    <Archive>cmn100</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicDiving</Archive>\n";
		stream << "  </DefAppend>\n";
		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicBoard</Archive>\n";
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

//Parameter Editor Options
HOOK(void, __cdecl, InitializeApplicationPlaygroundParams, 0x00D65180, Sonic::CParameterFile* This)
{
	boost::shared_ptr<Sonic::CParameterGroup> parameterGroup;
	This->CreateParameterGroup(parameterGroup, "Luna's Mods", "Parameters for Lady Luna's code mods");
	Sonic::CEditParam* cat_Bounce = parameterGroup->CreateParameterCategory("Lunar Playground", "Parameters for lunar playground");

	cat_Bounce->CreateParamTypeList((uint32_t*)&SonicRedTrailType, "Red Trail Type", "Choose trail type",
		{
			{ "None", 0},
			{ "Permanent", 1},
			{ "Speed", 2},
			{ "Action States", 3},
			{ "Boost", 4},
			{ "Boost and Action States", 5},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&SonicWhiteTrailType, "White Trail Type", "Choose trail type",
		{
			{ "None", 0},
			{ "Permanent", 1},
			{ "Speed", 2},
			{ "Action States", 3},
			{ "Boost", 4},
			{ "Boost and Action States", 5},
		});
	cat_Bounce->CreateParamTypeList((uint32_t*)&SonicBlueTrailType, "Blue Trail Type", "Choose trail type",
		{
			{ "None", 0},
			{ "Permanent", 1},
			{ "Speed", 2},
			{ "Action States", 3},
			{ "Boost", 4},
			{ "Boost and Action States", 5},
		});
	cat_Bounce->CreateParamFloat(&SonicTrailSpeed, "Speed type trail activation speed");
	cat_Bounce->CreateParamFloat(&ChaosEnergyTrickJumpDrain, "Chaos Energy trick jump drain");
	cat_Bounce->CreateParamFloat(&VelocityTrickJumpForce, "Trick jump force");
	cat_Bounce->CreateParamBool(&SonicNoBallJump, "No ball jump");

	parameterGroup->Flush();

	originalInitializeApplicationPlaygroundParams(This);
}

EXPORT void Init()
{
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(InitializeApplicationPlaygroundParams);
	// Ignore D-pad input for Sonic's control
	WRITE_JUMP(0xD97B56, (void*)0xD97B9E);
	//INSTALL_HOOK(InitializeApplicationParams_LUNA);

	//MessageBoxA(nullptr, "No", "Error", 0);

	hm = LoadLibraryA("LunarSharpPlayground.dll");
	if (hm == NULL)
	{
		MessageBoxA(nullptr, "Buttplug: Failed to load DLL, buttplug will not function!", "Error", 0);
		return;
	}
	GetProcAddress(hm, "Main")();
}