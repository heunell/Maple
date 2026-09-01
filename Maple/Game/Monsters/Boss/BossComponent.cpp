#include "pch.h"
#include "BossComponent.h"
#include "BossIdleState.h"
#include "BossBlackBoard.h"
#include "BossBladeState.h"
#include "Game/Monsters/MonsterStateMachine.h"

bool BossComponent::Init(int32 Id, const std::string& Name, Ptr<Actor> Owner)
{
	if (!MonsterComponent::Init(Id, Name, Owner))
	{
		return false;
	}

	Ptr<BossBlackBoard> BlackBoard = New<BossBlackBoard>();

	if (!BlackBoard)
	{
		return false;
	}

	SetBlackBoard(BlackBoard);

	Ptr<MonsterStateMachine> StateMachine = GetStateMachine();

	if (!StateMachine)
	{
		return false;
	}

	_IdleState = New<BossIdleState>();

	if (!_IdleState)
	{
		return false;
	}

	Ptr<BossBladeState> BladeState = New<BossBladeState>();

	if (!BladeState)
	{
		return false;
	}

	if (!BladeState->Init(This<BossComponent>()))
	{
		return false;
	}

	if (!StateMachine->AddState(_IdleState))
	{
		return false;
	}

	if (!StateMachine->AddState(BladeState))
	{
		return false;
	}

	_PatternStates.push_back(BladeState);

	TransitionState(_IdleState);

	return true;
}

void BossComponent::Destroy()
{
	MonsterComponent::Destroy();

	_PatternStates.clear();

	_IdleState.reset();
}

Ptr<MonsterState> BossComponent::SelectPatternState()
{
	if (_PatternStates.empty())
	{
		return nullptr;
	}

	return _PatternStates.front();
}

Ptr<BossIdleState> BossComponent::GetIdleState() const
{
	return _IdleState;
}