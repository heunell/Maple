#pragma once
#include "Defines.h"

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

template<typename T>
class ObjectPool
{
public:
    using ObjectPtr = Ptr<T>;
    
    using Factory = std::function<ObjectPtr()>;

    using LifecycleCallback = std::function<void(const ObjectPtr&)>;


public:
    ObjectPool() = default;

    ObjectPool(std::size_t MaxCount, Factory FactoryFunction, LifecycleCallback AcquireFunction, LifecycleCallback ReleaseFunction)
    {
        Configure(MaxCount, std::move(FactoryFunction), std::move(AcquireFunction), std::move(ReleaseFunction));
    }

    ~ObjectPool() = default;
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool& operator=(ObjectPool&&) = delete;


public:
    bool Configure(Factory FactoryFunction, LifecycleCallback AcquireFunction, LifecycleCallback ReleaseFunction)
    {
        if (!_Objects.empty())
        {
            return false;
        }

        if (!FactoryFunction || !AcquireFunction || !ReleaseFunction)
        {
            return false;
        }

        _Factory = std::move(FactoryFunction);

        _AcquireFunction = std::move(AcquireFunction);

        _ReleaseFunction = std::move(ReleaseFunction);

        return true;
    }

    bool Reserve(std::size_t Count)
    {
        if (!IsConfigured())
        {
            return false;
        }

        while (_Objects.size() < Count)
        {
            if (!CreateObject())
            {
                return false;
            }
        }

        return true;
    }

    ObjectPtr Acquire()
    {
        if (!IsConfigured())
        {
            return nullptr;
        }

        if (_AvailableObjects.empty())
        {
            if (!CreateObject())
            {
                return nullptr;
            }
        }

        ObjectPtr Object = _AvailableObjects.back();

        _AvailableObjects.pop_back();

        _InUseObjects.insert(Object.get());

        _AcquireFunction(Object);

        return Object;
    }

    bool Release(const ObjectPtr& Object)
    {
        if (!Object)
        {
            return false;
        }

        T* ObjectAddress = Object.get();

        if (!_ManagedObjects.contains(ObjectAddress))
        {
            return false;
        }

        if (!_InUseObjects.contains(ObjectAddress))
        {
            return false;
        }

        _ReleaseFunction(Object);

        _InUseObjects.erase(ObjectAddress);

        _AvailableObjects.push_back(Object);

        return true;
    }

    void ReleaseAll()
    {
        _AvailableObjects.clear();

        for (const ObjectPtr& Object : _Objects)
        {
            if (!Object)
            {
                continue;
            }

            T* ObjectAddress = Object.get();

            if (_InUseObjects.contains(ObjectAddress))
            {
                _ReleaseFunction(Object);
            }

            _AvailableObjects.push_back(Object);
        }

        _InUseObjects.clear();
    }


    bool IsConfigured() const
    {
        return static_cast<bool>(_Factory) && static_cast<bool>(_AcquireFunction) && static_cast<bool>(_ReleaseFunction);
    }


    std::size_t GetObjectCount() const
    {
        return _Objects.size();
    }


    std::size_t GetAvailableCount() const
    {
        return _AvailableObjects.size();
    }

    std::size_t GetInUseCount() const
    {
        return _InUseObjects.size();
    }


    bool HasAvailableObject() const
    {
        return !_AvailableObjects.empty();
    }


private:
    bool CreateObject()
    {
        ObjectPtr Object = _Factory();

        if (!Object)
        {
            return false;
        }

        T* ObjectAddress = Object.get();

        if (_ManagedObjects.contains(ObjectAddress))
        {
            return false;
        }

        _Objects.push_back(Object);

        _ManagedObjects.insert(ObjectAddress);

        _ReleaseFunction(Object);

        _AvailableObjects.push_back(Object);

        return true;
    }

private:
    Factory _Factory;

    LifecycleCallback _AcquireFunction;

    LifecycleCallback _ReleaseFunction;

    std::vector<ObjectPtr> _Objects;

    std::vector<ObjectPtr> _AvailableObjects;

    std::unordered_set<T*> _ManagedObjects;

    std::unordered_set<T*> _InUseObjects;
};