#include "pch.h"
#include "MaterialPixelShader.h"
#include "Core/Device.h"
#include "Core/DirectoryManager.h"

MaterialPixelShader::MaterialPixelShader()
{}

MaterialPixelShader::~MaterialPixelShader()
{}

bool MaterialPixelShader::Init()
{
    if (false == LoadPixelShader("DefaultMaterialPS", "Mesh.fx"))
    {
        return false;
    }

    return true;
}

void MaterialPixelShader::SetShader()
{
    Device::Instance().GetContext()->PSSetShader(_PixelShader.Get(), nullptr, 0);
}

bool MaterialPixelShader::LoadPixelShader(const char* EntryName, const char* FileName)
{
    auto HLSLPath = DirectoryManager::Instance().GetCachePath("HLSL");

    if (!HLSLPath.has_value())
    {
        return false;
    }

    std::filesystem::path FullPath;

    if (!DirectoryManager::Instance().GetFile(HLSLPath.value(), FileName, OUT FullPath))
    {
        return false;
    }

    uint32 Flag = 0;

#ifdef _DEBUG

    Flag = D3DCOMPILE_DEBUG;

#endif // _DEBUG

    ID3D10Blob* ErrorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(FullPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "ps_5_0", Flag, 0, _PixelShaderBlob.GetAddressOf(), &ErrorBlob)))
    {
        return false;
    }

    if (FAILED(Device::Instance().GetDevice()->CreatePixelShader(_PixelShaderBlob->GetBufferPointer(), _PixelShaderBlob->GetBufferSize(), nullptr, _PixelShader.GetAddressOf())))
    {
        return false;
    }
    return true;
}

void MaterialPixelShader::Destroy()
{}
