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
#include "Core/GameEngine.h"
#include "Core/TimeManager.h"
#include "Render/RenderManager.h"

bool Player::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	Pawn::Init(Id, Position, Scale, Rotator, Name);

	AddTag("Player");

	_Type = eActorType::Player;

	Ptr<StaticMeshComponent> MeshComponent = CreateSceneComponent<StaticMeshComponent>("PlayerMesh");

	MeshComponent->SetMesh("TexRect");

	MeshComponent->AddTexture(0, "TestCharacter", 0);

	SetRootComponent(MeshComponent);

	_Controller = GetLevel()->SpawnActor<PlayerController>("PlayerController", Position, Scale, Rotator);

	_Controller->SetPawn(This<Player>());

	Ptr<InputComponent> InputComponent = GetController<PlayerController>()->GetInputComponent();

	auto MappingContext = InputSystem::Instance().FindOrAddInputContext("MAPPING_CONTEXT");

	auto MoveRight = InputSystem::Instance().FindOrAddInputAction("MOVE_RIGHT");

	auto MoveLeft = InputSystem::Instance().FindOrAddInputAction("MOVE_LEFT");

	auto MoveUp = InputSystem::Instance().FindOrAddInputAction("MOVE_UP");

	auto MoveDown = InputSystem::Instance().FindOrAddInputAction("MOVE_DOWN");

	auto MoveJump = InputSystem::Instance().FindOrAddInputAction("MOVE_JUMP");

	auto Attack = InputSystem::Instance().FindOrAddInputAction("ATTACK");



	MappingContext->BindInputAction(MoveRight, VK_RIGHT);

	MappingContext->BindInputAction(MoveLeft, VK_LEFT);

	MappingContext->BindInputAction(MoveUp, VK_UP);

	MappingContext->BindInputAction(MoveDown, VK_DOWN);
	
	MappingContext->BindInputAction(MoveJump, 'C');

	MappingContext->BindInputAction(Attack, 'A');



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
	

	InputComponent->BindAction(MappingContext->GetName(), Attack->GetName(),    INPUT_TYPE::HOLD, this, &Player::Attack);
	
	InputComponent->BindAction(MappingContext->GetName(), Attack->GetName(),    INPUT_TYPE::UP,   this, &Player::MoveStop);


	_Movement = CreateActorComponent<MovementComponent>("Movement");

	_Movement->SetUpdateComponent(_Root);

	_Movement->SetSpeed(100.f);

	

	_Camera = CreateSceneComponent<CameraComponent>("Camera");

	_Camera->AttachToComponent(_Root);



	_AABBCollision = CreateSceneComponent<AABBCollisionComponent>("AABB");
	
	_AABBCollision->SetBoxSize(300.f, 300.f);

	_AABBCollision->AttachToComponent(_Root);

	_AABBCollision->SetCollisionProfile("Player");


	
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

void Player::MoveRight(float DeltaTime)
{
	_Movement->SetMoveAxis(FVector3D::Axis_X);
	
	// todo : 이미지 리소스가 추가되면 SpriteComponent를 작성하고 재생하기
}

void Player::MoveLeft(float DeltaTime)
{
	_Movement->SetMoveAxis(-FVector3D::Axis_X);
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
}

void Player::Attack(float DeltaTime)
{
}

void Player::Jump(float DeltaTime)
{
}

void Player::Prone(float DeltaTime)
{
}
