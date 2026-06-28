#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

class TransformCBuffer : public CBuffer
{
public:
    TransformCBuffer();
    virtual ~TransformCBuffer();
    
private:
    FTransformCBufferData _data;
    
public:
    virtual void Upate();
    void SetWorldMatrix(const FMatrix& mat)
    {
        _data._world = mat;
    }
    void SetViewMatrix(const FMatrix& mat)
    {
        _data._view = mat;
    }

    void SetProjMatrix(const FMatrix& mat)
    {
        _data._proj = mat;
    }
};
