#include "pch.h"
#include "CollisionProfileManager.h"

bool CollisionProfileManager::Init()
{
    CreateChannel(COLLISION_CHANNEL_PLAYER , COLLISION_RESPONSE_IGNORE);

    CreateChannel(COLLISION_CHANNEL_MONSTER, COLLISION_RESPONSE_IGNORE);
    
    CreateProfile("Player" , COLLISION_CHANNEL_PLAYER);
    
    CreateProfile("Monster", COLLISION_CHANNEL_MONSTER);
    
    SetProfileResponse("Player",
        std::make_pair(COLLISION_CHANNEL_MONSTER, COLLISION_RESPONSE_OVERLAP),
        std::make_pair(COLLISION_CHANNEL_BULLET, COLLISION_RESPONSE_IGNORE),
        std::make_pair(COLLISION_CHANNEL_ITEM, COLLISION_RESPONSE_BLOCK),
        std::make_pair(COLLISION_CHANNEL_BOSS, COLLISION_RESPONSE_OVERLAP)
        );
    
    SetProfileResponse("Monster",
        std::make_pair(COLLISION_CHANNEL_PLAYER, COLLISION_RESPONSE_OVERLAP));
    
    return true;
}

Ptr<CollisionProfile> CollisionProfileManager::FindProfile(const std::string& name)
{
    auto it = _profiles.find(name);
    if (_profiles.end() == it)
        return nullptr;
    return it->second;
}

void CollisionProfileManager::CreateChannel(eCollisionChannel channel, eCollisionResponse response)
{
    if (_channels.end() != _channels.find(channel))
        return;
    _channels[channel] = response;
}

void CollisionProfileManager::CreateProfile(const std::string& name, eCollisionChannel channel)
{
    if (_channels.end() == _channels.find(channel))
        return;
    if (_profiles.end() != _profiles.find(name))
        return;
    Ptr<CollisionProfile> profile = New<CollisionProfile>();
    profile->_Name    = name;
    profile->_channel = channel;
    for (auto it : _channels)
        profile->_response[it.first] = it.second;
    _profiles[profile->_Name] = profile;
}

void CollisionProfileManager::SetProfileResponse(const std::string& profileName, eCollisionChannel channel, eCollisionResponse response)
{
    Ptr<CollisionProfile> profile = FindProfile(profileName);
    if (nullptr == profile)
        return;
    profile->_response[channel] = response;
}

void CollisionProfileManager::Destroy()
{
    for (auto& it : _profiles)
        DESTROY(it.second);
    _profiles.clear();
    _channels.clear();
}
