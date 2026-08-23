#include "pch.h"
#include "Player.h"
#include "World/World.h"
#include "World/Level.h"
#include "Controller/PlayerController.h"
#include "Component/MovementComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/SkillComponent.h"
#include "Core/GameEngine.h"
#include "Core/TimeManager.h"
#include "Render/RenderManager.h"

bool Player::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	Pawn::Init(Id, Position, Scale, Rotator, Name);

	AddTag("Player");

	_Type = eActorType::Player;

	//Ptr<StaticMeshComponent> MeshComponent = CreateSceneComponent<StaticMeshComponent>("PlayerMesh");

	//MeshComponent->SetMesh("TexRect");

	//MeshComponent->AddTexture(0, "TestCharacter", 0);

	//SetRootComponent(MeshComponent);

	Ptr<SpriteComponent> Sprite = CreateSceneComponent<SpriteComponent>("PlayerSprite");

	Sprite->AddAnimationSequence("ARMED_STAND", true, false, 0.8f, 1.f);

	Sprite->AddAnimationSequence("ARMED_WALK", true, false, 0.6f, 1.f);
	
	Sprite->AddAnimationSequence("ARMED_JUMP", false);

	Sprite->AddAnimationSequence("ARMED_SHOOT", true);

	//SetRootComponent(Sprite);

	Sprite->AttachToComponent(GetRoot());

	_Controller = GetLevel()->SpawnActor<PlayerController>("PlayerController", Position, Scale, Rotator);

	_Controller->SetPawn(This<Player>());

	Ptr<SpriteComponent> SkillSprite = CreateSceneComponent<SpriteComponent>("SkillSprite");
	
	SkillSprite->AttachToComponent(GetRoot());
	
	SkillSprite->SetEnable(false);

	CreateActorComponent<SkillComponent>("Skill");

	GetController<PlayerController>()->KeyBind();

	Ptr<InputComponent> InputComponent = GetController<PlayerController>()->GetInputComponent();

	auto MappingContext = InputSystem::Instance().FindOrAddInputContext("MAPPING_CONTEXT");

	auto MoveRight = InputSystem::Instance().FindOrAddInputAction("MOVE_RIGHT");

	auto MoveLeft = InputSystem::Instance().FindOrAddInputAction("MOVE_LEFT");

	auto MoveUp = InputSystem::Instance().FindOrAddInputAction("MOVE_UP");

	auto MoveDown = InputSystem::Instance().FindOrAddInputAction("MOVE_DOWN");

	auto MoveJump = InputSystem::Instance().FindOrAddInputAction("MOVE_JUMP");



	MappingContext->BindInputAction(MoveRight, VK_RIGHT);

	MappingContext->BindInputAction(MoveLeft, VK_LEFT);

	MappingContext->BindInputAction(MoveUp, VK_UP);

	MappingContext->BindInputAction(MoveDown, VK_DOWN);
	
	MappingContext->BindInputAction(MoveJump, 'C');



	InputComponent->AddInputContext("MAPPING_CONTEXT");

	InputComponent->BindAction(MappingContext->GetName(), MoveRight->GetName(), INPUT_TYPE::HOLD, this, &Player::MoveRight);
	
	InputComponent->BindAction(MappingContext->GetName(), MoveRight->GetName(), INPUT_TYPE::UP,   this, &Player::MoveStop);
	

	InputComponent->BindAction(MappingContext->GetName(), MoveLeft->GetName(),  INPUT_TYPE::HOLD, this, &Player::MoveLeft);
	
	InputComponent->BindAction(MappingContext->GetName(), MoveLeft->GetName(),  INPUT_TYPE::UP,   this, &Player::MoveStop);
	

	InputComponent->BindAction(MappingContext->GetName(), MoveUp->GetName(),    INPUT_TYPE::HOLD, this, &Player::MoveUp);
	
	InputComponent->BindAction(MappingContext->GetName(), MoveUp->GetName(),    INPUT_TYPE::UP,   this, &Player::MoveStop);
	

	InputComponent->BindAction(MappingContext->GetName(), MoveDown->GetName(),  INPUT_TYPE::HOLD, this, &Player::MoveDown);
	
	InputComponent->BindAction(MappingContext->GetName(), MoveDown->GetName(),  INPUT_TYPE::UP,   this, &Player::MoveStop);
	

	InputComponent->BindAction(MappingContext->GetName(), MoveJump->GetName(),  INPUT_TYPE::HOLD, this, &Player::Jump);
	
	InputComponent->BindAction(MappingContext->GetName(), MoveJump->GetName(),  INPUT_TYPE::UP,   this, &Player::MoveStop);
	


	_Movement = CreateActorComponent<MovementComponent>("Movement");

	_Movement->SetUpdateComponent(_Root);

	_Movement->SetSpeed(100.f);

	

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

	Ptr<AABBCollisionComponent> LeftCollision = CreateSceneComponent<AABBCollisionComponent>("LeftCollision ");

	LeftCollision->SetBoxSize(8.f, 8.f);

	LeftCollision->SetRelativePosition(-20.f, 32.5f, 0.f);

	LeftCollision->AttachToComponent(_Root);

	LeftCollision->SetCollisionProfile("Player");

	Ptr<AABBCollisionComponent> RightCollision = CreateSceneComponent<AABBCollisionComponent>("RightCollision ");

	RightCollision->SetBoxSize(8.f, 8.f);

	RightCollision->SetRelativePosition(20.f, 32.5f, 0.f);

	RightCollision->AttachToComponent(_Root);

	RightCollision->SetCollisionProfile("Player");



	
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

void Player::MoveRight(float DeltaTime)
{
	_bReverse = true;

	_Movement->SetMoveAxis(FVector3D::Axis_X);

	Ptr<SpriteComponent> Sprite = FindSceneComponent<SpriteComponent>("PlayerSprite");

	if (Sprite)
	{
		Sprite->SetAnimationFlip(true);

		Sprite->ChangeAnimation("ARMED_WALK");
	}
}

void Player::MoveLeft(float DeltaTime)
{
	_bReverse = false;

	_Movement->SetMoveAxis(-FVector3D::Axis_X);

	Ptr<SpriteComponent> Sprite = FindSceneComponent<SpriteComponent>("PlayerSprite");

	if (Sprite)
	{
		Sprite->SetAnimationFlip(false);

		Sprite->ChangeAnimation("ARMED_WALK");
	}
}

void Player::MoveUp(float DeltaTime)
{
	// todo : Move Up과 Down은 메이플스토리에서 해당하는 로직이 조금씩 다르기 때문에 잠시 대기
}

void Player::MoveDown(float DeltaTime)
{

}

void Player::MoveStop(float DeltaTime)
{
	_Movement->Stop();

	Ptr<SpriteComponent> Sprite = FindSceneComponent<SpriteComponent>("PlayerSprite");

	if (Sprite)
	{
		Sprite->ChangeAnimation("ARMED_STAND");
	}
}

void Player::Jump(float DeltaTime)
{
}

void Player::Prone(float DeltaTime)
{
}
