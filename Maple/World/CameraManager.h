#pragma once
#include "Core/Object.h"
#include <unordered_map>
class CameraManager : public Object
{
public:
    CameraManager() = default;
    virtual ~CameraManager() = default;
    CameraManager(const CameraManager&) = delete;
    CameraManager(CameraManager&&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;
    CameraManager& operator=(CameraManager&&) = delete;
    
private:
    Weak<class Level> _Level;
   
    Ptr<class CameraComponent> _MainCamera;
    
    std::unordered_map<std::string, Ptr<class CameraComponent>> _Cameras;
    
    FMatrix _UiProjection;
    
public:
    bool Init(Ptr<class Level> Level);
    
    void SetMainCamera(Ptr<class CameraComponent> Camera);
    
    void ChangeMainCamera(const std::string& Name);
    
    Ptr<class CameraComponent> GetMainCamera() const;
    
    Ptr<class CameraComponent> FindCamera(const std::string& Name) const;
    
    const FMatrix& GetViewMatrix() const;
    
    const FMatrix& GetProjectionMatrix() const;
    
    const FVector3D& GetCameraWorldPosition() const;
    
    const FMatrix& GetUIProjectionMatrix() const;
    
public:
    virtual void Destroy() override;
    
private:
    void CreateUIProjection(float x, float y, float Distance);
};
