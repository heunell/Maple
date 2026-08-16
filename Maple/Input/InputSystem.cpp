#include "pch.h"
#include "InputSystem.h"
#include "InputContext.h"
#include "InputAction.h"
#include "Input.h"
#include "Component/InputComponent.h"

bool InputSystem::Init(Ptr<class Input> Input)
{
	_Input = Input;

	return true;
}

void InputSystem::Tick(float DeltaTime)
{
	for (auto& It : _ActiveContexts)
	{
		for (auto& SubIt : It.second->_Mappings)
		{
			FInputMapping& Mapping = SubIt.second;

			uint8 Key = Mapping._Key;

            if (Key == DIK_MOUSELBUTTON)
            {
                Mapping._State[INPUT_TYPE::DOWN] = _Input->GetMouseDown(MOUSE_BUTTON_TYPE::LButton);

                Mapping._State[INPUT_TYPE::HOLD] = _Input->GetMouseHold(MOUSE_BUTTON_TYPE::LButton);

                Mapping._State[INPUT_TYPE::UP]   = _Input->GetMouseUp(MOUSE_BUTTON_TYPE::LButton);
            }
            else if (Key == DIK_MOUSERBUTTON)
            {
                Mapping._State[INPUT_TYPE::DOWN] = _Input->GetMouseDown(MOUSE_BUTTON_TYPE::RButton);

                Mapping._State[INPUT_TYPE::HOLD] = _Input->GetMouseHold(MOUSE_BUTTON_TYPE::RButton);

                Mapping._State[INPUT_TYPE::UP]   = _Input->GetMouseUp(MOUSE_BUTTON_TYPE::RButton);
            }
            else if (Key == DIK_MOUSEWHEEL)
            {
                Mapping._State[INPUT_TYPE::DOWN] = _Input->GetMouseDown(MOUSE_BUTTON_TYPE::Wheel);

                Mapping._State[INPUT_TYPE::HOLD] = _Input->GetMouseHold(MOUSE_BUTTON_TYPE::Wheel);

                Mapping._State[INPUT_TYPE::UP]   = _Input->GetMouseUp(MOUSE_BUTTON_TYPE::Wheel);
            }
            else
            {
                bool keyPush = _Input->GetKeyState(Key);

                if (keyPush)
                {
                    if (!Mapping._State[INPUT_TYPE::DOWN] && !Mapping._State[INPUT_TYPE::HOLD])
                    {
                        Mapping._State[INPUT_TYPE::DOWN] = true;

                        Mapping._State[INPUT_TYPE::HOLD] = true;
                    }
                    else
                    {
                        Mapping._State[INPUT_TYPE::DOWN] = false;
                    }
                }
                else
                {
                    if (Mapping._State[INPUT_TYPE::HOLD])
                    {
                        Mapping._State[INPUT_TYPE::DOWN] = false;

                        Mapping._State[INPUT_TYPE::HOLD] = false;
                        
                        Mapping._State[INPUT_TYPE::UP] = true;
                    }
                    else
                    {
                        Mapping._State[INPUT_TYPE::UP] = false;
                    }
                }
            }
		}
	}
}

void InputSystem::AddActiveInputContext(Ptr<class InputContext> Context)
{
    _ActiveContexts[Context->_Name] = Context;
}

void InputSystem::RemoveActiveContext(const std::string& Name)
{
    _ActiveContexts.erase(Name);
}

Ptr<class InputContext> InputSystem::FindOrAddInputContext(const std::string& Name)
{
    auto It = _AllContexts.find(Name);

    if (_AllContexts.end() != It)
    {
        return It->second;
    }

    Ptr<InputContext> Context = New<InputContext>();

    Context->_Name = Name;

    _AllContexts[Name] = Context;

    return Context;
}

Ptr<class InputAction> InputSystem::FindOrAddInputAction(const std::string& Name)
{
    auto It = _AllActions.find(Name);

    if (_AllActions.end() != It)
    {
        return It->second;
    }

    Ptr<InputAction> Context = New<InputAction>();

    Context->_Name = Name;

    _AllActions[Name] = Context;

    return Context;
}

bool InputSystem::IsActiveCOntext(const std::string& Name)
{
    return _ActiveContexts.contains(Name);
}

uint8 InputSystem::ConvertKey(uint8 Key)
{
    return _Input->ConvertKey(Key);
}

void InputSystem::RegisterInputComponent(int32 ActorID, Ptr<class InputComponent> InputComponent)
{
    std::pair<int32, int32> key(ActorID, InputComponent->GetComponentID()); 

    _InputComponents[key] = InputComponent;
}

void InputSystem::RemoveInputComponent(int32 ActorID, int32 ComponentID)
{
    std::pair<int32, int32> key(ActorID, ComponentID);

    _InputComponents.erase(key);
}

void InputSystem::RefreshInputComponent(const std::string& ContextName, const struct FInputMapping& Mapping)
{
    for (auto It : _InputComponents)
    {
        Ptr<InputComponent> InputComponents = Lock<InputComponent>(It.second);

        if (nullptr == InputComponents)
        {
            continue;
        }

        InputComponents->RefreshInputMapping(ContextName, Mapping);
    }
}

const FVector2D& InputSystem::GetMouseWorldPosition() const
{
    return _Input->GetMouseWorldPosition();
}

const FVector2D& InputSystem::GetMousePosition() const
{
    return _Input->GetMousePosition();
}

const FVector2D& InputSystem::GetMouseMove() const
{
    return _Input->GetMouseMove();
}

bool InputSystem::GetMouseDown(MOUSE_BUTTON_TYPE::Type Type)
{
    return _Input->GetMouseDown(Type);
}

bool InputSystem::GetMouseHold(MOUSE_BUTTON_TYPE::Type Type)
{
    return _Input->GetMouseHold(Type);
}

bool InputSystem::GetMouseUp(MOUSE_BUTTON_TYPE::Type Type)
{
    return _Input->GetMouseUp(Type);
}

void InputSystem::Destroy()
{
    _ActiveContexts.clear();

    for (auto& It : _AllContexts)
    {
        DESTROY(It.second)
    }

    for (auto& It : _AllActions)
    {
        DESTROY(It.second);
    }

    _AllContexts.clear();

    _AllActions.clear();
}
