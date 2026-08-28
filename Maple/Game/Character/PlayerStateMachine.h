#pragma once
#include "PlayerBlackBoard.h"
#include "PlayerState.h"
#include "Collision/CollisionSystem.h"
#include "Common/Info.h"
#include "Core/Object.h"

/*
*  캐릭터의 State 보관과 전환관리 
*  애니메이션의 우선순위는 StateMachine에서 최종 결정한다.
*  todo : BlackBoard는 _aadd_i32 필요해질 _aadd_i32 추가한다.
*/

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
    
    Ptr<class PlayerBlackBoard>  _BlackBoard   = nullptr;
    
    Ptr<class PlayerState>       _CurrentState = nullptr;

public:
    virtual void Init(Ptr<class PlayerComponent> _Player, Ptr<class PlayerState> _State);

    virtual void Destroy() override;

    void Tick(float DeltaTime);

    void Transition(Ptr<class PlayerState> _State);

    void HandleInput(Ptr<class InputAction> Action, INPUT_TYPE::eType ButtonEvent);

    Ptr<class PlayerComponent> GetOwner() const;

    Ptr<class PlayerState> GetCurrentState() const;

    uint8 GetCurrentStateType() const;

    void SetBlackBoard(Ptr<class PlayerBlackBoard> BlackBoard);

    // Todo : void CollideWith(CollisionState::Type eCollisionType, Weak<class CollisionComponent> Collider); 라는 참고 코드가 있는데 이걸 어떻게 써먹어야할 지감이 안옴

public:
    template<typename T>
    Ptr<T> GetBlackBoard() const
    {
        return Cast<PlayerBlackBoard, T>(_BlackBoard);
    }
};

