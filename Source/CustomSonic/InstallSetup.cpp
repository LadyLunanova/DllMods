#include "InstallSetup.h"

//Setup
std::mutex mutex;
std::map<boost::shared_ptr<hh::mr::CModelData>, std::vector<boost::shared_ptr<hh::mr::CNodeGroupModelData>>> models;
void ItemVisibilityHandler(const boost::shared_ptr<hh::mr::CModelData>& model, std::vector<boost::shared_ptr<hh::mr::CNodeGroupModelData>>& nodeGroupModels);
void traverse(hh::mr::CRenderable* renderable);
bool IsGensRT = false;

HOOK(void*, __fastcall, CMirageDatabaseWrapperGetModelData, hh::mr::fpCMirageDatabaseWrapperGetModelData, Hedgehog::Mirage::CMirageDatabaseWrapper* This, void* Edx, boost::shared_ptr<Hedgehog::Mirage::CModelData>& out_spModelData, const Hedgehog::Base::CSharedString& in_rName, size_t in_Unknown)
{
	void* result = originalCMirageDatabaseWrapperGetModelData(This, Edx, out_spModelData, in_rName, in_Unknown);

	if (out_spModelData && (strstr(in_rName.c_str(), "chr_Sonic_HD") || strstr(in_rName.c_str(), "chr_SuperSonic_HD"))) //chr_spin_SA1
	{
		std::lock_guard lock(mutex);

		const auto pair = models.find(out_spModelData);
		if (pair == models.end())
			models[out_spModelData] = {};
	}

	return result;
}

//OnFrame
EXPORT void OnFrame()
{
	//Model Visibility////

	if (Sonic::CGameDocument::GetInstance())
	{
		for (auto& [symbol, renderable] : Sonic::CGameDocument::GetInstance()->GetWorld()->m_pMember->m_spRenderScene->m_BundleMap)
			traverse(renderable.get());
	}

	//ItemVisibilityHandler(it->first, it->second);

	std::lock_guard lock(mutex);
	for (auto it = models.begin(); it != models.end();)
	{
		if (it->first.unique())
		{
			it = models.erase(it);
			printf("NO SONIC MODEL\n");
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
	WRITE_MEMORY(0x01271FD1, char*, "ef_null"); // Disable original game's jumpball creation
	WRITE_MEMORY(0x01271DC1, char*, "ef_null"); // Disable original game's super jumpball creation
	INSTALL_HOOK(CMirageDatabaseWrapperGetModelData);
}
void InstallSetup::applyPostPatches()
{
	if (GetModuleHandle(TEXT("GenerationsRaytracing.X86.dll")) != nullptr)
		IsGensRT = true;
}