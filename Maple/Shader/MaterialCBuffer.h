#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

class MaterialCBuffer : public CBuffer
{
public:
    MaterialCBuffer();
    virtual ~MaterialCBuffer();

private:
    FMaterialCBufferData _Data;

public:
    virtual void Update();

    void SetBaseColor(float R, float G, float B, float A);
    void SetBaseColor(const FVector4D& Color);
    void SetOpacity(float Op);
    void SetTextureWidth(int32 W);
    void SetTextureHeight(int32 H);
};

