#pragma once

#include "AnemoiState.h"

class AnemoiEndState : public AnemoiState
{
public:
    AnemoiEndState() = default;
    virtual ~AnemoiEndState() = default;

public:
    virtual void Enter(Ptr<class AnemoiActor> Actor) override;

    virtual Ptr<AnemoiState> Tick(Ptr<class AnemoiActor> Actor, float DeltaTime) override;
};