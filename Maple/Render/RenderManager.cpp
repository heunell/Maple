#include "pch.h"
#include "RenderManager.h"
#include "RenderStateManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "Component/SceneComponent.h"
#include "Object/Actor.h"
#include <algorithm>

bool RenderManager::Init()
{
    CreateRenderLayer("BackGround", 0);

    CreateRenderLayer("Default", 50);

    CreateRenderLayer("Player", 100);

    //CreateRenderLayer("Monster", 500);

    CreateRenderLayer("Collider", INT_MAX);

    _RenderStateManager = New<RenderStateManager>();

    if (!_RenderStateManager->Init())
    {
        return false;
    }
    
    _AlphaBlend = _RenderStateManager->FindRenderState<BlendState>("AlphaBlend");

    _DepthStencilState = _RenderStateManager->FindRenderState<DepthStencilState>("DepthStencil");

    return true;
}

void RenderManager::Tick(float DeltaTime)
{
    for (auto& [LayerName, ActorID, ComponentID] : _RemoveRenders)
    {
        auto FoundIt = _LayerFinder.find(LayerName);

        if (_LayerFinder.end() == FoundIt)
        {
            continue;
        }

        auto ItLayer = _Layers.find(FoundIt->second);

        if (_Layers.end() == ItLayer)
        {
            continue;
        }

        ItLayer->second._Finder.erase({ ActorID, ComponentID });

        ItLayer->second._RefreshRenders = true;
    }

    _RemoveRenders.clear();

    if (_RefreshLayer)
    {
        for (auto& [Order, Layer] : _Layers)
        {
            auto SubIt = Layer._Finder.begin();

            while (SubIt != Layer._Finder.end())
            {
                Ptr<SceneComponent> RenderComponent = Lock<SceneComponent>(SubIt->second);

                if (nullptr == RenderComponent)
                {
                    SubIt++;

                    continue;
                }

                FRenderLayer* CurrentLayer = FindLayer(RenderComponent->GetRenderLayerName());

                if (nullptr == CurrentLayer)
                {
                    SubIt++;

                    continue;
                }

                if (CurrentLayer->_Order == Order)
                {
                    SubIt++;

                    continue;
                }

                Ptr<Actor> Owner = RenderComponent->GetOwner();
                if(nullptr == Owner)
                {
                    SubIt++;

                    continue;
                }

                std::pair<int32, int32> Key = { Owner->GetActorID(), RenderComponent->GetComponentID() };

                CurrentLayer->_Finder[Key] = SubIt->second;

                SubIt = Layer._Finder.erase(SubIt);

                CurrentLayer->_RefreshRenders = true;

                Layer._RefreshRenders = true;
            }
        }
    }
}

void RenderManager::Render(float DeltaTime)
{
    _AlphaBlend->SetState();
    
    _DepthStencilState->SetState();

    for (auto& [Order, Layer] : _Layers)
    {
        if (Layer._RefreshRenders)
        {
            Layer._Renders.clear();

            for (auto& [Key, Weak] : Layer._Finder)
            {
                Layer._Renders.push_back(Weak);
            }

            Layer._RefreshRenders = false;
        }

        if (_SortY)
        {
            std::sort(Layer._Renders.begin(), Layer._Renders.end(), [](const Weak<SceneComponent>& Source, const Weak<SceneComponent>& Destination)
                {
                    Ptr<SceneComponent> ComponentSource = Lock<SceneComponent>(Source);

                    Ptr<SceneComponent> ComponentDestination = Lock<SceneComponent>(Destination);

                    if (!ComponentSource || !ComponentDestination)
                    {
                        return false;
                    }

                    return ComponentSource->GetWorldPosition()._y < ComponentDestination->GetWorldPosition()._y;
                });
        }

        for (auto& It : Layer._Renders)
        {
            Ptr<SceneComponent> Component = Lock<SceneComponent>(It);
            
            if (nullptr == Component)
            {
                continue;
            }

            if (!Component->IsEnable())
            {
                continue;
            }

            if (!Component->IsActive())
            {
                continue;
            }

            Component->Render(DeltaTime);
        }
    }

    _AlphaBlend->ResetState();

    _DepthStencilState->ResetState();
}

void RenderManager::RefreshLayer()
{
    _RefreshLayer = true;
}

void RenderManager::SetYSort(bool Sort)
{
    _SortY = Sort;
}

void RenderManager::AddRenderComponent(int32 ActorID, Ptr<class SceneComponent> Component)
{
    FRenderLayer* Layer = FindLayer(Component->GetRenderLayerName());

    if (nullptr == Layer)
    {
        return;
    }

    Layer->_Finder[{ActorID, Component->GetComponentID()}] = Component;

    Layer->_RefreshRenders = true;
}

void RenderManager::RemoveRenderComponent(const std::string& LayerName, int32 ActorID, int32 ComponentID)
{
    std::tuple<std::string, int32, int32> Key = { LayerName, ActorID, ComponentID };

    _RemoveRenders.push_back(Key);
}

FRenderLayer* RenderManager::FindLayer(const std::string& Name)
{
    auto It = _LayerFinder.find(Name);

    if (_LayerFinder.end() == It)
    {
        return nullptr;
    }

    auto SubIt = _Layers.find(It->second);
    
    if (_Layers.end() == SubIt)
    {
        return nullptr;
    }

    return &(SubIt->second);
}

void RenderManager::Destroy()
{
    DESTROY(_RenderStateManager);
}

Ptr<class BlendState> RenderManager::GetAlphaBlend()
{
    return _AlphaBlend;
}

Ptr<class DepthStencilState> RenderManager::GetDepthStencilState()
{
    return _DepthStencilState;
}

void RenderManager::CreateRenderLayer(const std::string& Name, int32 Order)
{
    FRenderLayer* FoundLayer = FindLayer(Name);

    if (FoundLayer)
    {
        return;
    }

    FRenderLayer Layer;

    Layer._Name = Name;

    Layer._Order = Order;

    _LayerFinder[Name] = Order;

    _Layers[Order] = Layer;
}
