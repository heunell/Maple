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
    std::unordered_map<std::string, Ptr<class InputContext>> _activeContexts;
    std::unordered_map<std::string, Ptr<class InputContext>> _allContexts;
    std::unordered_map<std::string, Ptr<class InputAction>>  _allActions;
    std::map<std::pair<int32, int32>, Weak<class InputComponent>> _inputComponents;
    Ptr<class Input> _input;
    
public:
    bool Init(Ptr<class Input> input);
    void Tick(float deltaTime);
    
public:
    void AddActiveInputContext(Ptr<class InputContext> constext);
    void RemoveActiveContext(const std::string& name);
    Ptr<class InputContext> FindOrAddInputContext(const std::string& name);
    Ptr<class InputAction>  FindOrAddInputAction (const std::string& name);
    bool IsActiveCOntext(const std::string& name);
    uint8 ConvertKey(uint8 key);
    void RegisterInputComponent(int32 actorID, Ptr<class InputComponent> inputComp);
    void RemoveInputComponent(int32 actorID, int32 componentID);
    void RefreshInputComponent(const std::string& contextName, const struct FInputMapping& mapping);
    const FVector2D& GetMouseWorldPos() const;
    const FVector2D& GetMousePos()      const;
    const FVector2D& GetMouseMove()     const;
    bool GetMouseDown(MOUSE_BUTTON_TYPE::Type type);
    bool GetMouseHold(MOUSE_BUTTON_TYPE::Type type);
    bool GetMouseUp  (MOUSE_BUTTON_TYPE::Type type);
    virtual void Destroy() override;
};
