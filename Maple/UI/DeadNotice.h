#pragma once
#include "UI.h"

class DeadNotice : public UI
{
public:
    DeadNotice() = default;
    virtual ~DeadNotice() = default;
    
private:
    Weak<class Player> _Player;
    
    float _ElapsedTime = 0.f;
    
    bool _WaitRelease = true;
    
public:
    bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, Ptr<class Player> Player);
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void Destroy() override;
    
private:
    void Confirm();
};

