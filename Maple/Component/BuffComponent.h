#pragma once

#include "ActorComponent.h"
#include <functional>
#include <map>

enum class eBuffType
{
    SharpEyes,
    CKnight,
    StormBringer,
    GloryOfGuardians,
    DebuffResistance,

    END
};

enum class eBuffCategory
{
    Buff,
    Debuff,

    END
};

struct FBuffData
{
    eBuffType Type = eBuffType::END;
    
    eBuffCategory Category = eBuffCategory::END;

    float Duration = 0.f;
    
    float RemainTime = 0.f;

    std::function<void(Ptr<class Player>)> OnStart;
    
    std::function<void(Ptr<class Player>)> OnEnd;
};

class BuffComponent : public ActorComponent
{
public:
    BuffComponent() = default;
    virtual ~BuffComponent() = default;

private:
    std::map<eBuffType, FBuffData> _Buffs;

public:
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

    virtual void Tick(float DeltaTime) override;

    virtual void Destroy() override;

    bool ApplyBuff(const FBuffData& Data);

    bool RemoveBuff(eBuffType Type);
    
    void RemoveDebuffs();

    bool IsActive(eBuffType Type) const;
    
    const FBuffData* FindBuff(eBuffType Type) const;

    const std::map<eBuffType, FBuffData>& GetBuffs() const;

    Ptr<class Player> GetPlayer() const;
};