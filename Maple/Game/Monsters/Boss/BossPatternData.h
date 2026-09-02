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

	FVector3D SpawnOffset = FVector3D(23.f, 288.5f, 0.f); // 기존 (0, -100, 0)
};

struct FBossFlowerPatternData
{
	int32 PoolMaxCount = 8;

	int32 FirstSlotIndex = 1;

	int32 LastSlotIndex = 8;

	int32 MediumSmallWeight = 33;

	int32 MediumWeight = 45;

	int32 LargeWeight = 18;

	int32 ExtraLargeWeight = 4;

	float RotationMin = 20.f;

	float RotationMax = 70.f;

	float SpawnJitterRatio = 0.15f;
};

struct FBossPatternAreaData
{
	int32 SlotCount = 10;

	float LeftBound = 0.f;

	float RightBound = 0.f;

	float GroundY = 0.f;

	std::vector<FVector3D> GroundSpawnPositions;
};