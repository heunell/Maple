#pragma once
#include "Common/Matrix.h"
#include "Common/Vector2D.h"
#include "Common/Vector3D.h"
#include "Common/Vector4D.h"

struct FTransformCBufferData
{
    FMatrix _World;
    FMatrix _View;
    FMatrix _Proj;
    FMatrix _wv; 
    FMatrix _wvp;
};

struct FColorCBufferData
{
    FVector4D _Color;
};

struct FMaterialCBufferData
{
    FVector4D _BasaColor;
    float _Opacity;
    int32 _TextureWidht;
    int32 _TextureHeight;
    float _Empty;               
};

struct FAnimation2DCBufferData
{
    FVector2D _uvLeftTop;
    FVector2D _uvRightBottom;
    int32 _AnimationFilp;
    FVector3D _Empty;
};

struct FSpriteCBufferData
{
    FVector4D _Tint;
};

struct FTileMapCBufferData
{
    FVector2D _uvLeftTop;
    FVector2D _uvRightBottom;
};

struct FUIBrushData
{
    FVector4D _Tint;
    FVector2D _uvLeftTop;
    FVector2D _uvRightBottom;
    int32 _AnimationEnable;
    int32 _TextureEnable;
    FVector2D _Empty;
};

struct FProgressBarData
{
    float _Percent = 1.f;
    int32 _BarDirection = 0;
    FVector2D _Empty;
};
