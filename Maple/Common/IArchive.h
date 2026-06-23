#pragma once
#include <filesystem>
class IArchive
{
public:
    IArchive() {};
    virtual ~IArchive() {};
    
public:
    virtual void Save(std::ofstream& file) = 0;
    virtual void Load(std::ifstream& file) = 0;
};
