#pragma once
#include "Core/Object.h"
class CBuffer : public Object
{
public:
    CBuffer();
    virtual ~CBuffer();
    
protected:
    ComPtr<ID3D11Buffer> _buffer = nullptr;
    int _Size     = 0;
    int _Register = 0;
    int _Type     = 0;
    
public:
    bool Create(int Size, int Register, int Type);
    void SetData(void* Data);
    virtual void Update();
    virtual void Destroy() override;
};
