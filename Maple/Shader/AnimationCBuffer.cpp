#include "pch.h"
#include "AnimationCBuffer.h"

AnimationCBuffer::AnimationCBuffer()
{}

AnimationCBuffer::~AnimationCBuffer()
{}

void AnimationCBuffer::Update()
{
	SetData(&_Data);
}

void AnimationCBuffer::SetUV(float LeftTopX, float LeftTopY, float RightBottomX, float RightBottomY)
{
	_Data._uvLeftTop._x = LeftTopX;

	_Data._uvLeftTop._y = LeftTopY;

	_Data._uvRightBottom._x = RightBottomX;

	_Data._uvRightBottom._y = RightBottomY;
}

void AnimationCBuffer::SetAnimationFlip(bool Flip)
{
	_Data._AnimationFilp = Flip ? 1 : 0;
}
