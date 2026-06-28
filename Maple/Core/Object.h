#pragma once
#include "Common/Info.h"
#include "Defines.h"
#include <memory>

class Object : public std::enable_shared_from_this<Object>
{
public:
    Object() {}
    virtual ~Object() {}
    
protected:
    bool _bEnable = false;
    bool _bActive = false;
    bool _IsPooled = false;
    eObjectType _Type = eObjectType::END;
    
public:
    bool IsActive() const { return _bActive; }
    bool IsEnable() const { return _bEnable; }
    void SetActive(bool active) { _bActive = active; }
    void SetEnable(bool enable) { _bEnable = enable; }
    eObjectType GetType() const { return _Type; }
    
    void Pool() {_IsPooled = true; }
    
    virtual void DrawInspector();
    virtual void Destroy() = 0;
    
    template<typename T>
    Ptr<T> This()
    {
        return Cast<Object, T>(shared_from_this());
    }
};
