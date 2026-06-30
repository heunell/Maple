#pragma once
#include "SceneComponent.h"

enum eCameraType
{
    Perspective,
    Ortho,
    End
};

class CameraComponent : public SceneComponent
{
public:
    CameraComponent();
    virtual ~CameraComponent();
    
protected:
    FMatrix     _matView      = {};
    FMatrix     _matProj      = {};
    eCameraType _cameraType   = eCameraType::Ortho;
    float       _viewAngle    = 90.f;
    float       _width        = 0.f;
    float       _height       = 0.f;
    float       _viewDistance = 10000.f;

public:
    void SetProjectionType(eCameraType type);
    eCameraType GetProjectionType() const           { return _cameraType; }
    const FMatrix& GetViewMatrix() const            { return _matView; }
    const FMatrix& GetProjMatrix() const            { return _matProj; }
    float GetViewAnglg() const                      { return _viewAngle; }
    void  SetViewAngle(float angle)                 { _viewAngle = angle; }
    float GetWidth() const                          { return _width; }
    void  SetWidht(float width)                     { _width = width; }
    float GetHeight() const                         { return _height; }
    void  SetHeight(float height)                   { _height = height; }
    void  SetViewResolution(float w, float h)       { _width = w; _height = h; }
    float GetViewDistance() const                   { return _viewDistance; }
    void  SetViewDistance(float distance)           { _viewDistance = distance; }
    
public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTime) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;
};
