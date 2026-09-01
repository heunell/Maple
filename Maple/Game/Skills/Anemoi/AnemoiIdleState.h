#pragma once

#include "AnemoiState.h"

class AnemoiIdleState : public AnemoiState
{
public:
    AnemoiIdleState() = default;
    virtual ~AnemoiIdleState() = default;

public:
    virtual void Enter(Ptr<class AnemoiActor> Actor) override;

    virtual Ptr<AnemoiState> Tick(Ptr<class AnemoiActor> Actor, float DeltaTime) override;
};