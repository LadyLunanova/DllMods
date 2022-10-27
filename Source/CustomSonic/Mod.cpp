#include <Common.h>
#include "InstallSetup.h"
#include "InstallCustomUI.h"
#include "WildFireVFX.h"

//Declare
void ReadINI();
void ReadConfig();

//Init
EXPORT void Init()
{
	InstallSetup::applyPatches();
	InstallCustomUI::applyPatches();
	InstallWildFireVFX::applyPatches();
	ReadINI();
	ReadConfig();
}