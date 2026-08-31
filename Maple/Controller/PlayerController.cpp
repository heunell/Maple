#include "pch.h"
#include "PlayerController.h"
#include "Object/Pawn.h"
#include "Component/InputComponent.h"
#include "Component/PlayerComponent.h"
#include "Game/Map/Gate/GateInteractComponent.h"

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

void PlayerController::KeyBind()
{
	Ptr<Pawn> ControllPawn = GetPawn();

	if (!ControllPawn)
	{
		return;
	}

	Ptr<PlayerComponent> PlayerState = ControllPawn->FindActorComponent<PlayerComponent>("PlayerState");

	if (!PlayerState)
	{
		return;
	}

	Ptr<GateInteractComponent> GateInteract = ControllPawn->FindActorComponent<GateInteractComponent>("GateInteract");
	
	if (!GateInteract)
	{
		return;
	}

	Ptr<InputContext> MappingContext = InputSystem::Instance().FindOrAddInputContext("MAPPING_CONTEXT");

	if (nullptr == MappingContext)
	{
		return;
	}

	Ptr<InputAction> PortalAction = InputSystem::Instance().FindOrAddInputAction("PORTAL_INTERACT");

	MappingContext->BindInputAction(PortalAction, VK_SPACE);

	Ptr<InputAction> SkillAction = InputSystem::Instance().FindOrAddInputAction("SongOfHeaven");

	MappingContext->BindInputAction(SkillAction, 'A');

	_Input->AddInputContext(MappingContext->GetName());

	_Input->BindAction(MappingContext->GetName(), PortalAction->GetName(), INPUT_TYPE::DOWN, GateInteract.get(), &GateInteractComponent::HandleInput);

	_Input->BindAction(MappingContext->GetName(), SkillAction->GetName(),  INPUT_TYPE::DOWN, PlayerState.get(), &PlayerComponent::HandleInput);

	_Input->BindAction(MappingContext->GetName(), SkillAction->GetName(),  INPUT_TYPE::HOLD, PlayerState.get(), &PlayerComponent::HandleInput);

	_Input->BindAction(MappingContext->GetName(), SkillAction->GetName(),  INPUT_TYPE::UP,   PlayerState.get(), &PlayerComponent::HandleInput);
}
