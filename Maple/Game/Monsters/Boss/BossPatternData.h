#pragma once

#include "Common/Vector3D.h"

struct FBossBladePatternData
{
	int32 PoolMaxCount    = 15;
						  
	int32 SpawnMinCount   = 5;
						  
	int32 SpawnMaxCount   = 8;
						  
	int32 ResourceCount   = 3;
						  
	//int32 SpawnFrame      = 34;
						  
	float SpawnMaxRadius  = 55.f;

	//FVector3D SpawnOffset = FVector3D(23.f, 288.5f, 0.f); // 기존 (0, -100, 0)
};

struct FBossBladeStateData
{
	std::string CastAnimation;

	std::string IdleAnimation;

	int32 SpawnFrame = 0;

	FVector3D SpawnOffset = FVector3D::Zero;
};

struct FBossFlowerPatternData
{
	int32 PoolMaxCount      = 8;
						    
	int32 FirstSlotIndex    = 1;
						    
	int32 LastSlotIndex     = 8;

	int32 MediumSmallWeight = 33;

	int32 MediumWeight      = 45;
					        
	int32 LargeWeight       = 18;

	int32 ExtraLargeWeight  = 4;

	float RotationMin       = 20.f;
					        
	float RotationMax       = 70.f;

	float SpawnJitterRatio  = 0.15f;
};

struct FBossPatternAreaData
{
	int32 SlotCount  = 10;

	float LeftBound  = 0.f;

	float RightBound = 0.f;

	float GroundY    = 0.f;

	std::vector<FVector3D> GroundSpawnPositions;
};

struct FBossMushroomPatternData
{
	int32 PoolMaxCount = 2;

	int32 SpawnFrame = 18;

	float SpawnOffsetX = 170.f;
};

struct FBossPhase1GolemPatternData
{
	int32 PoolMaxCount   = 14;

	int32 SpawnCount     = 2;

	int32 FirstSlotIndex = 1;

	int32 LastSlotIndex  = 8;

	int32 SpawnFrame     = 18;

	float FallDuration   = 0.2f;
};

struct FBossPhase1GolemFallData
{
	float StartY     = 0.f;

	float GroundY    = 0.f;

	float Duration   = 0.f;

	float ElapsedTime = 0.f;
};

struct FBossTeleportPatternData
{
	int32 EffectStartFrame = 17;
};

struct FBossPatternNoticeData
{
	float FadeInTime = 0.1f;

	float HoldTime = 2.f;

	float FadeOutTime = 0.25f;

	float TotalWidth = 564.f;

	float LeftWidth = 102.f;

	float RightWidth = 32.f;
};

struct FBossDragonPatternData
{
	int32 SpawnFrame = 24;

	int32 BreathCount = 9;

	float AppearanceTime = 3.f;

	float BreathTime = 4.5f;

	float SpawnHeight = 300.f;

	float DragonHeight = 220.f;
};

struct FBoss2DragonPatternData
{
	int32 BreathStartFrame = 20;

	int32 BreathCount = 9;

	float AppearanceTime = 3.f;

	float SweepTime = 2.25f;

	FVector3D CreatePosition = FVector3D(-35.f, -720.f, 0.f);

	FVector3D UpperPosition = FVector3D(-35.f, -1030.f, 0.f);

	FVector3D LowerPosition = FVector3D(-35.f, -1420.5f, 0.f);
};

struct FBoss2LaserData
{
	std::string Animation = "LaserRain.laser";

	std::string RenderLayer = "Monster";

	int32 WarningStartFrame = 5;

	int32 WarningEndFrame = 10;

	int32 ActiveFrame = 11;

	int32 LastFrame = 20;

	float WarningFrameDelay = 0.055f;

	float WarningTime = 1.f;

	float ActiveTime = 0.12f;

	float FadeFrameDelay = 0.07f;

	float WarningOpacity = 0.65f;

	float ActiveOpacity = 1.f;

	float FadeOpacity = 0.35f;
};

struct FBoss2LaserPatternData
{
	int32 PoolMaxCount = 15;

	int32 LaserCount = 15;

	float SpawnInterval = 0.5f;

	float RotationMin = 30.f;

	float RotationMax = 150.f;

	float ReappearDelay = 3.f;
};

struct FBoss2LaserActionData
{
	std::string Animation = "LaserRain.action_no_blank";

	std::string RenderLayer = "BackGround";
};

struct FBoss2LaserStateData
{
	std::string IdleAnimation;

	std::wstring NoticeText;

	FVector3D ActionOffset = FVector3D::Zero;
};