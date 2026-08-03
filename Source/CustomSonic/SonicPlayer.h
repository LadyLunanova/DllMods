#pragma once

#include "CustomSonicAPI.h"

using namespace CustomSonicAPI;

class InstallSonicPlayer
{
public:
	static void applyPatches();
};

void MsgWildFire(int Enabled);
void MsgJumpBall(SelectJumpBallType BallType);
