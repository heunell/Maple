#pragma once
#include "Object.h"

class Asset : public Object
{
public:
    Asset();
    virtual ~Asset();
    
protected:
    std::string _Name;
    
public:
    void SetName(const std::string& Name) { _Name = Name; }
    const std::string& GetName()          { return _Name; }
};
