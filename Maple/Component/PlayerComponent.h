#pragma once
#include "ActorComponent.h"
#include "Collision/CollisionSystem.h"
#include "Game/Character/PlayerState.h"
#include "Game/Character/PlayerStateMachine.h"

class PlayerComponent : public ActorComponent
{
public:
    PlayerComponent() = default;
    virtual ~PlayerComponent() = default;

private:
    Ptr<class PlayerStateMachine> _StateMachine = nullptr;

public:    
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

    virtual void Tick(float DeltaTime) override;

    virtual void Destroy() override;

    void Transition(Ptr<class PlayerState> State);
    
    void HandleInput(Ptr<class InputAction> Action, INPUT_TYPE::eType ButtonEvent);

    Ptr<class Player> GetPlayer() const;

    Ptr<class PlayerStateMachine> GetStateMachine() const;

    uint8 GetStateType() const;

    void SetBlackBoard(Ptr<class PlayerBlackBoard> BlackBoard);
};