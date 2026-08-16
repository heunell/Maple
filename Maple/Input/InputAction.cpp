#include "pch.h"
#include "InputAction.h"

void InputAction::SetName(const std::string& Name)
{
    _Name = Name;
}

const std::string& InputAction::GetName() const
{
    return _Name;
}

void InputAction::Destroy()
{}
