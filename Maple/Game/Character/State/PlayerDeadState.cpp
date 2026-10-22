#include "pch.h"
#include "PlayerDeadState.h"
#include "PlayerNoneActionState.h"
#include "Component/PlayerComponent.h"
#include "Component/MovementComponent.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Controller/PlayerController.h"
#include "Game/Character/Player.h"
#include "World/Level.h"
#include <algorithm>
#include <cmath>

void PlayerDeadState::Enter(Ptr<class PlayerComponent> PlayerComponent)
{
	if (!PlayerComponent)
	{
		return;
	}
	
	Ptr<Player> Player = PlayerComponent->GetPlayer();
	
	if (!Player)
	{
		return;
	}
	
	_ElapsedTime = 0.f;
	
	Ptr<PlayerController> Controller = Player->GetController<PlayerController>();
	
	if (Controller && Controller->GetInputComponent())
	{
		Controller->GetInputComponent()->SetEnable(false);
	}
	
	PlayerComponent->TransitionAction(New<PlayerNoneActionState>());
	
	Ptr<MovementComponent> Movement = Player->FindActorComponent<MovementComponent>("Movement");
	
	if (Movement)
	{
		Movement->ResetMovement();
		
		Movement->SetEnable(false);
	}
	
	Ptr<AABBCollisionComponent> Collision = Player->FindSceneComponent<AABBCollisionComponent>("AABB");
	
	if (Collision)
	{
		Collision->SetEnable(false);
	}
	
	Ptr<SpriteComponent> Body = Player->FindSceneComponent<SpriteComponent>("PlayerSprite");
	
	if (Body)
	{
		Body->SetEnable(false);
	}
	
	Ptr<SceneComponent> GhostRoot = Player->FindSceneComponent<SceneComponent>("DeadGhostRoot");
	
	if (GhostRoot)
	{
		GhostRoot->SetRelativePosition(10.f, 72.f, 0.f);
	}
	
	Ptr<SpriteComponent> Ghost = Player->FindSceneComponent<SpriteComponent>("DeadGhost");
	
	if (Ghost)
	{
		Ghost->SetAnimationFlip(Player->IsRight());
		
		Ghost->SetEnable(true);
	}
	
	Ptr<SceneComponent> TombRoot = Player->FindSceneComponent<SceneComponent>("DeadTombRoot");
	
	if (TombRoot)
	{
		TombRoot->SetRelativePosition(0.f, 240.f, 0.f);
	}
	
	Ptr<SpriteComponent> Tomb =	Player->FindSceneComponent<SpriteComponent>("DeadTomb");

	if (Tomb)
	{
		Tomb->SetAnimationFrame(0);
		
		Tomb->SetPlay("TombFallFrame", true);
		
		Tomb->SetEnable(true);
	}

	_Notice = Player->GetLevel()->SpawnActor<DeadNotice>("DeadNotice", FVector3D(-213.f, 136.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Player);
}

void PlayerDeadState::Exit(Ptr<class PlayerComponent> PlayerComponent)
{
	if (_Notice)
	{
		_Notice->Destroy();
		
		_Notice->Remove();
		
		_Notice.reset();
	}

	if (!PlayerComponent)
	{
		return;
	}

	Ptr<Player> Player = PlayerComponent->GetPlayer();

	if (!Player)
	{
		return;
	}

	Ptr<SpriteComponent> Ghost = Player->FindSceneComponent<SpriteComponent>("DeadGhost");

	if (Ghost)
	{
		Ghost->SetEnable(false);
	}

	Ptr<SpriteComponent> Tomb = Player->FindSceneComponent<SpriteComponent>("DeadTomb");

	if (Tomb)
	{
		Tomb->SetEnable(false);
	}

	Ptr<SpriteComponent> Body = Player->FindSceneComponent<SpriteComponent>("PlayerSprite");

	if (Body)
	{
		Body->ChangeAnimation("ARMED_STAND");
		
		Body->SetEnable(true);
	}
}

Ptr<PlayerState> PlayerDeadState::Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime)
{
	if (!PlayerComponent)
	{
		return nullptr;
	}

	Ptr<Player> Player = PlayerComponent->GetPlayer();

	if (!Player)
	{
		return nullptr;
	}

	_ElapsedTime += DeltaTime;

	Ptr<SceneComponent> GhostRoot =	Player->FindSceneComponent<SceneComponent>("DeadGhostRoot");

	if (GhostRoot)
	{
		GhostRoot->SetRelativePosition(std::cos(_ElapsedTime * 0.8f) * 10.f, 72.f - std::sin(_ElapsedTime * 0.8f) * 10.f, 0.f);
	}

	Ptr<SceneComponent> TombRoot = Player->FindSceneComponent<SceneComponent>("DeadTombRoot");

	if (TombRoot)
	{
		TombRoot->SetRelativePosition(0.f, (std::max)(0.f, 240.f - _ElapsedTime * 250.f), 0.f);
	}

	return nullptr;
}

Ptr<PlayerState> PlayerDeadState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
	return nullptr;
}

ePlayerAnimationType PlayerDeadState::GetAnimationType() const
{
	return ePlayerAnimationType();
}
