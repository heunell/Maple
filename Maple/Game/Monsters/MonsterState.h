#pragma once

#include "Core/Object.h"

enum class eMonsterAnimationType // 몬스터의 State
{
    Idle, // = stand
    Move,
    Fly,
    Die,
    Respawn,

    End
};

enum class eBossAnimationType // 보스의 State
{
    Idle, // = stand
    Move, // 
    Fly,
    Die,

    Blade,
    Drangon,
    Laser,
    Sommon,

    End
};

class MonsterState : public Object
{
public:
    MonsterState() = default;
    virtual ~MonsterState() = default;

protected:
    uint8 _StateType = static_cast<uint8>(-1);

public:
    virtual void Destroy() override;

    virtual void PatternSequence();

    virtual void Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) {}

    virtual eMonsterAnimationType GetAnimation() const = 0;

    uint8 GetType() const;
};