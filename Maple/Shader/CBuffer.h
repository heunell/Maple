#pragma once
#include "Core/Object.h"
class CBuffer : public Object
{
public:
    CBuffer();
    virtual ~CBuffer();
    
protected:
    ComPtr<ID3D11Buffer> _buffer = nullptr;
    int _size     = 0;
    int _register = 0;
    int _type     = 0;
    
public:
    bool Create(int size, int regi, int tpye);
    void SetData(void* data);
    virtual void Update();
    virtual void Destroy() override;
};
