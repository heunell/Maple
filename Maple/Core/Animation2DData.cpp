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

void Animation2DData::AddFrame(const FVector2D& Start, const FVector2D& Size)
{
    AddFrame(Start._x, Start._y, Size._x, Size._y);
}

void Animation2DData::AddFrame(float StartX, float StartY, float SizeX, float SizeY)
{
    FAnimationFrame Frame;
    
    Frame._start._x = StartX;
    
    Frame._start._y = StartY;

    Frame._size._x = SizeX;

    Frame._size._y = SizeY;

    _Frames.emplace_back(Frame);
}

void Animation2DData::Destroy()
{
}
