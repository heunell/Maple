#include "pch.h"
#include "BossComponent.h"
#include "Boss2Component.h"
#include "Boss2BlackBoard.h"
#include "Boss2IdleState.h"
#include "BossBladeState.h"
#include "Boss2DragonState.h"
#include "Boss2LaserState.h"
#include "Game/Monsters/MonsterStateMachine.h"
#include "UI/BossPatternNotice.h"
#include <random>


bool Boss2Component::Init(int32 Id, const std::string& Name, Ptr<Actor> Owner)
{
	if (!MonsterComponent::Init(Id, Name, Owner))
	{
		return false;
	}

	Ptr<Boss2BlackBoard> BlackBoard = New<Boss2BlackBoard>();

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

	_IdleState = New<Boss2IdleState>();

	if (!_IdleState)
	{
		return false;
	}

	Ptr<BossBladeState> BladeState = New<BossBladeState>();

	if (!BladeState)
	{
		return false;
	}

	FBossBladeStateData BladeData;

	BladeData.CastAnimation = "LUCID_MOB_8880150.skill1";

	BladeData.IdleAnimation = "LUCID_MOB_8880150.fly";

	BladeData.SpawnFrame = 35;

	BladeData.SpawnOffset = FVector3D::Zero;

	if (!BladeState->Init(This<Boss2Component>(), _IdleState, BladeData))
	{
		return false;
	}

	Ptr<Boss2DragonState> DragonState = New<Boss2DragonState>();

	if (!DragonState)
	{
		return false;
	}
	
	if (!DragonState->Init(This<Boss2Component>(), _IdleState))
	{
		return false;
	}

	Ptr<Boss2LaserState> LaserState = New<Boss2LaserState>();
	
	if (!LaserState)
	{
		return false;
	}

	//if (!LaserState->Init(This<Boss2Component>(), _IdleState))
	//{
	//	return false;
	//}

	/*Ptr<Boss2GolemState> LaserState = New<Boss2GolemState>();

	if (!GolemState)
	{
		return false;
	}

	if (!GolemState->Init(This<Boss2Component>(), _IdleState))
	{
		return false;
	}*/

	if (!StateMachine->AddState(_IdleState))
	{
		return false;
	}

	if (!StateMachine->AddState(BladeState))
	{
		return false;
	}

	if (!StateMachine->AddState(DragonState))
	{
		return false;
	}

	if (!StateMachine->AddState(LaserState))
	{
		return false;
	}

	_PatternStates.push_back(BladeState);

	_PatternStates.push_back(DragonState);

	_PatternStates.push_back(LaserState);

	TransitionState(_IdleState);

	return true;
}

void Boss2Component::Destroy()
{
	MonsterComponent::Destroy();

	_PatternStates.clear();

	_IdleState.reset();
}

Ptr<MonsterState> Boss2Component::SelectPatternState()
{
	std::vector<Ptr<MonsterState>> SelectableStates;

	for (Ptr<MonsterState>& PatternState : _PatternStates)
	{
		if (!PatternState || !PatternState->CanSelect())
		{
			continue;
		}

		SelectableStates.push_back(PatternState);
	}

	if (SelectableStates.empty())
	{
		return nullptr;
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_int_distribution<int32> PatternDistribution(0, static_cast<int32>(SelectableStates.size()) - 1);

	return SelectableStates[PatternDistribution(RandomEngine)];
}

Ptr<Boss2IdleState> Boss2Component::GetIdleState() const
{
	return _IdleState;
}

void Boss2Component::SetPatternNotice(Ptr<class BossPatternNotice> PatternNotice)
{
	_PatternNotice = PatternNotice;
}

Ptr<BossPatternNotice> Boss2Component::GetPatternNotice() const
{
	return Lock(_PatternNotice);
}
