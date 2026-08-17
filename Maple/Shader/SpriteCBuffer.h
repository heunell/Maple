#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

class SpriteCBuffer : public CBuffer
{
public:
	SpriteCBuffer();
	virtual ~SpriteCBuffer();

private:
	FSpriteCBufferData _Data;

public:
	virtual void Update() override;

	virtual void Destroy() override;

	void SetTint(float Red, float Green, float Blue, float Alpha);

	void SetTint(const FVector4D& Color);
};

