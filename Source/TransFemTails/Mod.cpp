
bool IsTransSonic = false;

HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>ev031</Name>\n";
		stream << "    <Archive>evTails</Archive>\n";
		stream << "  </DefAppend>\n";

		//stream << "  <DefAppend>\n";
		//stream << "    <Name>pam_cmn</Name>\n";
		//stream << "    <Archive>FEMpam</Archive>\n";
		//stream << "  </DefAppend>\n";

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
	if (strstr(name.c_str(), "evTails") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority
	if (strstr(name.c_str(), "FEM") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchive(This, Edx, A3, A4, name, archiveInfo, A7, A8);
}

HOOK(void, __fastcall, LoadArchiveList, 0x69C270, void* This, void* Edx, void* A3, void* A4, const hh::base::CSharedString& name, void* archiveInfo)
{
	if (strstr(name.c_str(), "evTails") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority
	if (strstr(name.c_str(), "FEM") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchiveList(This, Edx, A3, A4, name, archiveInfo);
}

EXPORT void Init()
{
	INSTALL_HOOK(ParseArchiveTree);
	INSTALL_HOOK(LoadArchive);
	INSTALL_HOOK(LoadArchiveList);
}

extern "C" __declspec(dllexport) void PostInit(ModInfo * mods)
{
	// check for loaded mods
	for (Mod* mod : *mods->ModList)
	{
		//if (!_stricmp(mod->Name, "Unleashed Project"))
		//{
		//	IsUnleashedProject = true;
		//}
		if (_stricmp(mod->Name, "Trans Fem Sonic"))
		{
			WRITE_MEMORY(0x00588D6E, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x00589AD0, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x0058A5D0, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x00CF64E9, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x010608E4, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x010856F6, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x011717D3, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x01172904, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x01172C46, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x0117450D, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x01175F9D, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x01177835, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x0117890D, char*, "CollectionRoomTFTails_Cnv")
			WRITE_MEMORY(0x0117890D, char*, "CollectionRoomTFTails_Cnv")
		}
		printf("%s", mod->Name);
		printf("\n");
	}
}