#include "pch.h"
#include "MaterialManager.h"
#include "Material.h"
#include "Shader/ShaderManager.h"
#include "Shader/MaterialCBuffer.h"

bool MaterialManager::Init()
{
    if (!CreateMaterial("DefaultMaterial", "MaterialPixelShader", eTextureSampleType::TEXTURE_SAMPLE_LINEAR))
    {
        return false;
    }

    return true;
}

bool MaterialManager::CreateMaterial(const std::string& Name, const std::string& PixelShader, eTextureSampleType SampleType)
{
    if (FindMaterial(Name))
    {
        return false;
    }

    Ptr<Material> Materials = New<Material>();
    
    Materials->SetName(Name);
    
    Materials->SetSamplerType(SampleType);
    
    Materials->SetPixelShader(PixelShader);

    Materials->_Buffer = ShaderManager::Instance().FindCBuffer<MaterialCBuffer>("Material");

    _Material[Name] = Materials;

    return true;
}

Ptr<class Material> MaterialManager::CreateMaterialInstance(const std::string& Name)
{
    Ptr<Material> Origin = FindMaterial(Name);

    if (nullptr == Origin)
    {
        return nullptr;
    }

    return Origin->Clone();
}

Ptr<class Material> MaterialManager::FindMaterial(const std::string& Name)
{
    auto It = _Material.find(Name);

    if (_Material.end() == It)
    {
        return nullptr;
    }

    return It->second;
}

void MaterialManager::Destroy()
{
    for (auto& It : _Material)
    {
        DESTROY(It.second);
    }
}
