#include "pch.h"
#include "Boss2LaserState.h"
#include "Boss2Component.h"
#include "Component/Animation2D.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterComponent.h"
#include "UI/BossPatternNotice.h"
#include "World/Level.h"

#include <functional>


bool Boss2LaserState::Init(Ptr<Boss2Component> Owner, Ptr<MonsterState> IdleState, const FBoss2LaserStateData& StateData)
{
	if (!Owner || !IdleState || StateData.ActionAnimation.empty() || StateData.IdleAnimation.empty() || StateData.NoticeText.empty())
	{
		return false;
	}

	_Owner = Owner;

	_IdleState = IdleState;

	_StateData = StateData;

	_RandomEngine.seed(std::random_device{}());

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return false;
	}

	_ActionSprite = MonsterOwner->CreateSceneComponent<SpriteComponent>("Boss2LaserActionSprite");

	if (!_ActionSprite)
	{
		return false;
	}

	_ActionSprite->SetRenderLayerName(_StateData.ActionRenderLayer);

	_ActionSprite->AddAnimationSequence(_StateData.ActionAnimation, false);

	_ActionSprite->AttachToComponent(MonsterOwner->GetRoot());

	_ActionSprite->SetEnable(false);

	if (!_LaserPool.Configure(_PatternData.PoolMaxCount, std::bind_front(&Boss2LaserState::CreateLaser, this), std::bind_front(&Boss2LaserState::EnableLaser, this), std::bind_front(&Boss2LaserState::DisableLaser, this)))
	{
		return false;
	}

	return _LaserPool.Reserve(_PatternData.PoolMaxCount);
}

Ptr<Boss2Laser> Boss2LaserState::CreateLaser()
{
	Ptr<Boss2Component> Owner = Lock(_Owner);

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
	_ActionStarted = false;

	_LaserStarted = false;

	_SpawnElapsedTime = 0.f;

	_NextSpawnTime = 0.f;

	_ReturnElapsedTime = 0.f;

	_SpawnedLaserCount = 0;

	_Target.reset();

	_LaserPool.ReleaseAll();

	if (_ActionSprite)
	{
		_ActionSprite->SetEnable(false);
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

	_ReturnPosition = MonsterOwner->GetWorldPosition();

	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (LevelOwner)
	{
		Ptr<Actor> PlayerActor = LevelOwner->FindActor("Player");

		_Target = Cast<Actor, Player>(PlayerActor);
	}

	Ptr<Boss2Component> BossController = Cast<MonsterComponent, Boss2Component>(Monster);

	if (!BossController)
	{
		return;
	}

	Ptr<BossPatternNotice> PatternNotice = BossController->GetPatternNotice();

	if (PatternNotice)
	{
		PatternNotice->Show(L"루시드가 강력한 공격을 사용하려 합니다!");
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

	// Notice가 끝난 뒤 전조 연출을 시작한다.
	if (!_ActionStarted)
	{
		Ptr<BossPatternNotice> PatternNotice = BossController->GetPatternNotice();

		if (!PatternNotice || !PatternNotice->IsFinished())
		{
			return nullptr;
		}

		StartAction(Monster);

		return nullptr;
	}

	// 전조 연출이 끝나면 첫 레이저를 즉시 생성한다.
	if (!_LaserStarted)
	{
		if (!_ActionSprite)
		{
			return nullptr;
		}

		Ptr<Animation2D> ActionAnimation = _ActionSprite->GetAnimation();

		if (!ActionAnimation || !ActionAnimation->IsFinished())
		{
			return nullptr;
		}

		_ActionSprite->SetEnable(false);

		_LaserStarted = true;

		_SpawnElapsedTime = 0.f;

		_NextSpawnTime = 0.f;
	}

	if (_SpawnedLaserCount < _PatternData.LaserCount)
	{
		_SpawnElapsedTime += DeltaTime;

		while (_SpawnedLaserCount < _PatternData.LaserCount && _SpawnElapsedTime >= _NextSpawnTime)
		{
			if (!SpawnLaser())
			{
				break;
			}

			++_SpawnedLaserCount;

			_NextSpawnTime += _PatternData.SpawnInterval;
		}
	}

	if (_SpawnedLaserCount < _PatternData.LaserCount || _LaserPool.GetInUseCount() > 0)
	{
		return nullptr;
	}

	_ReturnElapsedTime += DeltaTime;

	if (_ReturnElapsedTime < _PatternData.ReappearDelay)
	{
		return nullptr;
	}

	ReturnLucid(Monster);

	return Lock(_IdleState);
}

bool Boss2LaserState::StartAction(Ptr<MonsterComponent> Monster)
{
	if (!Monster || !_ActionSprite)
	{
		return false;
	}

	Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

	if (!MonsterOwner)
	{
		return false;
	}

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (!BossSprite || !LevelOwner)
	{
		return false;
	}

	FVector3D CameraPosition = LevelOwner->GetCameraWorldPosition();

	FVector3D ActionPosition(CameraPosition._x + _StateData.ActionOffset._x, CameraPosition._y + _StateData.ActionOffset._y, _StateData.ActionOffset._z);

	// 전조 리소스가 루시드의 패턴 모션을 대신 그린다.
	BossSprite->SetEnable(false);

	_ActionSprite->SetWorldPosition(ActionPosition);

	_ActionSprite->SetEnable(true);

	_ActionSprite->ChangeAnimation(_StateData.ActionAnimation);

	_ActionSprite->SetAnimationFrame(0);

	_ActionSprite->SetPlay(_StateData.ActionAnimation, true);

	_ActionStarted = true;

	return true;
}

bool Boss2LaserState::SpawnLaser()
{
	Ptr<Player> Target = Lock(_Target);

	if (!Target)
	{
		return false;
	}

	FVector3D SpawnPosition = Target->GetWorldPosition();

	Ptr<AABBCollisionComponent> PlayerCollision = Target->FindSceneComponent<AABBCollisionComponent>("AABB");

	if (PlayerCollision)
	{
		SpawnPosition = PlayerCollision->GetWorldPosition();
	}

	Ptr<Boss2Laser> Laser = _LaserPool.Acquire();

	if (!Laser)
	{
		return false;
	}

	std::uniform_real_distribution<float> RotationDistribution(_PatternData.RotationMin, _PatternData.RotationMax);

	Laser->Start(This<Boss2LaserState>(), SpawnPosition, RotationDistribution(_RandomEngine));

	return true;
}

void Boss2LaserState::ReturnLucid(Ptr<MonsterComponent> Monster)
{
	if (!Monster)
	{
		return;
	}

	Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	MonsterOwner->SetWorldPosition(_ReturnPosition);

	if (BossSprite)
	{
		BossSprite->SetEnable(true);

		BossSprite->ChangeAnimation(_StateData.IdleAnimation);
	}
}

bool Boss2LaserState::CanSelect() const
{
	Ptr<Boss2Component> Owner = Lock(_Owner);

	return Owner && Owner->GetPatternNotice();
}

void Boss2LaserState::Exit(Ptr<MonsterComponent> Monster)
{
	if (_ActionSprite)
	{
		_ActionSprite->SetEnable(false);
	}

	_LaserPool.ReleaseAll();

	ReturnLucid(Monster);

	MonsterState::Exit(Monster);
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

	_ActionSprite.reset();

	_Target.reset();

	_Owner.reset();

	_IdleState.reset();

	MonsterState::Destroy();
}