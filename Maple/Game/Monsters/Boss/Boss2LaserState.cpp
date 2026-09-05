#include "pch.h"
#include "Boss2LaserState.h"
#include "Boss2Component.h"
#include "Component/SpriteComponent.h"
#include "Core/GameEngine.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterComponent.h"
#include "UI/BossPatternNotice.h"
#include "World/Level.h"
#include "World/World.h"

#include <functional>
#include <random>

bool Boss2LaserState::Init(Ptr<MonsterComponent> Owner, Ptr<MonsterState> IdleState)
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

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (!BossSprite)
	{
		return false;
	}

	BossSprite->AddAnimationSequence("LaserRain.action_no_blank", false);

	if (!_LaserPool.Configure(
		_PatternData.PoolMaxCount,
		std::bind_front(&Boss2LaserState::CreateLaser, this),
		std::bind_front(&Boss2LaserState::EnableLaser, this),
		std::bind_front(&Boss2LaserState::DisableLaser, this)))
	{
		return false;
	}

	return _LaserPool.Reserve(_PatternData.PoolMaxCount);
}

Ptr<Boss2Laser> Boss2LaserState::CreateLaser()
{
	Ptr<MonsterComponent> Owner = Lock(_Owner);

	if (!Owner)
	{
		return nullptr;
	}

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return nullptr;
	}

	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (!LevelOwner)
	{
		return nullptr;
	}

	return LevelOwner->SpawnActor<Boss2Laser>("Boss2Laser", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
}

void Boss2LaserState::EnableLaser(const Ptr<Boss2Laser>& Laser)
{
	if (Laser)
	{
		Laser->SetPoolEnable(true);
	}
}

void Boss2LaserState::DisableLaser(const Ptr<Boss2Laser>& Laser)
{
	if (Laser)
	{
		Laser->SetPoolEnable(false);
	}
}

void Boss2LaserState::Enter(Ptr<MonsterComponent> Monster)
{
	_ElapsedTime = 0.f;

	_SpawnCount = 0;

	_ActionStarted = false;

	if (!Monster)
	{
		return;
	}

	Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	_StartPosition = MonsterOwner->GetWorldPosition();

	Ptr<Boss2Component> BossController = Cast<MonsterComponent, Boss2Component>(Monster);

	if (!BossController)
	{
		return;
	}

	Ptr<BossPatternNotice> Notice = BossController->GetPatternNotice();

	if (Notice)
	{
		Notice->Show(L"루시드가 강력한 공격을 사용하려 합니다!");
	}
}

Ptr<MonsterState> Boss2LaserState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
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

	if (!_ActionStarted)
	{
		Ptr<BossPatternNotice> Notice = BossController->GetPatternNotice();

		if (!Notice || !Notice->IsFinished())
		{
			return nullptr;
		}

		// 기존 보스 Sprite를 전조로 교체하고 고정 위치로 이동한다.
		MonsterOwner->SetWorldPosition(_PatternData.ActionPosition);

		BossSprite->SetRenderLayerName(_PatternData.ActionRenderLayer);

		BossSprite->ChangeAnimation("LaserRain.action_no_blank");

		BossSprite->SetAnimationFrame(0);

		BossSprite->SetPlay("LaserRain.action_no_blank", true);

		_ActionStarted = true;

		return nullptr;
	}

	if (_SpawnCount == 0)
	{
		Ptr<Animation2D> Animation = BossSprite->GetAnimation();

		if (!Animation || !Animation->IsFinished())
		{
			return nullptr;
		}

		// 전조가 끝난 즉시 첫 레이저를 생성한다.
		BossSprite->SetEnable(false);

		SpawnLaserPattern();

		_ElapsedTime = 0.f;

		return nullptr;
	}

	if (_SpawnCount < _PatternData.LaserCount)
	{
		_ElapsedTime += DeltaTime;

		while (_ElapsedTime >= _PatternData.SpawnInterval && _SpawnCount < _PatternData.LaserCount)
		{
			_ElapsedTime -= _PatternData.SpawnInterval;

			SpawnLaserPattern();
		}
	}

	if (_SpawnCount < _PatternData.LaserCount || _LaserPool.GetInUseCount() > 0)
	{
		return nullptr;
	}

	return Lock(_IdleState);
}

void Boss2LaserState::SpawnLaserPattern()
{
	if (_SpawnCount >= _PatternData.LaserCount)
	{
		return;
	}

	Ptr<World> CurrentWorld = GameEngine::Instance().GetWorld();

	if (!CurrentWorld)
	{
		return;
	}

	Ptr<Actor> Player = CurrentWorld->GetPlayer();

	if (!Player)
	{
		return;
	}

	Ptr<Boss2Laser> Laser = _LaserPool.Acquire();

	if (!Laser)
	{
		return;
	}

	Ptr<SceneComponent> PlayerCenter = Player->FindSceneComponent("AABB");

	if (!PlayerCenter)
	{
		return;
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_real_distribution<float> RotationDistribution(_PatternData.RotationMin, _PatternData.RotationMax);

	// 생성 순간의 플레이어 위치만 전달한다.
	Laser->Start(This<Boss2LaserState>(), PlayerCenter->GetWorldPosition(), RotationDistribution(RandomEngine));

	++_SpawnCount;
}

void Boss2LaserState::Exit(Ptr<MonsterComponent> Monster)
{
	_LaserPool.ReleaseAll();

	if (!_ActionStarted)
	{
		return;
	}

	if (!Monster)
	{
		return;
	}

	Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	MonsterOwner->SetWorldPosition(_StartPosition);

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (BossSprite)
	{
		BossSprite->SetRenderLayerName("Default");

		BossSprite->ChangeAnimation("LUCID_MOB_8880150.fly");

		BossSprite->SetAnimationFrame(0);

		BossSprite->SetPlay("LUCID_MOB_8880150.fly", true);

		BossSprite->SetEnable(true);
	}

	_ActionStarted = false;
}

bool Boss2LaserState::CanSelect() const
{
	if (_LaserPool.GetInUseCount() > 0)
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

void Boss2LaserState::ReleaseLaser(Ptr<Boss2Laser> Laser)
{
	if (Laser)
	{
		_LaserPool.Release(Laser);
	}
}

void Boss2LaserState::Destroy()
{
	_LaserPool.ReleaseAll();

	_Owner.reset();

	_IdleState.reset();

	MonsterState::Destroy();
}