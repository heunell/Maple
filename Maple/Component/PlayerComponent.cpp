#include "pch.h"
#include "PlayerComponent.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"
#include "Game/Character/State/PlayerState.h"
#include "Game/Character/State/PlayerStateMachine.h"
#include "Game/Character/State/PlayerNoneActionState.h"
#include "Game/Character/State/PlayerIdleState.h"

bool PlayerComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    if(!ActorComponent::Init(Id, Name, Owner))
    {
        return false;
    }

    _StateMachine = New<PlayerStateMachine>();

    if(!_StateMachine)
    {
        return false;
    }

    Ptr<PlayerState> MovementState = New<PlayerIdleState>();

    Ptr<PlayerState> ActionState   = New<PlayerNoneActionState>();

    _StateMachine->Init(This<PlayerComponent>(), MovementState, ActionState);

    return true;
}

void PlayerComponent::Tick(float DeltaTime)
{
    ActorComponent::Tick(DeltaTime);

    if(_StateMachine)
    {
        _StateMachine->Tick(DeltaTime);

        UpdateAnimation();
    }
}

void PlayerComponent::Destroy()
{
    ActorComponent::Destroy();

    DESTROY(_StateMachine);
}

void PlayerComponent::HandleInput(Ptr<class InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    _StateMachine->HandleInput(Action, ButtonEvent);
}

void PlayerComponent::TransitionMovement(Ptr<class PlayerState> State)
{
    if(_StateMachine)
    {
        _StateMachine->TransitionMovement(State);
    }
}

void PlayerComponent::TransitionAction(Ptr<class PlayerState> State)
{
    if(_StateMachine)
    {
        _StateMachine->TransitionAction(State);
    }
}


Ptr<class Player> PlayerComponent::GetPlayer() const
{
    return Cast<Actor, Player>(GetOwner());
}

Ptr<class PlayerStateMachine> PlayerComponent::GetStateMachine() const
{
    return _StateMachine;
}

uint8 PlayerComponent::GetMovementStateType() const
{
    if(!_StateMachine)
    {
        return static_cast<uint8>(-1); //  여기 개념을 잘 모르지만 마무리에 개념 몰아서 정리하기
    }

    return _StateMachine->GetCurrentMovementStateType();
}

uint8 PlayerComponent::GetActionStateType() const
{
    if(!_StateMachine)
    {
        return static_cast<uint8>(-1); //  여기 개념을 잘 모르지만 마무리에 개념 몰아서 정리하기
    }

    return _StateMachine->GetCurrentActionStateType();
}

void PlayerComponent::UpdateAnimation()
{
    Ptr<Player> Player = GetPlayer();

    if(!Player || !_StateMachine)
    {
        return;
    }

    Ptr<SpriteComponent> Sprite = Player->FindSceneComponent<SpriteComponent>("PlayerSprite");

    if(!Sprite)
    {
        return;
    }

    Ptr<PlayerState> MovementState = _StateMachine->GetCurrentMovementState();

    Ptr<PlayerState> ActionState   = _StateMachine->GetCurrentActionState();

    if(!MovementState || !ActionState)
    {
        return;
    }

    ePlayerAnimationType AnimationType = ActionState->GetAnimationType();

    // 행동중이 아니라면 이동상태의 애니메이션을 사용
    if(AnimationType == ePlayerAnimationType::None)
    {
        AnimationType = MovementState->GetAnimationType();
    }

    switch(AnimationType)
    {
    case ePlayerAnimationType::Idle:
        Sprite->ChangeAnimation("ARMED_STAND");
        break;

    case ePlayerAnimationType::Walk:
        Sprite->ChangeAnimation("ARMED_WALK");
        break;

    case ePlayerAnimationType::Jump:
        Sprite->ChangeAnimation("ARMED_JUMP");
        break;

    case ePlayerAnimationType::Shoot:
        Sprite->ChangeAnimation("ARMED_SHOOT");
        break;
    
    case ePlayerAnimationType::None:
    case ePlayerAnimationType::Prone:
    case ePlayerAnimationType::Dead:
    case ePlayerAnimationType::Alter:
    case ePlayerAnimationType::End:
    break;    
    }
}

void PlayerComponent::SetBlackBoard(Ptr<class PlayerBlackBoard> BlackBoard)
{
    if(_StateMachine)
    {
        _StateMachine->SetBlackBoard(BlackBoard);
    }
}
