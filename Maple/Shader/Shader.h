#pragma once
#include "Core/Object.h"
class Shader : public Object
{
public:
    Shader();
    virtual ~Shader();
public:
    virtual bool Init();
    virtual void SetShader();
};
