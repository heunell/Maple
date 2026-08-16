#pragma once
#include "SubManager.h"
#include <unordered_map>

class TextureManager : public SubManager
{
public:
    TextureManager() = default;
    virtual ~TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager operator=(const TextureManager&) = delete;
    TextureManager operator=(TextureManager&&) = delete;

private:
    std::unordered_map<std::string, Ptr<class Texture>> _Texture;

public:
    bool Init();
    
    bool LoadTexture(const std::string& Name, const std::wstring& FileName);
    
    Ptr<class Texture> FindTexture(const std::string& Name);

    virtual void Destroy() override;
};

