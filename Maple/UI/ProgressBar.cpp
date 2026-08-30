#include "pch.h"
#include "ProgressBar.h"
#include "Core/Animation2DData.h"
#include "Core/Mesh.h"
#include "Core/Texture.h"
#include "Shader/AnimationCBuffer.h"
#include "Shader/ShaderManager.h"
#include "Shader/SpriteCBuffer.h"
#include "Shader/TransformCBuffer.h"
#include "World/Level.h"
#include <algorithm>

bool ProgressBar::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    if (!SpriteComponent::Init(Id, Name, Owner))
    {
        return false;
    }

    _AnimationCBuffer = ShaderManager::Instance().FindCBuffer<AnimationCBuffer>("Animation");

    return _AnimationCBuffer != nullptr;
}

void ProgressBar::Render(float DeltaTime)
{
    SceneComponent::Render(DeltaTime);

    if (!_Data || !_Texture)
    {
        return;
    }

    const float UVRight = _BaseFrame._UVLeftTop._x + (_BaseFrame._UVRightBottom._x - _BaseFrame._UVLeftTop._x) * _Ratio;

    _AnimationCBuffer->SetUV(_BaseFrame._UVLeftTop._x, _BaseFrame._UVLeftTop._y, UVRight, _BaseFrame._UVRightBottom._y);

    _AnimationCBuffer->SetAnimationFlip(false);

    _AnimationCBuffer->Update();

    ShaderManager::Instance().SetSample(eTextureSampleType::TEXTURE_SAMPLE_LINEAR);

    _SpriteCBuffer->SetTint(_Tint);

    _SpriteCBuffer->Update();

    _TransformCBuffer->SetWorldMatrix(_Matrix._World);

    FMatrix View;

    _TransformCBuffer->SetViewMatrix(View);

    _TransformCBuffer->SetProjectionMatrix(GetLevel()->GetUIProjMatrix());

    _TransformCBuffer->Update();

    _Shader->SetShader();

    _Texture->SetShader(0, SHADER_TYPE::PIXEL, _TextureIndex);

    _Mesh->Render();
}

bool ProgressBar::SetData(Ptr<class Animation2DData> Data)
{
    if (!Data || Data->GetFrameCount() <= 0)
    {
        return false;
    }

    _Data = Data;

    _BaseFrame = _Data->GetFrame(0);

    SetTexture(_Data->GetTexture());

    ApplyRatio();

    return true;
}

void ProgressBar::SetValue(float CurrentValue, float MaxValue)
{
    if (MaxValue <= 0.f)
    {
        _Ratio = 0.f;
    }
    else
    {
        _Ratio = std::clamp(CurrentValue / MaxValue, 0.f, 1.f);
    }

    ApplyRatio();
}

void ProgressBar::ApplyRatio()
{
    if (!_Data)
    {
        return;
    }

    const float VisibleWidth = _BaseFrame._Size._x * _Ratio;

    const float Left = _BaseFrame._Offset._x - _BaseFrame._Size._x * 0.5f;

    SetRelativeScale(VisibleWidth, _BaseFrame._Size._y, 1.f);

    SetRelativePosition(
        Left + VisibleWidth * 0.5f,
        _BaseFrame._Offset._y,
        GetRelativePosition()._z
    );

    SetEnable(_Ratio > 0.f);
}
