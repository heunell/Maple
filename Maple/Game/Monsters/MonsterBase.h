#pragma once
#include "Object/Actor.h"

// todo : Base 에서 일반Monster 와 Boss Monster로 자식을 둘 예정
// 해당 몬스터들의 공통된 부분을 다루면 좋을것같아서 하나 만듬 <- 언리얼에서 CharacterBase 를 만든것과 같은 원리

enum class eMonsterType
{
    Common,     // 일반몬스터, 패턴으로 인해 소환되어도 일반몬스터 취급한다. [1페이즈 골렘, 2페이즈 골렘, 1페이즈 버섯, ...]
    Boss,       // 보스몬스터 [ 루시드 ]

    End
};

struct FMonsterStatus
{
    int64 MaxHP     = 0.f;

    int64 CurrentHP = 0.f;
};

class MonsterBase : public Actor
{
public:
    MonsterBase() = default;
    virtual ~MonsterBase() = default;

protected:
    FMonsterStatus _Status;
};