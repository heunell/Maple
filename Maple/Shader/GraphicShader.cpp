#include "pch.h"
#include "GraphicShader.h"
#include "Core/Device.h"
#include "Core/DirectoryManager.h"

GraphicShader::GraphicShader()
{}

GraphicShader::~GraphicShader()
{}

bool GraphicShader::Init()
{
    return false;
}

void GraphicShader::Destroy()
{
}

void GraphicShader::SetShader()
{
    Device::Instance().GetContext()->VSSetShader(_VertexShader.Get(), nullptr, 0);

    Device::Instance().GetContext()->PSSetShader(_PixelShader.Get(), nullptr, 0);

    Device::Instance().GetContext()->IASetInputLayout(_InputLayout.Get());
}

void GraphicShader::AddInputLayourDescription(const char* Sementic, uint32 SementicIndex, DXGI_FORMAT Format, uint32 InputSlot, uint32 Size, D3D11_INPUT_CLASSIFICATION InputSlotClass, uint32 InstanceDataStepRate)
{
    D3D11_INPUT_ELEMENT_DESC Description = {};

    Description.SemanticName = Sementic;

    Description.SemanticIndex = SementicIndex;

    Description.Format = Format;

    Description.InputSlot = InputSlot;

    Description.InputSlotClass = InputSlotClass;

    Description.InstanceDataStepRate = InstanceDataStepRate;

    Description.AlignedByteOffset = _InputLayoutSize;

    _InputLayoutSize += Size;

    _Description.push_back(Description);
}

bool GraphicShader::CreateInputLayout()
{
    if (FAILED(Device::Instance().GetDevice()->CreateInputLayout(&_Description[0], (UINT)_Description.size(), _VertexShaderBlob->GetBufferPointer(), _VertexShaderBlob->GetBufferSize(), &_InputLayout)))
    {
        return false;
    }

    return true;
}

bool GraphicShader::LoadVertexShader(const char* EntryName, const char* FileName)
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

#endif

    ID3D10Blob* ErrorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(FullPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName, "vs_5_0", Flag, 0, _VertexShaderBlob.GetAddressOf(), &ErrorBlob)))
    {
        return false;
    }

    if (FAILED(Device::Instance().GetDevice()->CreateVertexShader(_VertexShaderBlob->GetBufferPointer(), _VertexShaderBlob->GetBufferSize(), nullptr, _VertexShader.GetAddressOf())))
    {
        return false;
    }

    return true;
}

bool GraphicShader::LoadPixelShader(const char* EntryName, const char* FileName)
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

#endif

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