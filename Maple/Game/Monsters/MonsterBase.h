#pragma once
#include "Object/Actor.h"

// todo : Base 에서 일반Monster 와 Boss Monster로 자식을 둘 예정
// 해당 몬스터들의 공통된 부분을 다루면 좋을것같아서 하나 만듬 <- 언리얼에서 CharacterBase 를 만든것과 같은 원리

enum class eMonsterType
{
    Common,     // 일반몬스터, 패턴으로 인해 소환되어도 일반몬스터 취급한다. [1페이즈 골렘, 2페이즈 골렘, 1페이즈 버섯, ...]
    Boss,       // 보스몬스터 [ 루시드 ]
    Pattern,    // 플레이어에게 공격당하지 않고 오로지 패턴중 나오는 몬스터 타입, 자기 애니메이션이 끝나거나 생명주기가 다하면 사라진다.

    End
};

class MonsterBase : public Actor
{

};