#include "pch.h"
#include "BuffComponent.h"
#include "Game/Character/Player.h"

bool BuffComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    if (!ActorComponent::Init(Id, Name, Owner))
    {
        return false;
    }

    return GetPlayer() != nullptr;
}

void BuffComponent::Tick(float DeltaTime)
{
    ActorComponent::Tick(DeltaTime);

    if (DeltaTime <= 0.f)
    {
        return;
    }

    std::vector<eBuffType> ExpiredBuffs;

    for (auto& [Type, Data] : _Buffs)
    {
        Data.RemainTime -= DeltaTime;

        if (Data.RemainTime <= 0.f)
        {
            ExpiredBuffs.push_back(Type);
        }
    }

    for (eBuffType Type : ExpiredBuffs)
    {
        RemoveBuff(Type);
    }
}

void BuffComponent::Destroy()
{
    _Buffs.clear();

    ActorComponent::Destroy();
}

bool BuffComponent::ApplyBuff(const FBuffData& Data)
{
    if (Data.Type == eBuffType::END ||
        Data.Category == eBuffCategory::END ||
        Data.Duration <= 0.f)
    {
        return false;
    }

    Ptr<Player> PlayerOwner = GetPlayer();

    if (!PlayerOwner)
    {
        return false;
    }

    auto It = _Buffs.find(Data.Type);

    if (It != _Buffs.end())
    {
        It->second.Category = Data.Category;

        It->second.Duration = Data.Duration;
        
        It->second.RemainTime = Data.Duration;
        
        It->second.OnStart = Data.OnStart;
        
        It->second.OnEnd = Data.OnEnd;

        return true;
    }

    FBuffData NewData = Data;

    NewData.RemainTime = NewData.Duration;

    _Buffs.emplace(NewData.Type, NewData);

    if (NewData.OnStart)
    {
        NewData.OnStart(PlayerOwner);
    }

    return true;
}

bool BuffComponent::RemoveBuff(eBuffType Type)
{
    auto It = _Buffs.find(Type);

    if (It == _Buffs.end())
    {
        return false;
    }

    std::function<void(Ptr<Player>)> OnEnd = It->second.OnEnd;
    
    _Buffs.erase(It);

    Ptr<Player> PlayerOwner = GetPlayer();

    if (OnEnd && PlayerOwner)
    {
        OnEnd(PlayerOwner);
    }

    return true;
}

void BuffComponent::RemoveDebuffs()
{
    std::vector<eBuffType> Debuffs;

    for (const auto& [Type, Data] : _Buffs)
    {
        if (Data.Category == eBuffCategory::Debuff)
        {
            Debuffs.push_back(Type);
        }
    }

    for (eBuffType Type : Debuffs)
    {
        RemoveBuff(Type);
    }
}

bool BuffComponent::IsActive(eBuffType Type) const
{
    return _Buffs.find(Type) != _Buffs.end();
}

const FBuffData* BuffComponent::FindBuff(eBuffType Type) const
{
    auto It = _Buffs.find(Type);

    if (It == _Buffs.end())
    {
        return nullptr;
    }

    return &It->second;
}

const std::map<eBuffType, FBuffData>& BuffComponent::GetBuffs() const
{
    return _Buffs;
}

Ptr<Player> BuffComponent::GetPlayer() const
{
    return Cast<Actor, Player>(GetOwner());
}