#include "pch.h"
#include "Boss2DragonState.h"
#include "Boss2Component.h"
#include "Boss2Dragon.h"
#include "Component/SpriteComponent.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterComponent.h"
#include "UI/BossPatternNotice.h"
#include "World/Level.h"

bool Boss2DragonState::Init(Ptr<MonsterComponent> Owner, Ptr<MonsterState> IdleState)
{
	if (!Owner || !IdleState)
	{
		return false;
	}

	_Owner = Owner;

	_IdleState = IdleState;

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return false;
	}

	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (!LevelOwner)
	{
		return false;
	}

	_Dragon = LevelOwner->SpawnActor<Boss2Dragon>("Boss2Dragon", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	return _Dragon != nullptr;
}

void Boss2DragonState::Enter(Ptr<MonsterComponent> Monster)
{
	_CastStarted = false;

	_DragonStarted = false;

	Ptr<Boss2Component> BossController = Cast<MonsterComponent, Boss2Component>(Monster);

	if (!BossController)
	{
		return;
	}

	Ptr<BossPatternNotice> Notice = BossController->GetPatternNotice();

	if (Notice)
	{
		Notice->Show(L"루시드가 강력한 소환수를 소환했습니다!");
	}
}

Ptr<MonsterState> Boss2DragonState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
{
	if (!Monster)
	{
		return nullptr;
	}

	Ptr<Boss2Component> BossController = Cast<MonsterComponent, Boss2Component>(Monster);

	if (!BossController)
	{
		return nullptr;
	}

	Ptr<BossPatternNotice> Notice = BossController->GetPatternNotice();

	if (!Notice || !Notice->IsFinished())
	{
		return nullptr;
	}

	Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

	if (!MonsterOwner)
	{
		return nullptr;
	}

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (!BossSprite)
	{
		return nullptr;
	}

	if (!_CastStarted)
	{
		_CastStarted = true;

		BossSprite->ChangeAnimation("LUCID_MOB_8880150.skill2");

		return nullptr;
	}

	if (!_DragonStarted)
	{
		Ptr<Animation2D> Animation = BossSprite->GetAnimation();

		if (!Animation || !Animation->IsFinished())
		{
			return nullptr;
		}

		SpawnDragonPattern();
	}

	if (!_DragonStarted)
	{
		return nullptr;
	}

	return Lock(_IdleState);
}

void Boss2DragonState::SpawnDragonPattern()
{
	if (_DragonStarted || !_Dragon)
	{
		return;
	}

	Ptr<MonsterComponent> Owner = Lock(_Owner);

	if (!Owner)
	{
		return;
	}

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	_DragonStarted = true;

	_Dragon->Start();
}

bool Boss2DragonState::CanSelect() const
{
	if (!_Dragon || _Dragon->IsEnable())
	{
		return false;
	}

	Ptr<MonsterComponent> Owner = Lock(_Owner);

	if (!Owner)
	{
		return false;
	}

	Ptr<Boss2Component> BossController = Cast<MonsterComponent, Boss2Component>(Owner);

	return BossController && BossController->GetPatternNotice();
}

void Boss2DragonState::Destroy()
{
	if (_Dragon)
	{
		_Dragon->SetPatternEnable(false);
	}

	_Dragon.reset();

	_Owner.reset();

	_IdleState.reset();

	MonsterState::Destroy();
}