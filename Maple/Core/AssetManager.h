#pragma once
#include "Common/Singleton.h"
#include "Common/Info.h"
#include "Defines.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "AnimationManager.h"

class AssetManager : public Singleton<AssetManager>
{

public:
    AssetManager() = default;
    ~AssetManager() = default;
    AssetManager(const AssetManager&) = delete;
    AssetManager(AssetManager&&) = delete;
    AssetManager& operator = (const AssetManager&) = delete;
    AssetManager& operator = (AssetManager&&) = delete;

private:
    std::map<eAssetType, Ptr<class SubManager>> _Managers;

public:
    bool Init();

    template<typename T>
    Ptr<T> Get(eAssetType Type)
    {
        if (_Managers.end() == _Managers.find(Type))
        {
            return nullptr;
        }

        return Cast<SubManager, T>(_Managers[Type]);
    }

    template<typename T>
    bool RegisterManager(eAssetType Type)
    {
        Ptr<T> Manager = New<T>();
        if (false == Manager->Init())
        {
            DESTROY(Manager);

            return false;
        }

        _Managers[Type] = Manager;

        return true;
    }

    virtual void Destroy() override;
};

#define MESH_MANAGER AssetManager::Instance().Get<MeshManager>(eAssetType::MESH)
#define TEXTURE_MANAGER AssetManager::Instance().Get<TextureManager>(eAssetType::TEXTURE)
#define MATERIAL_MANAGER AssetManager::Instance().Get<MaterialManager>(eAssetType::MATERIAL)
#define ANIMATION_MANAGER AssetManager::Instance().Get<AnimationManager>(eAssetType::ANIMATION)

