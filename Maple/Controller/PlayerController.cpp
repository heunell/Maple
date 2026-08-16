#include "pch.h"
#include "PlayerController.h"
#include "Component/InputComponent.h"

bool PlayerController::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotation, const std::string& Name)
{
	Controller::Init(Id, Position, Scale, Rotation, Name);

	_Input = CreateActorComponent<InputComponent>("Input");

	InputSystem::Instance().RegisterInputComponent(GetActorID(), _Input);

	return true;
}

void PlayerController::Tick(float DeltaTime)
{
	Controller::Tick(DeltaTime);
}

void PlayerController::Collision(float DeltaTime)
{
	Controller::Collision(DeltaTime);
}

void PlayerController::Render(float DeltaTime)
{
	Controller::Render(DeltaTime);
}

void PlayerController::Destroy()
{
	Controller::Destroy();
}

Ptr<InputComponent> PlayerController::GetInputComponent() const
{
	return _Input;
}
