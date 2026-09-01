#pragma once

#include "Core/Object.h"

class AnemoiStateMachine : public Object
{
public:
    AnemoiStateMachine() = default;
    virtual ~AnemoiStateMachine() = default;

private:
    Weak<class AnemoiActor> _Owner;

    Ptr<class AnemoiState> _CurrentState = nullptr;

public:
    void Init(Ptr<class AnemoiActor> Anemoi, Ptr<class AnemoiState> State);

    void Tick(float DeltaTime);

    void Transition(Ptr<class AnemoiState> State);

    Ptr<class AnemoiActor> GetOwner() const;

    Ptr<class AnemoiState> GetCurrentState() const;

    virtual void Destroy() override;
};