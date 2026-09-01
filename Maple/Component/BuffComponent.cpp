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

    for (auto& Buff : _Buffs)
    {
        FBuffData& Data = Buff.second;

        Data.RemainTime -= DeltaTime;

        if (Data.RemainTime <= 0.f)
        {
            ExpiredBuffs.push_back(Buff.first);
        }
    }

    // 순회 중 Map을 직접 지우지 않고 만료 목록을 따로 제거한다.
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
    if (Data.Type == eBuffType::END || Data.Category == eBuffCategory::END || Data.Duration <= 0.f)
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
        // 같은 Buff를 다시 사용하면 중첩하지 않고 시간을 갱신한다.
        It->second.Category   = Data.Category;

        It->second.Duration   = Data.Duration;
        
        It->second.RemainTime = Data.Duration;
        
        It->second.CanCleanse = Data.CanCleanse;
        
        It->second.OnStart    = Data.OnStart;
        
        It->second.OnEnd      = Data.OnEnd;

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

    Ptr<Player> PlayerOwner = GetPlayer();

    // 콜백에서 Buff 목록을 변경할 수 있으므로 먼저 복사한다.
    std::function<void(Ptr<Player>)> OnEnd = It->second.OnEnd;

    _Buffs.erase(It);

    if (OnEnd && PlayerOwner)
    {
        OnEnd(PlayerOwner);
    }

    return true;
}

void BuffComponent::RemoveCleanseableDebuffs()
{
    std::vector<eBuffType> RemoveBuffs;

    for (const auto& Buff : _Buffs)
    {
        const FBuffData& Data = Buff.second;

        if (Data.Category == eBuffCategory::Debuff && Data.CanCleanse)
        {
            RemoveBuffs.push_back(Buff.first);
        }
    }

    for (eBuffType Type : RemoveBuffs)
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

Ptr<class Player> BuffComponent::GetPlayer() const
{
    return Cast<Actor, Player>(GetOwner());
}
