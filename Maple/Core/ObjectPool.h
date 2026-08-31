#pragma once
#include "Defines.h"

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// 공용 오브젝트 풀
// T가 Object를 상속한다고 가정하지 않고 Actor뿐만 아니라 여러 Component를 묶은 Projectile 구조체도 관리할 수 있도록한다
// ObjectPool은 객체의 보관과 대여 상태만 관리
// 실제 활성화와 비활성화 방법은 각 객체에 맞는 콜백함수로 전달받기

template<typename T>
class ObjectPool
{
public:
    using ObjectPtr = Ptr<T>;

    // 풀에 새 객체가 필요할 때 호출한다.
    // 반드시 매번 새로운 객체를 반환해야 한다.
    using Factory = std::function<ObjectPtr()>;

    // 객체를 대여하거나 반환할 때 호출한다.
    using LifecycleCallback = std::function<void(const ObjectPtr&)>;


public:
    ObjectPool() = default;

    ObjectPool(Factory FactoryFunction, LifecycleCallback AcquireFunction, LifecycleCallback ReleaseFunction)
    {
        Configure(std::move(FactoryFunction), std::move(AcquireFunction), std::move(ReleaseFunction));
    }

    ~ObjectPool() = default;

    ObjectPool(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool& operator=(ObjectPool&&) = delete;


public:
    // 객체가 만들어지기 전에 Pool의 동작을 설정한다.
    //
    // FactoryFunction
    // - 새로운 객체를 생성한다.
    //
    // AcquireFunction
    // - 대여되는 객체의 재사용 상태를 초기화하고 활성화한다.
    //
    // ReleaseFunction
    // - 사용이 끝난 객체의 구성요소를 비활성화한다.
    //
    // 이미 객체를 만든 이후에는 동작 방식을 변경하지 않는다.
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


    // 지금까지 만들어진 전체 객체 수가 Count가 되도록 미리 생성한다.
    //
    // 새로 생성된 객체는 ReleaseFunction을 거쳐 대기 상태가 된다.
    // Count는 "대기 객체 수"가 아니라 "전체 객체 수" 기준이다.
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


    // 대기 중인 객체 하나를 가져온다.
    //
    // 대기 객체가 없으면 Factory를 통해 새 객체를 만든다.
    // 반환 전에 AcquireFunction이 호출된다.
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


    // 사용이 끝난 객체를 Pool에 반환한다.
    //
    // ReleaseFunction이 실제 객체의 Sprite, Collision, Root 등을
    // 비활성화하고, ObjectPool은 대여 상태만 변경한다.
    //
    // 반환값이 false인 경우:
    // - nullptr
    // - 이 Pool이 만들지 않은 객체
    // - 이미 반환된 객체
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


    // 현재 대여 중인 모든 객체를 반환 상태로 만든다.
    //
    // Destroy는 호출하지 않는다.
    // 이미 반환된 객체에는 ReleaseFunction을 다시 호출하지 않는다.
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


    // 엔진의 Object::IsActive()와 다른 개념이다.
    // 현재 Pool에서 빌려 간 객체 수를 의미한다.
    std::size_t GetInUseCount() const
    {
        return _InUseObjects.size();
    }


    bool HasAvailableObject() const
    {
        return !_AvailableObjects.empty();
    }


private:
    // 객체를 하나 생성하여 대기 목록에 넣는다.
    //
    // Maple의 Component와 Actor는 생성 직후 활성화될 수 있으므로
    // 반드시 ReleaseFunction을 호출하여 대기 상태로 변경한다.
    bool CreateObject()
    {
        ObjectPtr Object = _Factory();

        if (!Object)
        {
            return false;
        }

        T* ObjectAddress = Object.get();

        // Factory가 이미 Pool에 있는 객체를 다시 반환한 경우를 막는다.
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


    // Pool이 생성한 모든 객체를 소유한다.
    std::vector<ObjectPtr> _Objects;

    // 현재 대여할 수 있는 객체 목록이다.
    // 뒤에서 꺼내므로 Acquire는 O(1)이다.
    std::vector<ObjectPtr> _AvailableObjects;

    // 다른 Pool의 객체 반환과 Factory의 중복 반환을 검사한다.
    std::unordered_set<T*> _ManagedObjects;

    // 현재 대여 중인 객체를 기록한다.
    // Object의 Active 상태와 구분되는 Pool 내부 상태다.
    std::unordered_set<T*> _InUseObjects;
};