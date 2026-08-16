#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

class ColorCBuffer : public CBuffer
{
public:
    ColorCBuffer();
    virtual ~ColorCBuffer();

private:
    FColorCBufferData _Data;

public:
    virtual void Update();

public:
    void SetColor(float r, float g, float b, float a)
    {
        _Data._Color._x = r;
        _Data._Color._y = g;
        _Data._Color._z = b;
        _Data._Color._w = a;
    }
};

