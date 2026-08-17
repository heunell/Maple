#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

class AnimationCBuffer : public CBuffer
{
public:
	AnimationCBuffer();
	virtual ~AnimationCBuffer();

private:
	FAnimation2DCBufferData _Data;

public:
	virtual void Update() override;

	void SetUV(float LeftTopX, float LeftTopY, float RightBottomX, float RightBottomY);

	void SetAnimationFlip(bool Flip);
};

