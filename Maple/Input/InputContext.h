#pragma once
#include "Core/Object.h"
#include "InputAction.h"
#include <unordered_map>

struct FInputMapping
{
    Ptr<InputAction> _action;
    bool _state[INPUT_TYPE::END] = {};
    uint8 _key = 0;
};

class InputContext : public Object
{
    friend class InputSystem;
    friend class InputComponent;
public:
    InputContext() = default;
    virtual ~InputContext() = default;
    InputContext(InputContext const&) = default;
    InputContext(InputContext&&) = default;
    InputContext& operator=(InputContext const&) = default;
    InputContext& operator=(InputContext&&) = default;
    
private:
    std::string _name;
    std::unordered_map<std::string, FInputMapping> _mappings;
    
public:
    const std::string& GetName() const;
    void SetName(const std::string& name);
    void BindInputAction(Ptr<InputAction> action, uint8 key);
    void ChangeInputActionKey(const std::string& actionName, uint8 key);
    FInputMapping* FindMapping(const std::string& name);
    virtual void Destroy() override;
};
