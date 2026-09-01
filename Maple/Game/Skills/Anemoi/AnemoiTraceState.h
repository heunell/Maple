#pragma once

#include "AnemoiState.h"

class AnemoiTraceState : public AnemoiState
{
public:
    AnemoiTraceState() = default;
    virtual ~AnemoiTraceState() = default;

private:
    float _AttackTimer = 0.f;

public:
    virtual void Enter(Ptr<class AnemoiActor> Actor) override;

    virtual Ptr<AnemoiState> Tick(Ptr<class AnemoiActor> Actor, float DeltaTime) override;
};