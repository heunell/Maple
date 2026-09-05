#include "pch.h"
#include "MonsterState.h"

void MonsterState::Enter(Ptr<class MonsterComponent> Monster)
{}

Ptr<MonsterState> MonsterState::Tick(Ptr<class MonsterComponent> Monster, float DeltaTime)
{
	return nullptr;
}

bool MonsterState::CanSelect() const
{
    return true;
}

void MonsterState::Exit(Ptr<class MonsterComponent> Monster)
{}

void MonsterState::Destroy()
{}

uint8 MonsterState::GetType() const
{
	return _StateType;
}

void MonsterState::Reset()
{
}
