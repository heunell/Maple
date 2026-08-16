#include "pch.h"
#include "InputComponent.h"

bool InputComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    ActorComponent::Init(Id, Name, Owner);

    return true;
}

void InputComponent::Tick(float DeltaTime)
{
    ActorComponent::Tick(DeltaTime);

    auto It = _Binds.find(_ActiveContext);

    if (It == _Binds.end())
    {
        return;
    }

    for (auto& BindIt : It->second)
    {
        FInputMapping* Mapping = BindIt.second._Mapping;
        if (!Mapping)
        {
            continue;
        }

        for (int i = 0; i < INPUT_TYPE::END; ++i)
        {
            if (Mapping->_State[i] && BindIt.second._CallBack[i])
            {
                BindIt.second._CallBack[i](DeltaTime);
            }
        }
    }
}

void InputComponent::Destroy()
{
    _Binds.clear();
}

void InputComponent::SetActiveContext(const std::string& Name)
{
    _ActiveContext = Name;
}

const std::string& InputComponent::GetActiveContext() const
{
    return _ActiveContext;
}

void InputComponent::AddInputContext(const std::string& Name)
{
    if (_Binds.find(Name) != _Binds.end())
    {
        return;
    }

    Ptr<InputContext> FoundContext = InputSystem::Instance().FindOrAddInputContext(Name);

    if (!FoundContext)
    {
        return;
    }

    for (auto& It : FoundContext->_Mappings)
    {
        FBindAction Bind;
        
        Bind._Mapping = &It.second;

        Bind._Name = Bind._Mapping->_Action->GetName();

        _Binds[Name][Bind._Name] = Bind;
    }

    if (_ActiveContext.empty())
    {
        _ActiveContext = Name;
    }

    InputSystem::Instance().AddActiveInputContext(FoundContext);
}

void InputComponent::RemoveInputContext(const std::string& Name)
{
    _Binds.erase(Name);

    InputSystem::Instance().RemoveActiveContext(Name);
}

void InputComponent::RefreshInputMapping(const std::string& ContextName, const FInputMapping& Mapping)
{
    auto It = _Binds.find(ContextName);

    if (_Binds.end() == It)
    {
        return;
    }

    auto& ItArray = It->second;

    auto FoundItArray = ItArray.find(Mapping._Action->GetName());

    if (ItArray.end() != FoundItArray)
    {
        FBindAction& BindAction = FoundItArray->second;

        BindAction._Mapping = const_cast<FInputMapping*>(&Mapping);
    }
    else
    {
        FBindAction Bind;

        Bind._Mapping = const_cast<FInputMapping*>(&Mapping);

        Bind._Name = Bind._Mapping->_Action->GetName();

        _Binds[ContextName][Bind._Name] = Bind;
    }
}
