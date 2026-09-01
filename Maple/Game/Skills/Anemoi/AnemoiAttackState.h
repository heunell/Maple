#pragma once

#include "AnemoiState.h"

class AnemoiAttackState : public AnemoiState
{
public:
    AnemoiAttackState() = default;
    virtual ~AnemoiAttackState() = default;

public:
    virtual void Enter(Ptr<class AnemoiActor> Actor) override;

    virtual Ptr<AnemoiState> Tick(Ptr<class AnemoiActor> Actor, float DeltaTime) override;
};