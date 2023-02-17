#include <Common.h>
#include "InstallSetup.h"
#include "InstallCustomUI.h"
#include "ArchiveTree.h"
#include "CpkBinder.h"
#include "SonicVFX.h"

//Declare
void ReadINI();
void ReadConfig();

//Init
EXPORT void Init()
{
	InstallSetup::applyPatches();
	InstallCustomUI::applyPatches();
	InstallSonicVFX::applyPatches();
	ArchiveTree::applyPatches();
	CpkBinder::applyPatches();
	ReadINI();
	ReadConfig();
}

//Init
EXPORT void PostInit()
{
	InstallSetup::applyPostPatches();
}