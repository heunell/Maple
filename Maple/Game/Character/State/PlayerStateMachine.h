#pragma once

#include "../PlayerBlackBoard.h"
#include "Common/Info.h"
#include "Core/Object.h"

class PlayerStateMachine : public Object
{
public:
    PlayerStateMachine() = default;
    ~PlayerStateMachine() = default;
    PlayerStateMachine(const PlayerStateMachine&) = delete;
    PlayerStateMachine(PlayerStateMachine&&) = delete;
    PlayerStateMachine& operator=(const PlayerStateMachine&) = delete;
    PlayerStateMachine& operator=(PlayerStateMachine&&) = delete;

private:
    Weak<class PlayerComponent>  _Owner;
    
    Ptr<class PlayerBlackBoard>  _BlackBoard           = nullptr;
    
    Ptr<class PlayerState>       _CurrentMovementState = nullptr;

    Ptr<class PlayerState>       _CurrentActionState   = nullptr;

public:
    void Init(Ptr<class PlayerComponent> _Player, Ptr<class PlayerState> _MovementState, Ptr<class PlayerState> _ActionState);

    virtual void Destroy() override;

    void Tick(float DeltaTime);

    void HandleInput(Ptr<class InputAction> Action, INPUT_TYPE::eType ButtonEvent);

    void TransitionMovement(Ptr<class PlayerState> State);

    void TransitionAction(Ptr<class PlayerState> State);

    Ptr<class PlayerComponent> GetOwner() const;

    Ptr<class PlayerState> GetCurrentMovementState() const;

    Ptr<class PlayerState> GetCurrentActionState() const;

    uint8 GetCurrentMovementStateType() const;

    uint8 GetCurrentActionStateType() const;

    void SetBlackBoard(Ptr<class PlayerBlackBoard> BlackBoard);

public:
    template<typename T>
    Ptr<T> GetBlackBoard() const
    {
        return Cast<PlayerBlackBoard, T>(_BlackBoard);
    }
};

