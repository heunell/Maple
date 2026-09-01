#pragma once

#include "Common/Vector3D.h"

struct FBossBladePatternData
{
	int32 PoolMaxCount = 15;

	int32 SpawnMinCount = 5;

	int32 SpawnMaxCount = 8;

	int32 ResourceCount = 3;

	int32 SpawnFrame = 34;

	float SpawnMaxRadius = 55.f;

	FVector3D SpawnOffset = FVector3D(0.f, -100.f, 0.f);
};