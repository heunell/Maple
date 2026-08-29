#include "pch.h"
#include "CameraComponent.h"
#include "World/Level.h"
#include "World/CameraManager.h"
#include "Core/Device.h"
#include "Shader/TransformCBuffer.h"

CameraComponent::CameraComponent()
{
    FResolution Resolution = Device::Instance().GetRS();
    
    _Width = static_cast<float>(Resolution._Width);
    
    _Height = static_cast<float>(Resolution._Height);
}

CameraComponent::~CameraComponent()
{}

void CameraComponent::SetProjectionType(eCameraType Type)
{
    _CameraType = Type;
    
    switch (_CameraType)
    {
    case Perspective:
        break;
    
    case Ortho:
        _MatrixProjection = DirectX::XMMatrixOrthographicOffCenterLH(_Width / -2, _Width / 2, _Height / -2, _Height / 2, -1.f, _ViewDistance);
        break;
    
    default:
        break;
    }
}

bool CameraComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    SceneComponent::Init(Id, Name, Owner);
    
    SetProjectionType(_CameraType);
    
    Ptr<Level> level = Owner->GetLevel();
    
    if (nullptr == level)
    {
        return false;
    }

    level->SetMainCamera(This<CameraComponent>());
    
    _Type = COMPONENT_TYPE::CAMERA;
    
    return true;
}

void CameraComponent::Tick(float DeltaTime)
{
    SceneComponent::Tick(DeltaTime);

    Ptr<SceneComponent> Parent = GetParent();

    if(Parent)
    {
        FVector3D CameraPosition = Parent->GetWorldPosition();

        if(_IsCameraLock)
        {
            const float HalfWidth  = _Width  * 0.5f;

            const float HalfHeight = _Height * 0.5f;

            const float MinX = _CameraLock._Min._x + HalfWidth;

            const float MaxX = _CameraLock._Max._x - HalfWidth;

            const float MinY = _CameraLock._Min._y + HalfHeight;

            const float MaxY = _CameraLock._Max._y - HalfHeight;

            if(MinX <= MaxX)
            {
                CameraPosition._x = Utility::Clamp(CameraPosition._x, MinX, MaxX);
            }
            else
            {
                CameraPosition._x = (_CameraLock._Min._x + _CameraLock._Max._x) * 0.5f;
            }

            if(MinY <= MaxY)
            {
                CameraPosition._y = Utility::Clamp(CameraPosition._y, MinY, MaxY);
            }
            else
            {
                CameraPosition._y = (_CameraLock._Min._y + _CameraLock._Max._y) * 0.5f;
            }
        }

        SetWorldPosition(CameraPosition);
    }

    _MatrixView.Indentity();

    for(int i = 0; i < AXIS_TYPE::END; ++i)
    {
        FVector3D Axis = _Axis[i];
        
        memcpy(&_MatrixView[i][0], &Axis, sizeof(FVector3D));
    }

    _MatrixView.Transpose();

    _MatrixView._41 = -_Axis[AXIS_TYPE::X].Dot(_World._position);

    _MatrixView._42 = -_Axis[AXIS_TYPE::Y].Dot(_World._position);

    _MatrixView._43 = -_Axis[AXIS_TYPE::Z].Dot(_World._position);
    
    // _MatrixView.Indentity();
   
    // for (int i = 0; i < AXIS_TYPE::END; ++i)
    // {
    //     FVector3D axis = _Axis[i];
     
    //     memcpy(&_MatrixView[i][0], &axis, sizeof(FVector3D));
    // }
   
    // _MatrixView.Transpose();
    
    // _MatrixView._41 = -_Axis[AXIS_TYPE::X].Dot(_World._position);
    
    // _MatrixView._42 = -_Axis[AXIS_TYPE::Y].Dot(_World._position);
    
    // _MatrixView._43 = -_Axis[AXIS_TYPE::Z].Dot(_World._position);
}

void CameraComponent::Collision(float DeltaTime)
{
    SceneComponent::Collision(DeltaTime);
}

void CameraComponent::Render(float DeltaTime)
{
    SceneComponent::Render(DeltaTime);
}

void CameraComponent::Destroy()
{}

void CameraComponent::SetCameraLock(const FAABB2D &LockBound)
{
    _CameraLock = LockBound;

    _IsCameraLock = true;
}
