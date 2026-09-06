#include "pch.h"
#include "MonsterBase.h"
#include "UI/DamageNumber.h"
#include "World/Level.h"

void MonsterBase::TakeDamage(int64 Damage)
{
    if (Damage <= 0 || _Status.CurrentHP <= 0)
    {
        return;
    }

    if (Damage >= _Status.CurrentHP)
    {
        _Status.CurrentHP = 0;
    }
    else
    {
        _Status.CurrentHP -= Damage;
    }
}

int64 MonsterBase::GetHP() const
{
    return _Status.CurrentHP;
}

int64 MonsterBase::GetMaxHP() const
{
    return _Status.MaxHP;
}

void MonsterBase::ShowDamage(int64 Damage, const FVector3D& Position)
{
    if (!GetLevel())
    {
        return;
    }

    Ptr<DamageNumber> Number = Lock(_DamageNumbers[_DamageIndex]);

    if (Number && Number->IsActive())
    {
        Number->Destroy();

        Number->Remove();
    }

    Number = GetLevel()->SpawnActor<DamageNumber>("DamageNumber", FVector3D(Position._x, Position._y + _DamageIndex * 30.f, Position._z), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Damage);

    if (!Number)
    {
        _DamageNumbers[_DamageIndex].reset();
        return;
    }

    _DamageNumbers[_DamageIndex] = Number;

    _DamageIndex = (_DamageIndex + 1) % 8;
}