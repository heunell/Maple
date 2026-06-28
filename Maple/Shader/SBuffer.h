#pragma once
#include "Core/Object.h"
#include "SBufferData.h"
class SBuffer : public Object
{
public:
    SBuffer();
    virtual ~SBuffer();
    
protected:
    ComPtr<ID3D11Buffer> _buffer;
    ComPtr<ID3D11ShaderResourceView> _srv;
    int32 _size         = 0;
    int32 _elementCount = 0;
    int32 _register     = 0;
    int32 _type         = 0;
    
public:
    virtual void Update();
    bool Create(int32 size, int32 eleCnt, int32 regi, int32 type);
    void Bind();
    
protected:
    void SetData(void* data, int32 eleCnt);
    void Resize(int32 eleCnt);
    
public:
    virtual void Destroy() override;
};
