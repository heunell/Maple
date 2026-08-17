#include "pch.h"
#include "Animation2DData.h"
#include "Texture.h"
#include "AssetManager.h"

Animation2DData::Animation2DData()
{}

Animation2DData::~Animation2DData()
{}

eAnimationTextureType Animation2DData::GetAnimationTextureType() const
{
    return _Type;
}

Ptr<class Texture> Animation2DData::GetTexture() const
{
    return _Texture;
}

const FAnimationFrame& Animation2DData::GetFrame(int32 Index) const
{
    if (Index < 0)
    {
        return _Frames[0];
    }
    else if (Index >= _Frames.size())
    {
        return _Frames[_Frames.size() - 1];
    }

    return _Frames[Index];
}

int32 Animation2DData::GetFrameCount() const
{
    return static_cast<int32>(_Frames.size());
}

void Animation2DData::SetAnimationTextureType(eAnimationTextureType Type)
{
    _Type = Type;
}

void Animation2DData::SetTexture(const std::string& Name)
{
    _Texture = TEXTURE_MANAGER->FindTexture(Name);
}

void Animation2DData::SetTexture(Ptr<class Texture> Textures)
{
    _Texture = Textures;
}

void Animation2DData::SetTexture(const std::string& Name, const std::wstring& FileName)
{
    if (!TEXTURE_MANAGER->LoadTexture(Name, FileName))
    {
        return;
    }

    _Texture = TEXTURE_MANAGER->FindTexture(Name);
}

void Animation2DData::AddFrame(const FVector2D& UVLeftTop, const FVector2D& UVRightBottom, const FVector2D& Size, const FVector2D& Offset, float Delay)
{
    FAnimationFrame Frame;

    Frame._UVLeftTop = UVLeftTop;
    
    Frame._UVRightBottom = UVRightBottom;
    
    Frame._Size = Size;
    
    Frame._Offset = Offset;
    
    Frame._Delay = Delay;

    _Frames.emplace_back(Frame);
}

void Animation2DData::Destroy()
{
}
