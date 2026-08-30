#pragma once

#include "ActorComponent.h"
#include <vector>

struct FParallaxLayer
{
    Weak<class SceneComponent> Component;

    FVector3D BasePosition = {};

    float CameraFollowRate = 0.f;
};
class ParallaxComponent : public ActorComponent
{
public:
    ParallaxComponent() = default;
    virtual ~ParallaxComponent() = default;
	ParallaxComponent(const ParallaxComponent&) = delete;
	ParallaxComponent(ParallaxComponent&&) = delete;
	ParallaxComponent& operator=(const ParallaxComponent&) = delete;
	ParallaxComponent& operator=(ParallaxComponent&&) = delete;

public:
    void SetCamera(Ptr<class CameraComponent> Camera);

    bool AddLayer(Ptr<class SceneComponent> Component, float CameraFollowRate);

    void ClearLayer();

    virtual void Tick(float DeltaTime) override;

    virtual void Destroy() override;

private:
    Weak<class CameraComponent> _Camera;

    std::vector<FParallaxLayer> _Layers;

    FVector3D _StartCameraPosition = {};

    bool _IsStartCameraPosition = false;
};
