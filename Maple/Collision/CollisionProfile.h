#pragma once
#include "Core/Object.h"
#include "Common/Info.h"

class CollisionProfile : public Object
{
    friend class CollisionProfileManager;
public:
    CollisionProfile();
    virtual ~CollisionProfile();
    
private:
    std::string _Name;
    eCollisionChannel  _channel = eCollisionChannel::COLLISION_CHANNEL_END;
    eCollisionResponse _response[eCollisionChannel::COLLISION_CHANNEL_END] = {};
    
public:
    const eCollisionChannel  GetChannel() const;
    const std::string&       GetName() const;
    const eCollisionResponse GetResponse(eCollisionChannel channel);
    
public:
    virtual void Destroy() override;
    
public:
    void SetProfileResponse(std::pair<eCollisionChannel, eCollisionResponse> response)
    {
        _response[response.first] = response.second;
    }
    template<typename... Args>
    void SetProfileResponse(std::pair<eCollisionChannel, eCollisionResponse> response, Args... args)
    {
        SetProfileResponse(response);
        SetProfileResponse(args...);
    }
    
    void SetProfileResponse(){ /* Nothing */}
};
