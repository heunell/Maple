#include "pch.h"
#include "Player.h"
#include "State/PlayerState.h"
#include "State/PlayerStateMachine.h"
#include "World/World.h"
#include "World/Level.h"
#include "Collision/CollisionProfile.h"
#include "Controller/PlayerController.h"
#include "Component/PlayerComponent.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SphereCollisionComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/SkillComponent.h"
#include "Component/BuffComponent.h"
#include "Core/GameEngine.h"
#include "Core/TimeManager.h"
#include "Render/RenderManager.h"
#include "Game/Map/Gate/GateInteractComponent.h"

bool Player::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	Pawn::Init(Id, Position, Scale, Rotator, Name);

	AddTag("Player");

	_Type = eActorType::Player;

	Ptr<SpriteComponent> Sprite = CreateSceneComponent<SpriteComponent>("PlayerSprite");

	Sprite->SetRenderLayerName("Player");

	Sprite->AddAnimationSequence("ARMED_STAND", true, false, 0.8f, 1.f);

	Sprite->AddAnimationSequence("ARMED_WALK", true, false, 0.6f, 1.f);
	
	Sprite->AddAnimationSequence("ARMED_JUMP", false);

	Sprite->AddAnimationSequence("ARMED_SHOOT", true);

	Sprite->AddAnimationSequence("ARMED_PRONE", true);

	//SetRootComponent(Sprite);

	Sprite->AttachToComponent(GetRoot());

	_Controller = GetLevel()->SpawnActor<PlayerController>("PlayerController", Position, Scale, Rotator);

	_Controller->SetPawn(This<Player>());

	Ptr<SpriteComponent> SkillSprite = CreateSceneComponent<SpriteComponent>("SkillSprite");

	SkillSprite->SetRenderLayerName("Player");

	SkillSprite->AttachToComponent(GetRoot());
	
	SkillSprite->SetEnable(false);

	CreateActorComponent<SkillComponent>("Skill");

	if (!CreateActorComponent<BuffComponent>("Buff"))
	{
		return false;
	}

	Ptr<PlayerComponent> PlayerState = CreateActorComponent<PlayerComponent>("PlayerState");

	Ptr<GateInteractComponent> GateInteract = CreateActorComponent<GateInteractComponent>("GateInteract");

	if (!GateInteract)
	{
		return false;
	}

	GetController<PlayerController>()->KeyBind();

	Ptr<InputComponent> InputComponent = GetController<PlayerController>()->GetInputComponent();

	auto MappingContext = InputSystem::Instance().FindOrAddInputContext("MAPPING_CONTEXT");

	auto MoveRight      = InputSystem::Instance().FindOrAddInputAction("MOVE_RIGHT");
				        
	auto MoveLeft       = InputSystem::Instance().FindOrAddInputAction("MOVE_LEFT");
				        
	auto MoveUp         = InputSystem::Instance().FindOrAddInputAction("MOVE_UP");
				        
	auto MoveDown       = InputSystem::Instance().FindOrAddInputAction("MOVE_DOWN");
				        
	auto MoveJump       = InputSystem::Instance().FindOrAddInputAction("MOVE_JUMP");



	MappingContext->BindInputAction(MoveRight, VK_RIGHT);

	MappingContext->BindInputAction(MoveLeft, VK_LEFT);

	MappingContext->BindInputAction(MoveUp, VK_UP);

	MappingContext->BindInputAction(MoveDown, VK_DOWN);
	
	MappingContext->BindInputAction(MoveJump, 'C');



	InputComponent->AddInputContext("MAPPING_CONTEXT");

	InputComponent->BindAction(MappingContext->GetName(), MoveRight->GetName(), INPUT_TYPE::HOLD, PlayerState.get(), &PlayerComponent::HandleInput);

	InputComponent->BindAction(MappingContext->GetName(), MoveRight->GetName(), INPUT_TYPE::UP,   PlayerState.get(), &PlayerComponent::HandleInput);

	InputComponent->BindAction(MappingContext->GetName(), MoveLeft->GetName(),  INPUT_TYPE::HOLD, PlayerState.get(), &PlayerComponent::HandleInput);

	InputComponent->BindAction(MappingContext->GetName(), MoveLeft->GetName(),  INPUT_TYPE::UP,   PlayerState.get(), &PlayerComponent::HandleInput);

	InputComponent->BindAction(MappingContext->GetName(), MoveUp->GetName(),    INPUT_TYPE::HOLD, PlayerState.get(), &PlayerComponent::HandleInput);

	InputComponent->BindAction(MappingContext->GetName(), MoveUp->GetName(),    INPUT_TYPE::UP,   PlayerState.get(), &PlayerComponent::HandleInput);

	InputComponent->BindAction(MappingContext->GetName(), MoveDown->GetName(),  INPUT_TYPE::HOLD, PlayerState.get(), &PlayerComponent::HandleInput);

	InputComponent->BindAction(MappingContext->GetName(), MoveDown->GetName(),  INPUT_TYPE::UP,   PlayerState.get(), &PlayerComponent::HandleInput);

	InputComponent->BindAction(MappingContext->GetName(), MoveJump->GetName(),  INPUT_TYPE::HOLD, PlayerState.get(), &PlayerComponent::HandleInput);

	InputComponent->BindAction(MappingContext->GetName(), MoveJump->GetName(),  INPUT_TYPE::UP,   PlayerState.get(), &PlayerComponent::HandleInput);
	
	

	_Movement = CreateActorComponent<MovementComponent>("Movement");

	_Movement->SetUpdateComponent(_Root);

	_Movement->SetSpeed(170.f);

	_Movement->SetGravity(-1800.f);

	_Camera = CreateSceneComponent<CameraComponent>("Camera");

	_Camera->AttachToComponent(_Root);



	_AABBCollision = CreateSceneComponent<AABBCollisionComponent>("AABB");
	
	_AABBCollision->SetBoxSize(40.f, 65.f);

	_AABBCollision->SetRelativePosition(FVector3D(0.f, 32.5f, 0.f));

	_AABBCollision->AttachToComponent(_Root);

	_AABBCollision->SetCollisionProfile("Player");

	Ptr<AABBCollisionComponent> TopCollisiion = CreateSceneComponent<AABBCollisionComponent>("TopCollisiion");

	TopCollisiion->SetBoxSize(8.f, 8.f);

	TopCollisiion->SetRelativePosition(0.f, 65.f, 0.f);

	TopCollisiion->AttachToComponent(_Root);

	TopCollisiion->SetCollisionProfile("Player");

	Ptr<AABBCollisionComponent> BottomCollision = CreateSceneComponent<AABBCollisionComponent>("BottomCollision ");

	BottomCollision ->SetBoxSize(8.f, 8.f);
	
	BottomCollision ->SetRelativePosition(0.f, 0.f, 0.f);
	
	BottomCollision ->AttachToComponent(_Root);
	
	BottomCollision ->SetCollisionProfile("Player");

	BottomCollision ->SetCollisionCallBack(COLLISION_STATE_BLOCK, this, &Player::OnGround);

	Ptr<AABBCollisionComponent> LeftCollision = CreateSceneComponent<AABBCollisionComponent>("LeftCollision ");

	LeftCollision->SetBoxSize(8.f, 8.f);

	LeftCollision->SetRelativePosition(-20.f, 32.5f, 0.f);

	LeftCollision->AttachToComponent(_Root);

	LeftCollision->SetCollisionProfile("Player");

	LeftCollision->SetCollisionCallBack(COLLISION_STATE_BLOCK, this, &Player::OnLeftWall);

	Ptr<AABBCollisionComponent> RightCollision = CreateSceneComponent<AABBCollisionComponent>("RightCollision ");

	RightCollision->SetBoxSize(8.f, 8.f);

	RightCollision->SetRelativePosition(20.f, 32.5f, 0.f);

	RightCollision->AttachToComponent(_Root);

	RightCollision->SetCollisionProfile("Player");

	RightCollision->SetCollisionCallBack(COLLISION_STATE_BLOCK, this, &Player::OnRightWall);

	GameEngine::Instance().GetWorld()->SetMainPlayer(This<Player>());

	return true;
}

void Player::Tick(float DeltaTime)
{
	Pawn::Tick(DeltaTime);
}

void Player::Collision(float DeltaTime)
{
	Pawn::Collision(DeltaTime);
}

void Player::Render(float DeltaTime)
{
	Pawn::Render(DeltaTime);
}

void Player::Destroy()
{
	Pawn::Destroy();

	TimeManager::Instance().RemoveTimer(_TimerID);
}

bool Player::IsRight() const
{
	return _bReverse;
}

void Player::MoveRight()
{
	_bReverse = true;

	_Movement->SetMoveAxis(FVector3D::Axis_X);

	Ptr<SpriteComponent> Sprite = FindSceneComponent<SpriteComponent>("PlayerSprite");

	if(Sprite)
	{
		Sprite->SetAnimationFlip(true);
	}
}

void Player::MoveLeft()
{
    _bReverse = false;

    _Movement->SetMoveAxis(-FVector3D::Axis_X);

    Ptr<SpriteComponent> Sprite = FindSceneComponent<SpriteComponent>("PlayerSprite");

    if (Sprite)
    {
        Sprite->SetAnimationFlip(false);
    }
}

void Player::MoveStop()
{
    _Movement->Stop();
}

bool Player::Jump()
{
    return _Movement->StartJump(500.f);
}

bool Player::DoubleJump()
{
	if (!_Movement)
	{
		return false;
	}

	return _Movement->StartDoubleJump(IsRight() ? 430.f : -430.f, 360.f);
}

void Player::MoveUp(float DeltaTime)
{
	// todo : Move Up과 Down은 메이플스토리에서 해당하는 로직이 조금씩 다르기 때문에 잠시 대기
}

void Player::OnGround(Weak<class CollisionComponent> Collision)
{
	Ptr<CollisionComponent> Component = Lock<CollisionComponent>(Collision);

	if(!Component || !Component->GetProfile())
	{
		return;
	}

	if(Component->GetProfile()->GetChannel() != eCollisionChannel::COLLISION_CHANNEL_ENVIRONMENT)
	{
		return;
	}

	Ptr<AABBCollisionComponent> Ground = Cast<CollisionComponent, AABBCollisionComponent>(Component);

	if(!Ground)
	{
		return;
	}

	// 가로로 긴 Environment AABB만 바닥으로 처리한다.
	if(Ground->GetBoxSize()._x <= Ground->GetBoxSize()._y)
	{
		return;
	}

	if(_Movement->GetVelocity()._y > 0.f)
	{
		return;
	}

	const FAABB2D& PlayerBox = _AABBCollision->GetBox();

	const FAABB2D& GroundBox = Ground->GetBox();

	const float CorrectionY = GroundBox._Max._y - PlayerBox._Min._y;

	if(CorrectionY > 0.f)
	{
		_Movement->Blocking(FVector3D(0.f, CorrectionY, 0.f));
	}

	_Movement->SetLanding(true);
}

void Player::OnLeftWall (Weak<class CollisionComponent> Collision)
{
	Ptr<CollisionComponent> Component = Lock<CollisionComponent>(Collision);

	if(!Component || !Component->GetProfile())
	{
		return;
	}

	if(Component->GetProfile()->GetChannel() != eCollisionChannel::COLLISION_CHANNEL_ENVIRONMENT)
	{
		return;
	}

	Ptr<AABBCollisionComponent> Wall = Cast<CollisionComponent, AABBCollisionComponent>(Component);

	if(!Wall)
	{
		return;
	}

	if(Wall->GetBoxSize()._y <= Wall->GetBoxSize()._x)
	{
		return;
	}

	const FAABB2D& PlayerBox = _AABBCollision->GetBox();

	const FAABB2D& WallBox = Wall->GetBox();

	const float CorrectionX = WallBox._Max._x - PlayerBox._Min._x;

	if(CorrectionX > 0.f)
	{
		_Movement->Blocking(FVector3D(CorrectionX, 0.f, 0.f));
	}
}

void Player::OnRightWall(Weak<class CollisionComponent> Collision)
{
	Ptr<CollisionComponent> Component = Lock<CollisionComponent>(Collision);

	if(!Component || !Component->GetProfile())
	{
		return;
	}

	if(Component->GetProfile()->GetChannel() != eCollisionChannel::COLLISION_CHANNEL_ENVIRONMENT)
	{
		return;
	}

	Ptr<AABBCollisionComponent> Wall = Cast<CollisionComponent, AABBCollisionComponent>(Component);

	if(!Wall)
	{
		return;
	}

	if(Wall->GetBoxSize()._y <= Wall->GetBoxSize()._x)
	{
		return;
	}

	const FAABB2D& PlayerBox = _AABBCollision->GetBox();

	const FAABB2D& WallBox = Wall->GetBox();

	const float CorrectionX = WallBox._Min._x - PlayerBox._Max._x;

	if(CorrectionX < 0.f)
	{
		_Movement->Blocking(FVector3D(CorrectionX, 0.f, 0.f));
	}	
}

void Player::Prone(float DeltaTime)
{
	MoveStop();
}
