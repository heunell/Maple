#include "pch.h"
#include "InputContext.h"
#include "InputSystem.h"

const std::string& InputContext::GetName() const
{
	return _Name;
}

void InputContext::SetName(const std::string& Name)
{
	_Name = Name;
}

void InputContext::BindInputAction(Ptr<InputAction> Action, uint8 Key)
{
	auto It = _Mappings.find(Action->GetName());

	if (_Mappings.end() != It)
	{
		return;
	}

	FInputMapping Mapping;

	Mapping._Action = Action;

	Mapping._Key = InputSystem::Instance().ConvertKey(Key);

	_Mappings[Action->GetName()] = Mapping;

	InputSystem::Instance().RefreshInputComponent(_Name, _Mappings[Action->GetName()]);
}

void InputContext::ChangeInputActionKey(const std::string& ActionName, uint8 Key)
{
	auto It = _Mappings.find(ActionName);

	if (_Mappings.end() != It)
	{
		_Mappings[ActionName]._Key = InputSystem::Instance().ConvertKey(Key);
	}
}

FInputMapping* InputContext::FindMapping(const std::string& Name)
{
	auto It = _Mappings.find(Name);

	if (_Mappings.end() == It)
	{
		return nullptr;
	}

	return &(It->second);
}

void InputContext::Destroy()
{
	_Mappings.clear();
}
