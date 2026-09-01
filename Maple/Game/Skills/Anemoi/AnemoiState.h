#pragma once
#include "Core/Object.h"

class AnemoiState : public Object
{
public:
    AnemoiState() = default;
    virtual ~AnemoiState() = default;

public:
    virtual void Enter(Ptr<class AnemoiActor> Actor) {}

    virtual Ptr<AnemoiState> Tick(Ptr<class AnemoiActor> Actor, float DeltaTime) = 0;

    virtual void Exit(Ptr<class AnemoiActor> Actor) {}

    virtual void Destroy() override;
};