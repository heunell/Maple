#pragma once
#include "Common/Singleton.h"
#include "Core/Defines.h"
#include <vector>
#include <map>
#include <unordered_map>

struct FRenderLayer
{
    std::string _Name;
    int32 _Order;
    
    std::vector<Weak<class SceneComponent>> _Renders;
    std::map<std::pair<int32, int32>, Weak<class SceneComponent>> _Finder;
    bool _RefreshRenders;
};

class RenderManager : public Singleton<RenderManager>
{
public:
    RenderManager() = default;
    virtual ~RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    RenderManager(RenderManager&&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;
    RenderManager& operator=(RenderManager&&) = delete;
    
private:
    std::map<int32, FRenderLayer> _Layers;
    std::unordered_map<std::string, int32> _LayerFinder;
    std::vector<std::tuple<std::string, int32, int32>> _RemoveRenders;
    bool _RefreshLayer;
    bool _SortY = true;
    
    Ptr<class RenderStateManager> _RenderStateManager;
    Ptr<class BlendState>         _AlphaBlend;
    Ptr<class DepthStencilState>  _DepthStencilState;
    
public:
    bool Init();
    void Tick(float DeltaTime);
    void Render(float DeltaTime);
    void RefreshLayer();
    void SetYSort(bool Sort);
    void AddRenderComponent(int32 ActorID, Ptr<class SceneComponent> Component);
    void RemoveRenderComponent(const std::string& LayerName, int32 ActorID, int32 ComponentID);
    FRenderLayer* FindLayer(const std::string& Name);
    virtual void Destroy() override;
    Ptr<class BlendState> GetAlphaBlend();
    Ptr<class DepthStencilState> GetDepthStencilState();
    
private:
    void CreateRenderLayer(const std::string& Name, int32 Order);
};
