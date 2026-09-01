#pragma once
#include "Defines.h"
#include <cstddef>
#include <functional>
#include <unordered_set>
#include <utility>
#include <vector>


// 공용 오브젝트 풀
// 객체의 보관과 대여 상태만 관리하고 활성화와 비활성화는 콜백에서 처리한다.

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
	// MaxCount가 0이면 객체 수를 제한하지 않는다.
	bool Configure(std::size_t MaxCount, Factory FactoryFunction, LifecycleCallback AcquireFunction, LifecycleCallback ReleaseFunction)
	{
		if (!_Objects.empty())
		{
			return false;
		}

		if (!FactoryFunction || !AcquireFunction || !ReleaseFunction)
		{
			return false;
		}

		_MaxCount = MaxCount;

		_Factory = std::move(FactoryFunction);

		_AcquireFunction = std::move(AcquireFunction);

		_ReleaseFunction = std::move(ReleaseFunction);

		return true;
	}


	// 전체 객체 수가 Count가 되도록 미리 생성한다.
	bool Reserve(std::size_t Count)
	{
		if (!IsConfigured())
		{
			return false;
		}

		if (_MaxCount > 0 && Count > _MaxCount)
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


	// 대기 객체가 없으면 최대 개수 안에서 새로운 객체를 생성한다.
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


	// 현재 대여 중인 모든 객체를 반환한다.
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
		if (_MaxCount > 0 && _Objects.size() >= _MaxCount)
		{
			return false;
		}

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
	std::size_t _MaxCount = 0;

	Factory _Factory;

	LifecycleCallback _AcquireFunction;

	LifecycleCallback _ReleaseFunction;


	std::vector<ObjectPtr> _Objects;

	std::vector<ObjectPtr> _AvailableObjects;

	std::unordered_set<T*> _ManagedObjects;

	std::unordered_set<T*> _InUseObjects;
};