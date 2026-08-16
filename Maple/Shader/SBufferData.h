#pragma once
#include "Common/Matrix.h"
#include "Common/Vector2D.h"
#include "Common/Vector3D.h"
#include "Common/Vector4D.h"

struct FTileInstanceData
{
    FVector2D _WorldPos;
    FVector2D _uvLT;
    FVector2D _uvRB;
    FVector2D _size;
};

struct FTileLineInstanceData
{
    FVector2D _WorldPos;
    FVector2D _color;
    FVector2D _size;
};