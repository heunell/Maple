#include "pch.h"
#include "SpriteCBuffer.h"

SpriteCBuffer::SpriteCBuffer()
{}

SpriteCBuffer::~SpriteCBuffer()
{}

void SpriteCBuffer::Update()
{
	SetData(&_Data);
}

void SpriteCBuffer::Destroy()
{}

void SpriteCBuffer::SetTint(float Red, float Green, float Blue, float Alpha)
{
	_Data._Tint._x = Red;
	_Data._Tint._y = Green;
	_Data._Tint._z = Blue;
	_Data._Tint._w = Alpha;

}

void SpriteCBuffer::SetTint(const FVector4D& Color)
{
	SetTint(Color._x, Color._y, Color._z, Color._w);
}
