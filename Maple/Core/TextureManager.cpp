#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

bool TextureManager::Init()
{
    if (!LoadTexture("TestCharacter", TEXT("Characters\\TestCharacter.png")))
    {
        return false;
    }

    if (!LoadTexture("Dead", TEXT("Characters\\Dead.png")))
    {
        return false;
    }

    return true;
}

bool TextureManager::LoadTexture(const std::string& Name, const std::wstring& FileName)
{
    Ptr<Texture> texture = nullptr;
    if (texture = FindTexture(Name))
    {
        return texture->LoadTexture((FileName));
    }

    texture = New<Texture>();
    if (!texture->LoadTexture(FileName))
    {
        return false;
    }

    texture->SetName(Name);
    _Texture[Name] = texture;

    return true;
}

Ptr<class Texture> TextureManager::FindTexture(const std::string& Name)
{
    auto It = _Texture.find(Name);
    if (_Texture.end() == It)
    {
        return nullptr;
    }

    return It->second;
}

void TextureManager::Destroy()
{
    for (auto& It : _Texture)
    {
        DESTROY(It.second);
    }
}
