#include "pch.h"
#include "MaterialCBuffer.h"

MaterialCBuffer::MaterialCBuffer()
{}

MaterialCBuffer::~MaterialCBuffer()
{}

void MaterialCBuffer::Update()
{
	SetData(&_Data);
}

void MaterialCBuffer::SetBaseColor(float R, float G, float B, float A)
{
    _Data._BasaColor._x = R;
    _Data._BasaColor._y = G;
    _Data._BasaColor._z = B;
    _Data._BasaColor._w = A;
}

void MaterialCBuffer::SetBaseColor(const FVector4D& Color)
{
    SetBaseColor(Color._x, Color._y, Color._z, Color._w);
}

void MaterialCBuffer::SetOpacity(float Op)
{
    _Data._Opacity = Op;
}

void MaterialCBuffer::SetTextureWidth(int32 W)
{
    _Data._TextureWidht = W;
}

void MaterialCBuffer::SetTextureHeight(int32 H)
{
    _Data._TextureHeight = H;
}
