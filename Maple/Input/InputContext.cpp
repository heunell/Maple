#include "pch.h"
#include "InputContext.h"

const std::string& InputContext::GetName() const
{
}

void InputContext::SetName(const std::string& name)
{
}

void InputContext::BindInputAction(Ptr<InputAction> action, uint8 key)
{
}

void InputContext::ChangeInputActionKey(const std::string& actionName, uint8 key)
{
}

FInputMapping* InputContext::FindMapping(const std::string& name)
{
}

void InputContext::Destroy()
{
}
