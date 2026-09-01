#include "pch.h"
#include "MonsterComponent.h"
#include "MonsterBase.h"
#include "MonsterStateMachine.h"
#include "MonsterBlackBoard.h"

bool MonsterComponent::Init(int32 Id, const std::string& Name, Ptr<Actor> Owner)
{
	if (!ActorComponent::Init(Id, Name, Owner))
	{
		return false;
	}

	_StateMachine = New<MonsterStateMachine>();

	if (!_StateMachine)
	{
		return false;
	}

	if (!_StateMachine->Init(This<MonsterComponent>()))
	{
		return false;
	}

	return true;
}

void MonsterComponent::Tick(float DeltaTime)
{
	ActorComponent::Tick(DeltaTime);

	if (_StateMachine)
	{
		_StateMachine->Tick(DeltaTime);
	}
}

void MonsterComponent::Destroy()
{
	ActorComponent::Destroy();

	DESTROY(_StateMachine);
}

void MonsterComponent::TransitionState(Ptr<MonsterState> State)
{
	if (_StateMachine)
	{
		_StateMachine->Transition(State);
	}
}

Ptr<MonsterBase> MonsterComponent::GetMonster() const
{
	return Cast<Actor, MonsterBase>(GetOwner());
}

Ptr<MonsterStateMachine> MonsterComponent::GetStateMachine() const
{
	return _StateMachine;
}

void MonsterComponent::SetBlackBoard(Ptr<MonsterBlackBoard> BlackBoard)
{
    if (_StateMachine)
	{
		_StateMachine->SetBlackBoard(BlackBoard);
	}
}

Ptr<MonsterBlackBoard> MonsterComponent::GetBlackBoard() const
{
	if (!_StateMachine)
	{
		return nullptr;
	}

	return _StateMachine->GetBlackBoard();
}