#include "pch.h"
#include "AnimationManager.h"
#include "Animation2DData.h"
#include "DirectoryManager.h"
#include <fstream>
#include <nlohmann/json.hpp>

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
}

bool AnimationManager::LoadAnimationFile(const std::wstring& FileName, const std::string& DataName)
{
    std::optional<std::filesystem::path> ResourcePath = DirectoryManager::Instance().GetCachePath("Resources");

    if (!ResourcePath.has_value())
    {
        return false;
    }

    std::filesystem::path TexturePath;

    if (!DirectoryManager::Instance().GetDirectory(ResourcePath.value(), "Texture", TexturePath))
    {
        return false;
    }

    std::filesystem::path JsonPath;

    if (!DirectoryManager::Instance().GetFile(TexturePath, FileName, JsonPath))
    {
        return false;
    }

    std::ifstream File(JsonPath);

    if (!File.is_open())
    {
        return false;
    }

    nlohmann::json Json;

    try
    {
        File >> Json;

        if (!Json.contains(DataName) || !Json[DataName].is_object())
        {
            return false;
        }

        for (const auto& [AnimationName, AnimationJson] : Json[DataName].items())
        {
            if (!AnimationJson.contains("Texture") || !AnimationJson["Texture"].is_string() || 
                !AnimationJson.contains("Frames") ||!AnimationJson["Frames"].is_array() || AnimationJson["Frames"].empty())
            {
                return false;
            }

            const std::string TextureName = AnimationJson["Texture"].get<std::string>();

            if (!CreateAnimation(AnimationName, eAnimationTextureType::SPRITE))
            {
                return false;
            }

            SetTexture(AnimationName, TextureName, std::filesystem::path(TextureName).wstring());

            Ptr<Animation2DData> Animation = FindAnimation(AnimationName);

            if (!Animation || !Animation->GetTexture())
            {
                return false;
            }

            for (const nlohmann::json& FrameJson : AnimationJson["Frames"])
            {
                if (!FrameJson.contains("UV") || !FrameJson["UV"].is_array() || FrameJson["UV"].size() != 4 ||
                    !FrameJson.contains("Size") || !FrameJson["Size"].is_array() || FrameJson["Size"].size() != 2 ||
                    !FrameJson.contains("Offset") || !FrameJson["Offset"].is_array() || FrameJson["Offset"].size() != 2 ||
                    !FrameJson.contains("Delay") || !FrameJson["Delay"].is_number())
                {
                    return false;
                }

                const nlohmann::json& UV = FrameJson["UV"];

                const nlohmann::json& Size = FrameJson["Size"];

                const nlohmann::json& Offset = FrameJson["Offset"];

                const FVector2D UVLeftTop(UV[0].get<float>(), UV[1].get<float>());

                const FVector2D UVRightBottom(UV[2].get<float>(), UV[3].get<float>());

                const FVector2D FrameSize(Size[0].get<float>(), Size[1].get<float>());

                const FVector2D FrameOffset(Offset[0].get<float>(), Offset[1].get<float>());

                const float Delay = FrameJson["Delay"].get<float>() / 1000.f;

                AddFrame(AnimationName, UVLeftTop, UVRightBottom, FrameSize, FrameOffset, Delay);
            }
        }
    }
    catch (const nlohmann::json::exception&)
    {
        return false;
    }

    return true;
}

bool AnimationManager::Init()
{
    if (!LoadAnimationFile(TEXT("Characters\\CharacterState.json"), "Animations"))
    {
        return false;
    }

    return LoadAnimationFile(TEXT("Skill\\Skill.json"), "Skills");
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

void AnimationManager::AddFrame(const std::string& Name, const FVector2D& UVLeftTop, const FVector2D& UVRightBottom, const FVector2D& Size, const FVector2D& Offset, float Delay)
{
    Ptr<Animation2DData> Animation = FindAnimation(Name);

    if (!Animation)
    {
        return;
    }

    Animation->AddFrame(UVLeftTop, UVRightBottom, Size, Offset, Delay);
}

void AnimationManager::Destroy()
{
    for (auto& It : _Animation)
    {
        DESTROY(It.second);
    }
}
