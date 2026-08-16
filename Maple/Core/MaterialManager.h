#pragma once
#include "SubManager.h"
#include <unordered_map>

class MaterialManager : public SubManager
{
public:
    MaterialManager() = default;
    virtual ~MaterialManager() = default;
    MaterialManager(const MaterialManager&) = delete;
    MaterialManager(MaterialManager&&) = delete;
    MaterialManager operator= (const MaterialManager&) = delete;
    MaterialManager operator= (MaterialManager&&) = delete;

private:
    std::unordered_map<std::string, Ptr<class Material>> _Material;

public:
    bool Init();
    
    bool CreateMaterial(const std::string& Name, const std::string& PixelShader, eTextureSampleType SampleType);
    
    Ptr<class Material> CreateMaterialInstance(const std::string& Name);
    
    Ptr<class Material> FindMaterial(const std::string& Name);

    virtual void Destroy() override;
};

