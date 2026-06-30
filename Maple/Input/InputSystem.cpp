#include "pch.h"
#include "InputSystem.h"
#include "InputContext.h"
#include "InputAction.h"
#include "Input.h"
#include "Component/InputComponent.h"

bool InputSystem::Init(Ptr<class Input> input)
{
}

void InputSystem::Tick(float deltaTime)
{
}

void InputSystem::AddActiveInputContext(Ptr<class InputContext> constext)
{
}

void InputSystem::RemoveActiveContext(const std::string& name)
{
}

Ptr<class InputContext> InputSystem::FindOrAddInputContext(const std::string& name)
{
}

Ptr<class InputAction> InputSystem::FindOrAddInputAction(const std::string& name)
{
}

bool InputSystem::IsActiveCOntext(const std::string& name)
{
}

uint8 InputSystem::ConvertKey(uint8 key)
{
}

void InputSystem::RegisterInputComponent(int32 actorID, Ptr<class InputComponent> inputComp)
{
}

void InputSystem::RemoveInputComponent(int32 actorID, int32 componentID)
{
}

void InputSystem::RefreshInputComponent(const std::string& contextName, const struct FInputMapping& mapping)
{
}

const FVector2D& InputSystem::GetMouseWorldPos() const
{
}

const FVector2D& InputSystem::GetMousePos() const
{
}

const FVector2D& InputSystem::GetMouseMove() const
{
}

bool InputSystem::GetMouseDown(MOUSE_BUTTON_TYPE::Type type)
{
}

bool InputSystem::GetMouseHold(MOUSE_BUTTON_TYPE::Type type)
{
}

bool InputSystem::GetMouseUp(MOUSE_BUTTON_TYPE::Type type)
{
}

void InputSystem::Destroy()
{
}
