#include "pch.h"
#include "CameraManager.h"
#include "Component/CameraComponent.h"
#include "Core/Device.h"
#include "World/Level.h"

bool CameraManager::Init(Ptr<class Level> level)
{
    _Level = level;
   
    FResolution rs = Device::Instance().GetRS();
    
    CreateUIProjection(static_cast<float>(rs._Width), static_cast<float>(rs._Height), 2000);
    
    return true;
}

void CameraManager::SetMainCamera(Ptr<class CameraComponent> camera)
{
    _MainCamera = camera;
    
    _Cameras[_MainCamera->GetName()] = _MainCamera;
}

void CameraManager::ChangeMainCamera(const std::string& name)
{
    auto foundCamera = FindCamera(name);
    
    if (!foundCamera)
    {
        return;
    }

    _MainCamera = foundCamera;
}

Ptr<class CameraComponent> CameraManager::GetMainCamera() const
{
    return _MainCamera;
}

Ptr<class CameraComponent> CameraManager::FindCamera(const std::string& name) const
{
    auto it = _Cameras.find(name);
    
    if (it == _Cameras.end())
    {
        return nullptr;
    }

    return it->second;
}

const FMatrix& CameraManager::GetViewMatrix() const
{
    return _MainCamera->GetViewMatrix();
}

const FMatrix& CameraManager::GetProjectionMatrix() const
{
    return _MainCamera->GetProjectionMatrix();
}

const FVector3D& CameraManager::GetCameraWorldPosition() const
{
    return _MainCamera->GetWorldPosition();
}

const FMatrix& CameraManager::GetUIProjectionMatrix() const
{
    return _UiProjection;
}

void CameraManager::Destroy()
{}

void CameraManager::CreateUIProjection(float x, float y, float Distance)
{
    _UiProjection = DirectX::XMMatrixOrthographicOffCenterLH(0.f, x, 0.f, y, 0, Distance);
}
