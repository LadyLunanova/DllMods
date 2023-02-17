#include "InstallSetup.h"

//Setup
std::mutex mutex;
std::map<boost::shared_ptr<hh::mr::CModelData>, std::vector<boost::shared_ptr<hh::mr::CNodeGroupModelData>>> models;
void ItemVisibilityHandler(const boost::shared_ptr<hh::mr::CModelData>& model, std::vector<boost::shared_ptr<hh::mr::CNodeGroupModelData>>& nodeGroupModels);
void traverse(hh::mr::CRenderable* renderable);
bool IsGensRT = false;

static void loadArchiveDatabase()
{
	database = hh::db::CDatabase::CreateDatabase();
	auto& loader = Sonic::CApplicationDocument::GetInstance()->m_pMember->m_spDatabaseLoader;

	const std::string base = "ui/ui_customize";
	//const std::string base = "ui/" + std::string(stage.name);
	const std::string ar = base + ".ar";
	const std::string arl = base + ".arl";

	loader->CreateArchiveList(ar.c_str(), arl.c_str(), { 200, 5 });

	loader->LoadArchiveList(database, arl.c_str());
	loader->LoadArchive(database, ar.c_str(), { -10, 5 }, false, false);
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

		//const auto pair2 = meshes.find(out_spModelData);
		//if (pair2 == meshes.end())
		//	meshes[out_spModelData] = {};
		
	}

	return result;
}

//Handle Files
HOOK(bool, __stdcall, ParseSonicArchiveTree, 0xD4C8E0, void* A1, char* data, const size_t size, void* database)
{
	std::string str;
	{
		std::stringstream stream;

		stream << "  <DefAppend>\n";
		stream << "    <Name>Sonic</Name>\n";
		stream << "    <Archive>SonicVFX</Archive>\n";
		stream << "  </DefAppend>\n";

		//stream << "  <DefAppend>\n";
		//stream << "    <Name>SonicActionCommonHud</Name>\n";
		//stream << "    <Archive>FittingRoom</Archive>\n";
		//stream << "  </DefAppend>\n";

		//stream << "  <DefAppend>\n";
		//stream << "    <Name>pam_cmn</Name>\n";
		//stream << "    <Archive>FittingRoom</Archive>\n";
		//stream << "  </DefAppend>\n";

		//stream << "  <DefAppend>\n";
		//stream << "    <Name>pam000</Name>\n";
		//stream << "    <Archive>FittingRoom</Archive>\n";
		//stream << "  </DefAppend>\n";

		//stream << "  <DefAppend>\n";
		//stream << "    <Name>pam001</Name>\n";
		//stream << "    <Archive>FittingRoom</Archive>\n";
		//stream << "  </DefAppend>\n";

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

		//if (IsGensRT == true)
		//{
		//	stream << "  <DefAppend>\n";
		//	stream << "    <Name>Sonic</Name>\n";
		//	stream << "    <Archive>RTXSonic</Archive>\n";
		//	stream << "  </DefAppend>\n";
		//}

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
		result = originalParseSonicArchiveTree(A1, buffer.get(), newSize, database);
	}

	return result;
}
HOOK(void, __fastcall, LoadArchive, 0x69AB10, void* This, void* Edx, void* A3, void* A4, const hh::base::CSharedString& name, void* archiveInfo, void* A7, void* A8)
{
	if (strstr(name.c_str(), "RTX") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchive(This, Edx, A3, A4, name, archiveInfo, A7, A8);
}
HOOK(void, __fastcall, LoadArchiveList, 0x69C270, void* This, void* Edx, void* A3, void* A4, const hh::base::CSharedString& name, void* archiveInfo)
{
	if (strstr(name.c_str(), "RTX") != nullptr)
		(*(int32_t*)archiveInfo) += 0x0BADF00D; // Priority

	return originalLoadArchiveList(This, Edx, A3, A4, name, archiveInfo);
}

//OnFrame
EXPORT void OnFrame()
{
	////Model Visibility////

	if (Sonic::CApplicationDocument::GetInstance() && database != nullptr)
		loadArchiveDatabase();

	if (Sonic::CGameDocument::GetInstance())
	{
		for (auto& [symbol, renderable] : Sonic::CGameDocument::GetInstance()->GetWorld()->m_pMember->m_spRenderScene->m_BundleMap)
			traverse(renderable.get());
	}

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

void InstallSetup::applyPatches()
{
	//INIReader reader("CustomizeSetting.ini");
	WRITE_MEMORY(0x01271FD1, char*, "ef_null"); // Disable original game's jumpball creation
	WRITE_MEMORY(0x01271DC1, char*, "ef_null"); // Disable original game's super jumpball creation
	INSTALL_HOOK(CMirageDatabaseWrapperGetModelData);
}

void InstallSetup::applyPostPatches()
{
	if (GetModuleHandle(TEXT("GenerationsRaytracing.X86.dll")) != nullptr)
		IsGensRT = true;
	INSTALL_HOOK(ParseSonicArchiveTree);
	//INSTALL_HOOK(LoadArchive);
	//INSTALL_HOOK(LoadArchiveList);
}