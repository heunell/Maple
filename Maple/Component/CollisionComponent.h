#pragma once
#include "SceneComponent.h"
#include <functional>
#include <map>

class CollisionComponent : public SceneComponent
{
	friend class CollisionManager;

public:
	CollisionComponent();
	
	virtual ~CollisionComponent();

protected:
	eCollisionShape _Shape = eCollisionShape::End;
	
	FVector3D _Min;
	
	FVector3D _Max;

	std::pair<int32, int32> _ColliderID;

	Ptr<class CollisionProfile> _Profile;

	std::function<void(Weak<CollisionComponent>)> _CollisionCallBack[eCollisionState::COLLISION_STATE_END];
	
	std::map<std::pair<int32, int32>, eCollisionState> _ColliderReactions;

#if _DEBUG
	
	Ptr<class TransformCBuffer> _TransformCBuffer = nullptr;
	
	Ptr<class ColorCBuffer> _ColorCBuffer = nullptr;
	
	Ptr<class Mesh> _Mesh = nullptr;
	
	Ptr<class Shader> _Shader = nullptr;

#endif

public:
	eCollisionShape GetShape() const { return _Shape; }
	
	eCollisionState CheckState(std::pair<int32, int32>& DestKey);

	const std::pair<int32, int32>& GetColliderID() const;

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;
	
	virtual bool Collision(Weak<CollisionComponent> Dest);

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;

	void SetCollisionProfile(const std::string& Name);

	const Ptr<class CollisionProfile> GetProfile() const;

private:
	void Invoke(eCollisionState, Weak<CollisionComponent> Dest, const std::pair<int32, int32>& DestKey);

public:
	template<typename T>
	void SetCollisionCallBack(eCollisionState State, T* Obj, void(T::* MemFunc)(Weak<CollisionComponent>))
	{
		_CollisionCallBack[State] = std::bind(MemFunc, Obj, std::placeholders::_1);
	}

	template<typename T>
	void SetCollisionCallBack(eCollisionState State, T&& Func)
	{
		_CollisionCallBack[State] = std::forward<T>(Func);
	}

};

