#include "pch.h"
#include "ShaderManager.h"
#include "ColorMeshShader.h"
#include "ColorCBuffer.h"
#include "FrameMeshShader.h"
#include "MaterialCBuffer.h"
#include "MaterialPixelShader.h"
#include "StaticMeshShader.h"
#include "SpriteCBuffer.h"
#include "SpriteShader.h"
#include "AnimationCBuffer.h"
#include "TransformCBuffer.h"
#include "Core/Device.h"

void ShaderManager::CreateSampler()
{
    D3D11_SAMPLER_DESC Desc = {};
    
    Desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
    
    Desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
    
    Desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
    
    Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    
    Desc.MinLOD         = 0;
    
    Desc.MaxLOD         = D3D11_FLOAT32_MAX;


    //포인트 샘플러
    Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    
    Device::Instance().GetDevice()->CreateSamplerState(&Desc, _Samplers[TEXTURE_SAMPLE_POINT].GetAddressOf());

    //Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

    Device::Instance().GetDevice()->CreateSamplerState(&Desc, _Samplers[TEXTURE_SAMPLE_LINEAR].GetAddressOf());

    Desc.Filter = D3D11_FILTER_ANISOTROPIC;
    
    Desc.MaxAnisotropy = 16; 
    
    Device::Instance().GetDevice()->CreateSamplerState(&Desc, _Samplers[TEXTURE_SAMPLE_ANISOTROPIC].GetAddressOf());
}

bool ShaderManager::Init()
{
    if (!CreateShader<ColorMeshShader>("ColorMeshShader"))
    {
        return false;
    }

    if (!CreateShader<FrameMeshShader>("FrameMeshShader"))
    {
        return false;
    }


    if (!CreateShader<StaticMeshShader>("StaticMeshShader"))
    {
        return false;
    }

    if (!CreateShader<SpriteShader>("SpriteShader"))
    {
        return false;
    }

    if (!CreateShader<MaterialPixelShader>("MaterialPixelShader"))
    {
        return false;
    }

    if (!CreateCBuffer<TransformCBuffer>("Transform", sizeof(FTransformCBufferData), 0, SHADER_TYPE::VERTEX))
    {
        return false;
    }

    if (!CreateCBuffer<AnimationCBuffer>("Animation", sizeof(FAnimation2DCBufferData), 1, SHADER_TYPE::VERTEX))
    {
        return false;
    }

    if (!CreateCBuffer<ColorCBuffer>("Color", sizeof(FColorCBufferData), 0, SHADER_TYPE::PIXEL))
    {
        return false;
    }

    if (!CreateCBuffer<MaterialCBuffer>("Material", sizeof(FMaterialCBufferData), 1, SHADER_TYPE::PIXEL))
    {
        return false;   
    }

    if (!CreateCBuffer<SpriteCBuffer>("Sprite", sizeof(FSpriteCBufferData), 2, SHADER_TYPE::PIXEL))
    {
        return false;
    }

    CreateSampler();

    return true;
}

void ShaderManager::Destroy()
{
    for (auto& it : _Shaders)
    {
        DESTROY(it.second);
    }

    for (auto& it : _cBuffers)
    {
        DESTROY(it.second);
    }

    _Shaders.clear();

    _cBuffers.clear();
}

Ptr<Shader> ShaderManager::FindShader(const std::string& Name)
{
    auto it = _Shaders.find(Name);

    if (_Shaders.end() == it)
    {
        return nullptr;
    }

    return it->second;
}

void ShaderManager::SetSample(eTextureSampleType Type)
{
    Device::Instance().GetContext()->PSSetSamplers(0, 1, _Samplers[Type].GetAddressOf());
}
