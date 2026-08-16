#include "pch.h"
#include "Texture.h"
#include "DirectoryManager.h"
#include "Device.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif

const FTextureInfo* Texture::GetTexture(int32 Index) const
{
    if (_TextureInfos.size() < Index || Index < 0)
    {
        return nullptr;
    }

    return &_TextureInfos[Index];
}

bool Texture::LoadTexture(const std::wstring& FileName)
{
    std::filesystem::path FilePath;
    
    std::optional<std::filesystem::path> ResourcePath = DirectoryManager::Instance().GetCachePath("Resources");

    if (!DirectoryManager::Instance().GetDirectory(ResourcePath.value(), "Texture", FilePath))
    {
        return false;
    }

    if (!DirectoryManager::Instance().GetFile(FilePath, FileName, FilePath))
    {
        return false;
    }

    FTextureInfo Texture;
    
    if (DirectoryManager::Instance().IsExtension(FilePath, ".DDS"))
    {
        if (FAILED(DirectX::LoadFromDDSFile(FilePath.wstring().c_str(), DirectX::DDS_FLAGS_NONE, nullptr, Texture._Image)))
        {
            return false;
        }
    }
    else if (DirectoryManager::Instance().IsExtension(FilePath, ".TGA"))
    {
        if (FAILED(DirectX::LoadFromTGAFile(FilePath.wstring().c_str(), nullptr, Texture._Image)))
        {
            return false;
        }
    }
    else
    {
        if (FAILED(DirectX::LoadFromWICFile(FilePath.wstring().c_str(), DirectX::WIC_FLAGS_NONE, nullptr, Texture._Image)))
        {
            return false;
        }
    }

    size_t srcIndex = _TextureInfos.size();

    Texture._FileName = FileName;
    
    Texture._Width = static_cast<uint32>(Texture._Image.GetImages()->width);
    
    Texture._Height = static_cast<uint32>(Texture._Image.GetImages()->height);

    _TextureInfos.emplace_back(std::move(Texture));

    return CreateResourcesView(static_cast<int32>(srcIndex));

}

void Texture::SetShader(int32 RegisterNum, int32 ShaderBufferType, int32 TextureIndex)
{
    if (ShaderBufferType & SHADER_TYPE::VERTEX)
    {
        Device::Instance().GetContext()->VSSetShaderResources(RegisterNum, 1, _TextureInfos[TextureIndex]._Srv.GetAddressOf());
    }
    
    if (ShaderBufferType & SHADER_TYPE::PIXEL)
    {
        Device::Instance().GetContext()->PSSetShaderResources(RegisterNum, 1, _TextureInfos[TextureIndex]._Srv.GetAddressOf());
    }
}

void Texture::ResetShader(int32 RegisterNum, int32 ShaderBufferType)
{
    ID3D11ShaderResourceView* Srv = nullptr;

    if (ShaderBufferType & SHADER_TYPE::VERTEX)
    {
        Device::Instance().GetContext()->VSSetShaderResources(RegisterNum, 1, &Srv);
    }

    if (ShaderBufferType & SHADER_TYPE::PIXEL)
    {
        Device::Instance().GetContext()->PSSetShaderResources(RegisterNum, 1, &Srv);
    }
}

void Texture::Destroy()
{
}

bool Texture::CreateResourcesView(int32 Index)
{
    if (FAILED(DirectX::CreateShaderResourceView(Device::Instance().GetDevice().Get(), 
               _TextureInfos[Index]._Image.GetImages(), 
               _TextureInfos[Index]._Image.GetImageCount(), 
               _TextureInfos[Index]._Image.GetMetadata(), 
               _TextureInfos[Index]._Srv.GetAddressOf())))
    {
        return false;
    }

    return true;
}
