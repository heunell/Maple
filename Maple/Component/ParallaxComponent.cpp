#include "pch.h"
#include "ParallaxComponent.h"
#include "CameraComponent.h"
#include "SceneComponent.h"

void ParallaxComponent::SetCamera(Ptr<class CameraComponent> Camera)
{
    _Camera = Camera;

    _IsStartCameraPosition = false;
}

bool ParallaxComponent::AddLayer(Ptr<class SceneComponent> Component, float CameraFollowRate)
{
    if(!Component)
    {
        return false;
    }

    FParallaxLayer Layer;

    Layer.Component = Component;

    Layer.BasePosition = Component->GetRelativePosition();

    Layer.CameraFollowRate = CameraFollowRate;

    _Layers.push_back(Layer);

    return true;
}

void ParallaxComponent::ClearLayer()
{
    _Layers.clear();

    _IsStartCameraPosition = false;
}

void ParallaxComponent::Tick(float DeltaTime)
{
    ActorComponent::Tick(DeltaTime);

    Ptr<CameraComponent> Camera = Lock(_Camera);

    if(!Camera)
    {
        return;
    }

    const FVector3D CameraPosition = Camera->GetWorldPosition();

     if (!_IsStartCameraPosition)
    {
        _StartCameraPosition = CameraPosition;

        _IsStartCameraPosition = true;

        return;
    }

    const FVector3D CameraMovement = CameraPosition - _StartCameraPosition;

    for (FParallaxLayer& Layer : _Layers)
    {
        Ptr<SceneComponent> Background = Lock(Layer.Component);

        if (!Background)
        {
            continue;
        }

        Background->SetRelativePosition(Layer.BasePosition + CameraMovement * Layer.CameraFollowRate);
    }
}

void ParallaxComponent::Destroy()
{
    ClearLayer();

    _Camera.reset();

    ActorComponent::Destroy();
}
 