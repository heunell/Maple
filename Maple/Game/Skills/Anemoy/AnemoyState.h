#pragma once
#include "Core/Object.h"

class AnemoyState : public Object
{
public:
    AnemoyState() = default;
    virtual ~AnemoyState() = default;

public:
    virtual void Enter(Ptr<class AnemoyActor> Actor) {}

    virtual Ptr<AnemoyState> Tick(Ptr<class AnemoyActor> Actor, float DeltaTime) = 0;

    virtual void Exit(Ptr<class AnemoyActor> Actor) {}

    virtual void Destroy() override;
};