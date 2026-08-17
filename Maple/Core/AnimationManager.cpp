#include "pch.h"
#include "AnimationManager.h"
#include "Animation2DData.h"

AnimationManager::AnimationManager()
{}

AnimationManager::~AnimationManager()
{}

bool AnimationManager::Init()
{
    // Codex : 선생님은 여기서 uv 값을 계산해서 넣으신거 같은데 나는 JSON으으로 처리해서 여기에서 재생하면 되려나? 생각좀 해보자
    return true;
}

Ptr<class Animation2DData> AnimationManager::FindAnimation(const std::string& Name)
{
    auto It = _Animation.find(Name);

    if (_Animation.end() == It)
    {
        return nullptr;
    }

    return It->second;
}

bool AnimationManager::CreateAnimation(const std::string& Name, eAnimationTextureType Type)
{
    if (FindAnimation(Name))
    {
        return false;
    }

    Ptr<Animation2DData> Animation = New<Animation2DData>();

    Animation->SetName(Name);

    Animation->SetAnimationTextureType(Type);

    _Animation[Name] = Animation;

    return true;
}

bool AnimationManager::SetAnimationTextureType(const std::string& Name, eAnimationTextureType Type)
{
    Ptr<Animation2DData> Animation = FindAnimation(Name);

    if (!Animation)
    {
        return false;
    }

    Animation->SetAnimationTextureType(Type);

    return true;
}

void AnimationManager::SetTexture(const std::string& Name, Ptr<class Texture> Texture)
{
    Ptr<Animation2DData> Animation = FindAnimation(Name);

    if (!Animation)
    {
        return;
    }

    Animation->SetTexture(Texture);
}

void AnimationManager::SetTexture(const std::string& Name, const std::string& TextureName)
{
    Ptr<Animation2DData> Animation = FindAnimation(Name);

    if (!Animation)
    {
        return;
    }

    Animation->SetTexture(TextureName);
}

void AnimationManager::SetTexture(const std::string& Name, const std::string& TextureName, const std::wstring& FileName)
{
    Ptr<Animation2DData> Animation = FindAnimation(Name);

    if (!Animation)
    {
        return;
    }

    Animation->SetTexture(TextureName, FileName);
}

void AnimationManager::AddFrame(const std::string& Name, const FVector2D& Start, const FVector2D& Size)
{
    Ptr<Animation2DData> Animation = FindAnimation(Name);

    if (!Animation)
    {
        return;
    }

    Animation->AddFrame(Start, Size);
}

void AnimationManager::AddFrame(const std::string& Name, float StartX, float StartY, float SizeX, float SizeY)
{
    Ptr<Animation2DData> Animation = FindAnimation(Name);

    if (!Animation)
    {
        return;
    }

    Animation->AddFrame(StartX, StartY, SizeX, SizeY);
}

void AnimationManager::Destroy()
{
    for (auto& It : _Animation)
    {
        DESTROY(It.second);
    }
}
