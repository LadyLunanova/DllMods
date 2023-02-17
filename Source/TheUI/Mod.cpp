#include "TheUINCP.h"

//Handle Archives
HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>SonicActionCommonHud</Name>\n";
		stream << "    <Archive>TheUI</Archive>\n";
		stream << "  </DefAppend>\n";

		stream << "  <DefAppend>\n";
		stream << "    <Name>pam_cmn</Name>\n";
		stream << "    <Archive>TheUI</Archive>\n";
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
	TheUINCP::applyPatches();
	INSTALL_HOOK(ParseArchiveTree);
}