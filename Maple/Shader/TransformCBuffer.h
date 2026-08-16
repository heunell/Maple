#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

class TransformCBuffer : public CBuffer
{
public:
    TransformCBuffer();
    virtual ~TransformCBuffer();
    
private:
    FTransformCBufferData _Data;
    
public:
    virtual void Update();

    void SetWorldMatrix(const FMatrix& Matrix)
    {
        _Data._World = Matrix;
    }
    void SetViewMatrix(const FMatrix& Matrix)
    {
        _Data._View = Matrix;
    }

    void SetProjectionMatrix(const FMatrix& Matrix)
    {
        _Data._Proj = Matrix;
    }
};
