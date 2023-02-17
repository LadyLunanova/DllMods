#include <Common.h>
#include "InstallSetup.h"
#include "InstallCustomUI.h"
<<<<<<< HEAD
#include "ArchiveTree.h"
#include "CpkBinder.h"
#include "SonicVFX.h"
=======
#include "WildFireVFX.h"
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9

//Declare
void ReadINI();
void ReadConfig();

//Init
EXPORT void Init()
{
	InstallSetup::applyPatches();
	InstallCustomUI::applyPatches();
<<<<<<< HEAD
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
=======
	InstallWildFireVFX::applyPatches();
	ReadINI();
	ReadConfig();
>>>>>>> e139a2e196ae21cfc27f2a98d570497a3ee761d9
}