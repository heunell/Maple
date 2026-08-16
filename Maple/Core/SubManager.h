#pragma once
#include "Object.h"

class SubManager : public Object
{
public:
    SubManager();

    virtual ~SubManager();

public:
    virtual bool Init() { return false;}
};



// todo : subManager , AssetManager New ,T  확인