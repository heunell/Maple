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
    FMatrix     _MatrixView          = {};
    
    FMatrix     _MatrixProjection = {};
    
    eCameraType _CameraType    = eCameraType::Ortho;
    
    float       _ViewAngle     = 90.f;
    
    float       _Width         = 0.f;
    
    float       _Height        = 0.f;
    
    float       _ViewDistance  = 10000.f;

public:
    void SetProjectionType(eCameraType Type);
    
    eCameraType GetProjectionType() const              { return _CameraType;                }
                                                       
    const FMatrix& GetViewMatrix() const               { return _MatrixView;                }
                                                                                            
    const FMatrix& GetProjectionMatrix() const         { return _MatrixProjection;          }
                                                       
    float GetViewAnglg() const                         { return _ViewAngle;                 }
                                                       
    void  SetViewAngle(float Angle)                    { _ViewAngle = Angle;                }
                                                       
    float GetWidth() const                             { return _Width;                     }
                                                       
    void  SetWidht(float Width)                        { _Width = Width;                    }
                                                       
    float GetHeight() const                            { return _Height;                    }
                                                       
    void  SetHeight(float Height)                      { _Height = Height;                  }
    
    void  SetViewResolution(float Width, float Height) { _Width = Width; _Height = Height;  }
    
    float GetViewDistance() const                      { return _ViewDistance;              }   
                                                      
    void  SetViewDistance(float Distance)              { _ViewDistance = Distance;          }
    
public:
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void Collision(float DeltaTime) override;
    
    virtual void Render(float DeltaTime) override;
    
    virtual void Destroy() override;
};
