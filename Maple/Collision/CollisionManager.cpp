#include "pch.h"
#include "CollisionManager.h"
#include "Component/CollisionComponent.h"
#include "Collision/CollisionProfileManager.h"
#include "Object/Actor.h"

void CollisionManager::Init()
{
}

void CollisionManager::Collision(float DeltaTime)
{
    // 지워져야 할 우선순위 먼저 정리
    for (auto it : _RemoveColliders)
    {
        _Colliders.erase(it);
    }

    _RemoveColliders.clear();

    // 전체 충돌컴포넌트들을 순회 -> 충돌연산을 진행
    for (auto& It : _Colliders)
    {
        std::pair<int32, int32> ItKey = It.first;    // 충돌체의 ID

        Ptr<CollisionComponent> ItValue = It.second; // 충돌 컴포넌트(충돌체)

        Ptr<Actor> ColliderOwner = ItValue->GetOwner();
        
        if (!ColliderOwner || !ColliderOwner->IsEnable() || !ColliderOwner->IsActive()) // 맵 Actor가 Disable이 되면 그 Actor의 Collision도 검사하지 않는다
        {
            continue;
        }

        if (!ItValue->IsEnable())
        {
            continue;
        }

        if (!ItValue->IsActive())
        {
            continue;
        }

        Ptr<CollisionProfile> SourceProfile = ItValue->GetProfile();
        if (nullptr == SourceProfile)
        {
            continue;
        }

        for (auto& SubIt : _Colliders)
        {
            std::pair<int32, int32> SubItKey = SubIt.first;     // 충돌체의 ID

            Ptr<CollisionComponent> SubItValue = SubIt.second;  // 충돌 컴포넌트(충돌체)

            Ptr<Actor> DestinationOwner = SubItValue->GetOwner();

            // 꺼진 맵에 속한 Collider가 다른 활성 Actor와 충돌하지 않게 한다.
            if (!DestinationOwner || !DestinationOwner->IsEnable() || !DestinationOwner->IsActive())
            {
                continue;
            }

            if (!SubItValue->IsEnable())
            {
                continue;
            }

            if (!SubItValue->IsActive())
            {
                continue;
            }

            Ptr<CollisionProfile> DestinationProfile = SubItValue->GetProfile();
            if (nullptr == DestinationProfile)
            {
                continue;
            }

            if (SourceProfile->GetResponse(DestinationProfile->GetChannel()) == eCollisionResponse::COLLISION_RESPONSE_IGNORE ||
                SourceProfile->GetResponse(DestinationProfile->GetChannel()) == eCollisionResponse::COLLISION_RESPONSE_END)
                {
                    continue;
                }
            
            if (DestinationProfile->GetResponse(SourceProfile->GetChannel()) == eCollisionResponse::COLLISION_RESPONSE_IGNORE ||
                DestinationProfile->GetResponse(SourceProfile->GetChannel()) == eCollisionResponse::COLLISION_RESPONSE_END)
                {
                    continue;
                }

            const eCollisionResponse SourceResponse = SourceProfile->GetResponse(DestinationProfile->GetChannel());

            const eCollisionResponse DestinationResponse = DestinationProfile->GetResponse(SourceProfile->GetChannel());

            if (SourceResponse      == COLLISION_RESPONSE_IGNORE || SourceResponse      == COLLISION_RESPONSE_END ||
                DestinationResponse == COLLISION_RESPONSE_IGNORE || DestinationResponse == COLLISION_RESPONSE_END)
            {
                continue;
            }

            const eCollisionState CollisionState = SourceResponse      == COLLISION_RESPONSE_BLOCK && 
                                                   DestinationResponse == COLLISION_RESPONSE_BLOCK ? COLLISION_STATE_BLOCK : COLLISION_STATE_OVERLAP;

            if (ItValue->Collision(SubItValue))
            {
                ItValue->Invoke(CollisionState, SubItValue, SubItKey);
            }
            else
            {
                const eCollisionState PreviousState = ItValue->CheckState(SubItKey);

                if (PreviousState == COLLISION_STATE_BLOCK ||
                    PreviousState == COLLISION_STATE_OVERLAP)
                {
                    ItValue->Invoke(COLLISION_STATE_RELEASE, SubItValue, SubItKey);
                }
            }
        }
    }
}

void CollisionManager::Insert(std::pair<int32, int32>& CollisionID, Ptr<class CollisionComponent> Component)
{
    _Colliders[CollisionID] = Component;
}

void CollisionManager::Remove(std::pair<int32, int32>& CollisionID)
{
    _RemoveColliders.push_back(CollisionID);
}

Ptr<class CollisionComponent> CollisionManager::FindCollider(std::pair<int32, int32>& CollisionID)
{
    auto It = _Colliders.find(CollisionID);
    if (_Colliders.end() == It)
    {
        return nullptr;
    }

    return It->second;
}

void CollisionManager::Destroy()
{
    _Colliders.clear();
}