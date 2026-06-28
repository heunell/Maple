#pragma once
#include "Common/Singleton.h"
#include "Core/Defines.h"
#include <vector>
#include <map>
#include <unordered_map>

struct FRenderLayer
{
    std::string _name;
    int32 _order;
    
    std::vector<Weak<class SceneComponent>> _renders;
    std::map<std::pair<int32, int32>, Weak<class SceneComponent>> _finder;
    bool _refreshRenders;
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
    std::map<int32, FRenderLayer> _layers;
    std::unordered_map<std::string, int32> _layerFinder;
    std::vector<std::tuple<std::string, int32, int32>> _removeRenders;
    bool _refreshLayer;
    bool sortY = true;
    
    Ptr<class RenderStateManager> _renderStateManager;
    Ptr<class BlendState>         _alphaBlend;
    Ptr<class DepthStencilState>  _depthStencilState;
    
public:
    bool Init();
    void Tick(float deltaTime);
    void Render(float deltaTime);
    void RefreshLayer();
    void SetYSort(bool sort);
    void AddRenderComponent(int32 actorID, Ptr<class SceneComponent> comp);
    void RemoveRenderComponent(const std::string& layerName, int32 actorID, int32 compID);
    FRenderLayer* FindLayer(const std::string& name);
    virtual void Destroy() override;
    Ptr<class BlendState> GetAlphaBlend();
    Ptr<class DepthStencilState> GetDepthStencilState();
    
private:
    void CreateRenderLayer(const std::string& name, int32 order);
};
