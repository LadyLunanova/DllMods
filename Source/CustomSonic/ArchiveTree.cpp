#include "ArchiveTree.h"



HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
    constexpr std::string_view str =
        "  <Node>"
        "    <Name>ui_customize</Name>"
        "    <Archive>ui_customize</Archive>"
        "    <Order>0</Order>"
        "    <DefAppend>ui_customize</DefAppend>"
        "    <Node>"
        "      <Name>SystemCommon</Name>"
        "      <Archive>SystemCommon</Archive>"
        "      <Order>0</Order>"
        "    </Node>"
        "  </Node>";

    const size_t newSize = size + str.size();
    const std::unique_ptr<char[]> buffer = std::make_unique<char[]>(newSize);
    memcpy(buffer.get(), data, size);

    char* insertionPos = strstr(buffer.get(), "<Include>");

    memmove(insertionPos + str.size(), insertionPos, size - (size_t)(insertionPos - buffer.get()));
    memcpy(insertionPos, str.data(), str.size());

    bool result;
    {
        result = originalParseArchiveTree(A1, buffer.get(), newSize, database);
    }

    return result;
}

void ArchiveTree::applyPatches()
{
    //INSTALL_HOOK(ParseArchiveTree);
}
