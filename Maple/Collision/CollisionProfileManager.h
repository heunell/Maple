#pragma once
#include "Common/Singleton.h"
#include "Common/Info.h"
#include "CollisionProfile.h"
#include <map>
#include <unordered_map>

class CollisionProfileManager : public Singleton<CollisionProfileManager>
{
public:
    CollisionProfileManager() = default;
    virtual ~CollisionProfileManager() = default;
    CollisionProfileManager(const CollisionProfileManager&) = delete;
    CollisionProfileManager(CollisionProfileManager&&) = delete;
    CollisionProfileManager& operator=(const CollisionProfileManager&) = delete;
    CollisionProfileManager& operator=(CollisionProfileManager&&) = delete;
    
private:
    std::map<eCollisionChannel, eCollisionResponse> _channels;
    std::unordered_map<std::string, Ptr<CollisionProfile>> _profiles;
    
public:
    bool Init();
    Ptr<CollisionProfile> FindProfile(const std::string& name);
    void CreateChannel(eCollisionChannel channel, eCollisionResponse response);
    void CreateProfile(const std::string& name, eCollisionChannel channel);
    void SetProfileResponse(const std::string& profileName, eCollisionChannel channel ,eCollisionResponse response);
    virtual void Destroy() override;
public:
    template<typename... Args>
    void SetProfileResponse(const std::string& name, std::pair<eCollisionChannel, eCollisionResponse> response, Args... args)
    {
        Ptr<CollisionProfile> profile = FindProfile(name);
        if (nullptr == profile)
            return;
        profile->SetProfileResponse(response, args...);
    }
};
