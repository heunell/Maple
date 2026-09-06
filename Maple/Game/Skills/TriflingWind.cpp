#include "pch.h"
#include "TriflingWind.h"
#include "TriflingWindActor.h"
#include "Component/AABBCollisionComponent.h"
#include "Game/Character/Player.h"
#include "Game/Monsters/MonsterBase.h"
#include "World/Level.h"

#include <algorithm>
#include <random>
#include <vector>

void TriflingWind::Collision(float DeltaTime)
{
}

void TriflingWind::Start()
{
    Ptr<Player> OwnerPlayer = Cast<Actor, Player>(Lock<Actor>(_Owner));

    if (!OwnerPlayer || !OwnerPlayer->IsActive())
    {
        return;
    }

    Ptr<Level> CurrentLevel = GetLevel();

    if (!CurrentLevel)
    {
        return;
    }

    static std::mt19937 RandomEngine(std::random_device{}());

    std::bernoulli_distribution ProcChance(0.5);

    if (!ProcChance(RandomEngine))
    {
        return;
    }

    const float Direction = OwnerPlayer->IsRight() ? 1.f : -1.f;

    const FVector3D PlayerPosition = OwnerPlayer->GetWorldPosition();

    FVector3D SearchPosition = PlayerPosition;

    SearchPosition._x += Direction * 315.f;

    SearchPosition._y += 60.f;

    std::vector<Ptr<Actor>> Actors;

    CurrentLevel->FindActors("Boss", Actors);

    std::vector<Ptr<MonsterBase>> Targets;

    for (const Ptr<Actor>& Candidate : Actors)
    {
        Ptr<MonsterBase> Monster = Cast<Actor, MonsterBase>(Candidate);

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

                if (Box._Max._x >= SearchPosition._x - 450.f &&
                    Box._Min._x <= SearchPosition._x + 450.f &&
                    Box._Max._y >= SearchPosition._y - 300.f &&
                    Box._Min._y <= SearchPosition._y + 300.f)
                {
                    Targets.push_back(Monster);

                    break;
                }
            }

            for (const auto& Child : Component->GetChilds())
            {
                Components.push_back(Child.second);
            }
        }
    }

    if (Targets.empty())
    {
        return;
    }

    std::sort(Targets.begin(), Targets.end(), [PlayerPosition](const Ptr<MonsterBase>& A, const Ptr<MonsterBase>& B)
        {
            if (A->GetMaxHP() != B->GetMaxHP())
            {
                return A->GetMaxHP() > B->GetMaxHP();
            }

            const FVector3D PositionA = A->GetWorldPosition();

            const FVector3D PositionB = B->GetWorldPosition();

            const float AX = PositionA._x - PlayerPosition._x;

            const float AY = PositionA._y - PlayerPosition._y;

            const float BX = PositionB._x - PlayerPosition._x;

            const float BY = PositionB._y - PlayerPosition._y;

            return AX * AX + AY * AY < BX * BX + BY * BY;
        });

    const FVector2D SpawnOffsets[8] =
    {
        FVector2D(0.f, 88.f),
        FVector2D(Direction * 58.f, 62.f),
        FVector2D(Direction * 78.f, 20.f),
        FVector2D(Direction * 56.f, -28.f),
        FVector2D(0.f, -58.f),
        FVector2D(-Direction * 42.f, -26.f),
        FVector2D(-Direction * 58.f, 18.f),
        FVector2D(-Direction * 42.f, 58.f)
    };

    std::uniform_real_distribution<float> OffsetX(-8.f, 8.f);

    std::uniform_real_distribution<float> OffsetY(-7.f, 7.f);

    std::uniform_real_distribution<float> StartAngle(58.f, 82.f);

    std::bernoulli_distribution EnhancedChance(0.2);

    for (int32 i = 0; i < 5; ++i)
    {
        const int32 Index = (_SpawnIndex + i) % 8;

        FVector3D SpawnPosition = PlayerPosition;

        SpawnPosition._x += SpawnOffsets[Index]._x + OffsetX(RandomEngine);

        SpawnPosition._y += 8.f + SpawnOffsets[Index]._y + OffsetY(RandomEngine);

        Ptr<MonsterBase> Target = Targets[i % Targets.size()];

        const bool TargetRight = Target->GetWorldPosition()._x >= SpawnPosition._x;

        const float ArcDirection = i <= 2 ? 1.f : -1.f;

        const float Angle = (TargetRight ? 0.f : 180.f) + (TargetRight ? ArcDirection : -ArcDirection) * StartAngle(RandomEngine);

        const bool Enhanced = EnhancedChance(RandomEngine);

        Ptr<TriflingWindActor> Projectile = CurrentLevel->SpawnActor<TriflingWindActor>("TriflingWindActor", SpawnPosition, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

        if (!Projectile)
        {
            continue;
        }

        Projectile->Start(Target, Enhanced, Angle);
    }

    _SpawnIndex = (_SpawnIndex + 5) % 8;
}

void TriflingWind::Update(float DeltaTime)
{
}

void TriflingWind::End()
{
}