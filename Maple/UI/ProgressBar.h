#pragma once
#include "Component/SpriteComponent.h"

class ProgressBar : public SpriteComponent
{
public:
	ProgressBar() = default;
	virtual ~ProgressBar() = default;
    ProgressBar(const ProgressBar&) = delete;
    ProgressBar(ProgressBar&&) = delete;
    ProgressBar& operator=(const ProgressBar&) = delete;
    ProgressBar& operator=(ProgressBar&&) = delete;

private:
    Ptr<class Animation2DData> _Data;

    Ptr<class AnimationCBuffer> _AnimationCBuffer;

    FAnimationFrame _BaseFrame;

    float _Ratio = 1.f;

public:
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

    virtual void Render(float DeltaTime) override;

    bool SetData(Ptr<class Animation2DData> Data);

    void SetValue(float CurrentValue, float MaxValue);

private:
    void ApplyRatio();

    // Update가 없는데.. 그럼 어떻게 실시간으로 Progress를 반영하는거지?
};

