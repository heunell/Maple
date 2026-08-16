#pragma once
#include "Core/Object.h"

class InputAction : public Object
{
    friend class InputSystem;
public:
    InputAction() = default;
    virtual ~InputAction() = default;
    InputAction(InputAction const&) = default;
    InputAction(InputAction&&) = default;
    InputAction& operator=(InputAction const&) = default;
    InputAction& operator=(InputAction&&) = default;
    
private:
    std::string _Name;
    
public:
    void SetName(const std::string& Name);
   
    const std::string& GetName() const;
    
    virtual void Destroy() override;
};
