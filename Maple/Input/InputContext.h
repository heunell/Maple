#pragma once
#include "Core/Object.h"
#include "InputAction.h"
#include <unordered_map>

struct FInputMapping
{
    Ptr<InputAction> _Action;
  
    bool _State[INPUT_TYPE::END] = {};
    
    uint8 _Key = 0;
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
    std::string _Name;

    std::unordered_map<std::string, FInputMapping> _Mappings;
    
public:
    const std::string& GetName() const;

    void SetName(const std::string& Name);

    void BindInputAction(Ptr<InputAction> Action, uint8 Key);

    void ChangeInputActionKey(const std::string& ActionName, uint8 Key);

    FInputMapping* FindMapping(const std::string& Name);

    virtual void Destroy() override;
};
