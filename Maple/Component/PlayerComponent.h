#pragma once
#include "ActorComponent.h"

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

    void HandleInput(Ptr<class InputAction> Action, INPUT_TYPE::eType ButtonEvent);

    void TransitionMovement(Ptr<class PlayerState> State);

    void TransitionAction(Ptr<class PlayerState> State);

    Ptr<class Player> GetPlayer() const;

    Ptr<class PlayerStateMachine> GetStateMachine() const;

    uint8 GetMovementStateType() const;

    uint8 GetActionStateType() const;

    void UpdateAnimation();
    
    void SetBlackBoard(Ptr<class PlayerBlackBoard> BlackBoard);
};