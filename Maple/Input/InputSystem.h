#pragma once
#include "Common/Singleton.h"
#include "Core/Defines.h"
#include "Common/Info.h"
#include <unordered_map>

class InputSystem : public Singleton<InputSystem>
{
public:
    InputSystem() = default;
    virtual ~InputSystem() = default;
    InputSystem(InputSystem const&) = delete;
    InputSystem(InputSystem&&) = delete;
    InputSystem& operator=(InputSystem const&) = delete;
    InputSystem& operator=(InputSystem&&) = delete;
    
private:
    std::unordered_map<std::string, Ptr<class InputContext>> _ActiveContexts;
   
    std::unordered_map<std::string, Ptr<class InputContext>> _AllContexts;
    
    std::unordered_map<std::string, Ptr<class InputAction>>  _AllActions;
    
    std::map<std::pair<int32, int32>, Weak<class InputComponent>> _InputComponents;
    
    Ptr<class Input> _Input;
    
public:
    bool Init(Ptr<class Input> Input);
    
    void Tick(float DeltaTime);
    
public:
    void AddActiveInputContext(Ptr<class InputContext> Context);
    
    void RemoveActiveContext(const std::string& Name);
    
    Ptr<class InputContext> FindOrAddInputContext(const std::string& Name);
    
    Ptr<class InputAction>  FindOrAddInputAction (const std::string& Name);
    
    bool IsActiveCOntext(const std::string& Name);
    
    uint8 ConvertKey(uint8 Key);
    
    void RegisterInputComponent(int32 ActorID, Ptr<class InputComponent> InputComp);
    
    void RemoveInputComponent(int32 ActorID, int32 ComponentID);
    
    void RefreshInputComponent(const std::string& ContextName, const struct FInputMapping& Mapping);
    
    const FVector2D& GetMouseWorldPosition() const;
    
    const FVector2D& GetMousePosition()      const;
    
    const FVector2D& GetMouseMove()     const;
    
    bool GetMouseDown(MOUSE_BUTTON_TYPE::Type Type);
    
    bool GetMouseHold(MOUSE_BUTTON_TYPE::Type Type);
    
    bool GetMouseUp  (MOUSE_BUTTON_TYPE::Type Type);
    
    virtual void Destroy() override;
};