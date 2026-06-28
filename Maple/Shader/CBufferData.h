#pragma once
#include "Common/Matrix.h"
#include "Common/Vector2D.h"
#include "Common/Vector3D.h"
#include "Common/Vector4D.h"

struct FTransformCBufferData
{
    FMatrix _world;
    FMatrix _view;
    FMatrix _proj;
    FMatrix _wv; 
    FMatrix _wvp;
};

struct FColorCBufferData
{
    FVector4D _color;
};

struct FMaterialCBufferData
{
    FVector4D _basaColor;
    float _opacity;
    int32 _textureWidht;
    int32 _textureHeight;
    float _empty;               
};

struct FAnimation2DCBufferData
{
    FVector2D _uvLT;
    FVector2D _uvRB;
    int32 _animFilp;
    FVector3D _empty;
};

struct FSpriteCBufferData
{
    FVector4D _tint;
};

struct FTileMapCBufferData
{
    FVector2D _uvLT;
    FVector2D _uvRB;
};

struct FUIBrushData
{
    FVector4D _tint;
    FVector2D _uvLT;
    FVector2D _uvRB;
    int32 _animEnable;
    int32 _textureEnable;
    FVector2D _empty;
};

struct FProgressBarData
{
    float _percent = 1.f;
    int32 _barDir = 0;
    FVector2D _empty;
};
