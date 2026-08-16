#pragma once
#include "Controller.h"
#include "Component/InputComponent.h"

class PlayerController : public Controller
{
public:
    PlayerController() = default;
    virtual ~PlayerController() = default;
    PlayerController(const PlayerController&) = delete;
    PlayerController(PlayerController&&) = delete;
    PlayerController& operator = (const PlayerController&) = delete;
    PlayerController& operator = (PlayerController&&) = delete;

public:
    Ptr<InputComponent> _Input;
    
public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;
   
    virtual void Tick(float DeltaTime) override;
    
    virtual void Collision(float DeltaTime) override;
    
    virtual void Render(float DeltaTime) override;

    virtual void Destroy() override;

    Ptr<InputComponent> GetInputComponent() const;
};