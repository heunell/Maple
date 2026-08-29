#pragma once
#include "ActorComponent.h"
#include "Input/InputContext.h"
#include "Input/InputSystem.h"
#include <functional>

struct FBindAction
{
    std::string _Name;

    FInputMapping* _Mapping = nullptr;
    // std::function<void(float)> _CallBack[INPUT_TYPE::END];
    std::function<void(Ptr<class InputAction>, INPUT_TYPE::eType)> _CallBack[INPUT_TYPE::END];
};


class InputComponent : public ActorComponent
{
public:
    InputComponent() = default;
    
    virtual ~InputComponent() = default;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, FBindAction>> _Binds;

    std::string _ActiveContext;

public:
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

    virtual void Tick(float DeltaTime) override;

    virtual void Destroy() override;

    void SetActiveContext(const std::string& Name);

    const std::string& GetActiveContext() const;

    void AddInputContext(const std::string& Name);

    void RemoveInputContext(const std::string& Name);

    void RefreshInputMapping(const std::string& ContextName, const FInputMapping& Mapping);

    // 람다 혹은 객체가 필요없는 함수 받기
    template<typename T>
    void BindAction(const std::string& ContextName, const std::string& ActionName, INPUT_TYPE::eType Type, T&& Function)
    {
        auto It = _Binds.find(ContextName);
        if (It == _Binds.end())
        {
            return;
        }

        _Binds[ContextName][ActionName]._CallBack[Type] = std::forward<T>(Function);
    }


    // 멤버함수 포인터 바인딩
    template<typename T>
    void BindAction(const std::string& ContextName, const std::string& ActionName, INPUT_TYPE::eType Type, T* Object, void(T::* MemberFunction)(Ptr<class InputAction>, INPUT_TYPE::eType))
    {
        auto It = _Binds.find(ContextName);
        if (It == _Binds.end())
        {
            return;
        }

        _Binds[ContextName][ActionName]._CallBack[Type] = std::bind(MemberFunction, Object, std::placeholders::_1, std::placeholders::_2);
    }

};
