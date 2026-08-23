#include "pch.h"
#include "PlayerController.h"
#include "Object/Pawn.h"
#include "Component/InputComponent.h"
#include "Component/SkillComponent.h"

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

	Ptr<SkillComponent> Skill = ControllPawn->FindActorComponent<SkillComponent>("Skill");

	if (!Skill)
	{
		return;
	}

	Ptr<InputContext> MappingContext = InputSystem::Instance().FindOrAddInputContext("MAPPING_CONTEXT");

	if (nullptr == MappingContext)
	{
		return;
	}

	Ptr<InputAction> SkillAction = InputSystem::Instance().FindOrAddInputAction("SongOfHeaven");

	MappingContext->BindInputAction(SkillAction, 'A');

	_Input->AddInputContext(MappingContext->GetName());

	_Input->BindAction(MappingContext->GetName(), SkillAction->GetName(), INPUT_TYPE::DOWN, Skill.get(), &SkillComponent::StartSkill);

	_Input->BindAction(MappingContext->GetName(), SkillAction->GetName(), INPUT_TYPE::HOLD, Skill.get(), &SkillComponent::UseSkill);

	_Input->BindAction(MappingContext->GetName(), SkillAction->GetName(), INPUT_TYPE::UP, Skill.get(), &SkillComponent::StopSkill);
}
