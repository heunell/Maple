#include "pch.h"
#include "TriflingWindActor.h"
#include "Component/SpriteComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "Game/Monsters/MonsterBase.h"
#include "World/Level.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

bool TriflingWindActor::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    AddTag("PlayerAttack");

    AddTag("PlayerProjectile");

    AddTag("TriflingWind");

    Ptr<Animation2DData> FlyData = ANIMATION_MANAGER->FindAnimation("TriflingWind.Fly");

    if (!FlyData)
    {
        Ptr<Animation2DData> SpawnData = ANIMATION_MANAGER->FindAnimation("TriflingWind.Spawn2");

        if (!SpawnData || !SpawnData->GetTexture() || SpawnData->GetFrameCount() < 18)
        {
            return false;
        }

        if (!ANIMATION_MANAGER->CreateAnimation("TriflingWind.Fly", SpawnData->GetAnimationTextureType()))
        {
            return false;
        }

        FlyData = ANIMATION_MANAGER->FindAnimation("TriflingWind.Fly");

        if (!FlyData)
        {
            return false;
        }

        FlyData->SetTexture(SpawnData->GetTexture());

        for (int32 i = 7; i <= 17; ++i)
        {
            const FAnimationFrame& Frame = SpawnData->GetFrame(i);

            FlyData->AddFrame(Frame._UVLeftTop, Frame._UVRightBottom, Frame._Size, Frame._Offset, Frame._Delay);
        }
    }

    if (FlyData->GetFrameCount() != 11 || !FlyData->GetTexture())
    {
        return false;
    }

    for (const char* AnimationName : { "TriflingWind.Die1.1", "TriflingWind.Die1.2", "TriflingWind.Die2.1", "TriflingWind.Die2.2" })
    {
        Ptr<Animation2DData> DieData = ANIMATION_MANAGER->FindAnimation(AnimationName);

        if (!DieData || DieData->GetFrameCount() < 2 || !DieData->GetTexture())
        {
            return false;
        }
    }

    _Sprite = CreateSceneComponent<SpriteComponent>("TriflingWindSprite");

    if (!_Sprite)
    {
        return false;
    }

    _Sprite->AttachToComponent(GetRoot());

    _Sprite->SetRenderLayerName("Default");

    _Sprite->AddAnimationSequence(FlyData, true);

    _Sprite->AddAnimationSequence("TriflingWind.Die1.1", false);

    _Sprite->AddAnimationSequence("TriflingWind.Die1.2", false);

    _Sprite->AddAnimationSequence("TriflingWind.Die2.1", false);

    _Sprite->AddAnimationSequence("TriflingWind.Die2.2", false);

    _Sprite->SetEnable(false);

    SetEnable(false);

    return true;
}

void TriflingWindActor::Tick(float DeltaTime)
{
    if (!IsActive() || !IsEnable() || !_Sprite)
    {
        return;
    }

    if (!_Dying)
    {
        _ElapsedTime += DeltaTime;

        if (_ElapsedTime >= 5.f)
        {
            End();
        }
        else
        {
            UpdateMovement(DeltaTime);
        }
    }

    Actor::Tick(DeltaTime);

    if (_Dying)
    {
        Ptr<Animation2D> Animation = _Sprite->GetAnimation();

        if (!Animation || Animation->IsFinished())
        {
            Destroy();

            Remove();
        }
    }
}

void TriflingWindActor::Destroy()
{
    SetEnable(false);

    SetActive(false);

    _Target.reset();

    if (_Root)
    {
        _Root->Destroy();

        _Root.reset();
    }

    _Sprite = nullptr;

    Actor::Destroy();
}

void TriflingWindActor::Start(Ptr<class MonsterBase> Target, bool Enhanced, float Angle)
{
    if (!_Sprite || !Target || !Target->IsActive() || !Target->IsEnable() || Target->GetHP() <= 0)
    {
        Destroy();

        Remove();

        return;
    }

    _Target = Target;

    _Enhanced = Enhanced;

    _Angle = Angle;

    _TurnResponse = 0.55f;

    _ElapsedTime = 0.f;

    _Dying = false;

    const float Radian = DirectX::XMConvertToRadians(_Angle);

    _Direction = FVector2D(cosf(Radian), sinf(Radian));

    _FacingRight = _Direction._x >= 0.f;

    _Sprite->ChangeAnimation("TriflingWind.Fly");

    _Sprite->SetAnimationFrame(0);

    _Sprite->SetPlay("TriflingWind.Fly", true);

    _Sprite->SetEnable(true);

    SetEnable(true);

    UpdateVisual();
}

Ptr<class MonsterBase> TriflingWindActor::FindTarget() const
{
    Ptr<Level> CurrentLevel = GetLevel();

    if (!CurrentLevel)
    {
        return nullptr;
    }

    std::vector<Ptr<Actor>> Actors;

    CurrentLevel->FindActors("Boss", Actors);

    Ptr<MonsterBase> Target = nullptr;

    float NearestDistance = FLT_MAX;

    const FVector3D Position = GetWorldPosition();

    for (const Ptr<Actor>& Actor : Actors)
    {
        Ptr<MonsterBase> Monster = Cast<class Actor, MonsterBase>(Actor);

        if (!Monster || !Monster->IsActive() || !Monster->IsEnable() || Monster->GetHP() <= 0)
        {
            continue;
        }

        if (!Monster->GetRoot())
        {
            continue;
        }

        std::vector<Ptr<SceneComponent>> Components;

        Components.push_back(Monster->GetRoot());

        bool InRange = false;

        for (size_t i = 0; i < Components.size(); ++i)
        {
            Ptr<SceneComponent> Component = Components[i];

            if (!Component || !Component->IsActive() || !Component->IsEnable())
            {
                continue;
            }

            Ptr<AABBCollisionComponent> Collision = Cast<SceneComponent, AABBCollisionComponent>(Component);

            if (Collision)
            {
                const FAABB2D& Box = Collision->GetBox();

                if (Box._Max._x >= Position._x - 750.f &&
                    Box._Min._x <= Position._x + 750.f &&
                    Box._Max._y >= Position._y - 500.f &&
                    Box._Min._y <= Position._y + 500.f)
                {
                    InRange = true;

                    break;
                }
            }

            for (const auto& Child : Component->GetChilds())
            {
                Components.push_back(Child.second);
            }
        }

        if (!InRange)
        {
            continue;
        }

        const FVector3D MonsterPosition = Monster->GetWorldPosition();

        const float X = MonsterPosition._x - Position._x;

        const float Y = MonsterPosition._y - Position._y;

        const float Distance = X * X + Y * Y;

        if (Distance < NearestDistance)
        {
            NearestDistance = Distance;

            Target = Monster;
        }
    }

    return Target;
}

void TriflingWindActor::UpdateMovement(float DeltaTime)
{
    Ptr<MonsterBase> Target = Lock<MonsterBase>(_Target);

    if (!Target || !Target->IsActive() || !Target->IsEnable() || Target->GetHP() <= 0)
    {
        Target = FindTarget();

        _Target = Target;
    }

    if (!Target)
    {
        End();

        return;
    }

    FVector3D TargetPosition = Target->GetWorldPosition();

    TargetPosition._y += 40.f;

    const FVector3D Position = GetWorldPosition();

    const float X = TargetPosition._x - Position._x;

    const float Y = TargetPosition._y - Position._y;

    const float Distance = X * X + Y * Y;

    if (Distance > 0.0001f)
    {
        const float TargetAngle = DirectX::XMConvertToDegrees(atan2f(Y, X));

        const float AngleDifference = std::remainder(TargetAngle - _Angle, 360.f);

        _TurnResponse = std::clamp(_TurnResponse + 9.5f * DeltaTime, 0.55f, 13.f);

        float Alpha = std::clamp(_TurnResponse * DeltaTime, 0.f, 0.22f);

        if (Distance <= 130.f * 130.f)
        {
            Alpha = 0.42f;
        }

        _Angle = std::remainder(_Angle + AngleDifference * Alpha, 360.f);

        const float Radian = DirectX::XMConvertToRadians(_Angle);

        _Direction = FVector2D(cosf(Radian), sinf(Radian));
    }

    const float Speed = _Enhanced ? 680.f : 560.f;

    AddWorldPosition(_Direction._x * Speed * DeltaTime, _Direction._y * Speed * DeltaTime, 0.f);

    UpdateVisual();

    const FVector3D NextPosition = GetWorldPosition();

    const float HitX = TargetPosition._x - NextPosition._x;

    const float HitY = TargetPosition._y - NextPosition._y;

    if (HitX * HitX + HitY * HitY <= 52.f * 52.f)
    {
        Hit();
    }
}

void TriflingWindActor::UpdateVisual()
{
    if (!_Sprite)
    {
        return;
    }

    if (fabsf(_Direction._x) > 0.02f)
    {
        _FacingRight = _Direction._x >= 0.f;
    }

    float Rotation = DirectX::XMConvertToDegrees(atan2f(_Direction._y, fabsf(_Direction._x)));

    if (!_FacingRight)
    {
        Rotation = -Rotation;
    }

    Rotation = std::clamp(Rotation, -82.f, 82.f);

    _Sprite->SetAnimationFlip(_FacingRight);

    _Sprite->SetRelativeRotation(0.f, 0.f, Rotation);
}

void TriflingWindActor::Hit()
{
    if (_Dying)
    {
        return;
    }

    Ptr<MonsterBase> Target = Lock<MonsterBase>(_Target);

    End();

    if (!Target || !Target->IsActive() || !Target->IsEnable() || Target->GetHP() <= 0)
    {
        return;
    }

    static std::mt19937 RandomEngine(std::random_device{}());

    std::uniform_int_distribution<int64> DamageRange(_Enhanced ? 1200000 : 850000, _Enhanced ? 1700000 : 1250000);

    const int64 Damage = DamageRange(RandomEngine);

    Target->TakeDamage(Damage);

    FVector3D DamagePosition = Target->GetWorldPosition();

    DamagePosition._y += 85.f;

    Target->ShowDamage(Damage, DamagePosition);
}

void TriflingWindActor::End()
{
    if (_Dying || !_Sprite)
    {
        return;
    }

    _Dying = true;

    static int32 DieVariant = 0;

    const bool First = DieVariant == 0;

    DieVariant = First ? 1 : 0;

    if (_Enhanced)
    {
        _Sprite->ChangeAnimation(First ? "TriflingWind.Die2.1" : "TriflingWind.Die2.2");
    }
    else
    {
        _Sprite->ChangeAnimation(First ? "TriflingWind.Die1.1" : "TriflingWind.Die1.2");
    }

    _Sprite->SetRelativeRotation(0.f, 0.f, 0.f);
}