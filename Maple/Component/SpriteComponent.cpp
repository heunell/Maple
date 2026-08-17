#include "pch.h"
#include "SpriteComponent.h"
#include "Core/Texture.h"
#include "Core/AssetManager.h"
#include "Core/Mesh.h"
#include "Shader/ShaderManager.h"
#include "Shader/SpriteCBuffer.h"
#include "Shader/SpriteShader.h"
#include "Shader/TransformCBuffer.h"
#include "World/Level.h"

SpriteComponent::SpriteComponent()
{
    _IsRender = true;
}

SpriteComponent::~SpriteComponent()
{}

bool SpriteComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    SceneComponent::Init(Id, Name, Owner);

    _SpriteCBuffer = ShaderManager::Instance().FindCBuffer<SpriteCBuffer>("Sprite");

    SetShader("SpriteShader");

    _Mesh = MESH_MANAGER->FindMesh("TexRect");

    _Type = COMPONENT_TYPE::SPRITE;

    return true;
}

void SpriteComponent::Tick(float DeltaTime)
{
    SceneComponent::Tick(DeltaTime);

    if (_Animation)
    {
        _Animation->Tick(DeltaTime);
    }
}

void SpriteComponent::Collision(float DeltaTime)
{
    SceneComponent::Collision(DeltaTime);
}

void SpriteComponent::Render(float DeltaTime)
{
    SceneComponent::Render(DeltaTime);

    if (_Animation)
    {
        _Animation->SetShader();
    }

    ShaderManager::Instance().SetSample(eTextureSampleType::TEXTURE_SAMPLE_LINEAR);

    _SpriteCBuffer->SetTint(_Tint);

    _SpriteCBuffer->Update();

    _TransformCBuffer->SetWorldMatrix(_Matrix._World);

    Ptr<Level> _Level = Lock<Level>(_Level);

    if (nullptr == _Level)
    {
        return;
    }

    FMatrix View = _Level->GetViewMatrix();

    FMatrix Projection = _Level->GetProjectionMatrix();

    _TransformCBuffer->SetViewMatrix(View);

    _TransformCBuffer->SetProjectionMatrix(Projection);

    _Shader->SetShader();

    if (_Texture)
    {
        _Texture->SetShader(0, SHADER_TYPE::PIXEL, _TextureIndex);
    }

    _Mesh->Render();
}

void SpriteComponent::Destroy()
{
    SceneComponent::Destroy();

    DESTROY(_Animation);
}

void SpriteComponent::SetShader(const std::string& Name)
{
    _Shader = ShaderManager::Instance().FindShader(Name);
}

void SpriteComponent::SetTint(const FVector4D& Tint)
{
    _Tint = Tint;
}

void SpriteComponent::SetTint(float Red, float Green, float Blue)
{
    _Tint._x = Red;

    _Tint._y = Green;

    _Tint._z = Blue;
}

void SpriteComponent::SetOpacity(float Opacity)
{
    _Tint._w = Opacity;
}

void SpriteComponent::SetTexture(const std::string& Name, int TextureIndex)
{
    _Texture = TEXTURE_MANAGER->FindTexture(Name);

    _TextureIndex = TextureIndex;
}

void SpriteComponent::SetTexture(Ptr<class Texture> Texture, INT TextureIndex)
{
    _Texture = Texture;

    _TextureIndex = TextureIndex;
}

void SpriteComponent::SetTextureIndex(int32 Index)
{
    _TextureIndex = Index;
}

void SpriteComponent::AddAnimationSequence(const std::string& Name, bool Loop, bool Reverse, float PlayTime, float PlayRate)
{
    if (!_Animation)
    {
        _Animation = CreateAnimation();
    }

    if (_Animation)
    {
        _Animation->AddSequence(Name, Loop, Reverse, PlayTime, PlayRate);
    }
}

void SpriteComponent::AddAnimationSequence(Ptr<class Animation2DData> Data, bool Loop, bool Reverse, float PlayTime, float PlayRate)
{
    if (!_Animation)
    {
        _Animation = CreateAnimation();
    }

    if (_Animation)
    {
        _Animation->AddSequence(Data, Loop, Reverse, PlayTime, PlayRate);
    }

}

void SpriteComponent::SetPlayTime(const std::string& Name, float Time)
{
    if (!_Animation)
    {
        _Animation = CreateAnimation();
    }

    if (_Animation)
    {
        _Animation->SetPlayTime(Name, Time);
    }
}

void SpriteComponent::SetPlayRate(const std::string& Name, float Rate)
{
    if (!_Animation)
    {
        _Animation = CreateAnimation();
    }

    if (_Animation)
    {
        _Animation->SetPlayRate(Name, Rate);
    }
}

void SpriteComponent::SetLoop(const std::string& Name, bool Loop)
{
    if (!_Animation)
    {
        _Animation = CreateAnimation();
    }

    if (_Animation)
    {
        _Animation->SetLoop(Name, Loop);
    }
}

void SpriteComponent::SetReverse(const std::string& Name, bool Reverse)
{
    if (!_Animation)
    {
        _Animation = CreateAnimation();
    }

    if (_Animation)
    {
        _Animation->SetReverse(Name, Reverse);
    }
}

void SpriteComponent::ChangeAnimation(const std::string& Name)
{
    if (!_Animation)
    {
        _Animation = CreateAnimation();
    }

    if (_Animation)
    {
        _Animation->ChangeAnimation(Name);
    }
}

void SpriteComponent::SetPlay(const std::string& Name, bool Play)
{
    if (!_Animation)
    {
        _Animation = CreateAnimation();
    }

    if (_Animation)
    {
        _Animation->SetPlay(Name, Play);
    }
}

void SpriteComponent::SetAnimationFlip(bool Flip)
{
    if (!_Animation)
    {
        _Animation = CreateAnimation();
    }

    if (_Animation)
    {
        _Animation->SetAnimationFlip(Flip);
    }
}

Ptr<class Animation2D> SpriteComponent::CreateAnimation()
{
    _Animation = New<Animation2D>();

    _Animation->_Owner = This<SpriteComponent>();

    if (!_Animation->Init())
    {
        DESTROY(_Animation);

        return nullptr;
    }

    return _Animation;
}

Ptr<class Animation2D> SpriteComponent::GetAnimation()
{
    return _Animation;
}
