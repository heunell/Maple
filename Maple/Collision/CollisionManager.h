#pragma once
#include "Core/Object.h"
#include <map>

class CollisionManager : public Object
{
public:
	CollisionManager() {}
	virtual ~CollisionManager() {}

private:
	// 모든 충돌체들을 관리하는 클래스
	std::map<std::pair<int32, int32>, Ptr<class CollisionComponent>> _Colliders;

	// 지워지는 충돌체들
	std::vector<std::pair<int32, int32>> _RemoveColliders;

public:
	void Init();
	
	void Collision(float DeltaTime);
	
	void Insert(std::pair<int32, int32>& CollisionID, Ptr<class CollisionComponent> Component);
	
	void Remove(std::pair<int32, int32>& CollisionID);

	virtual void Destroy();

	Ptr<class CollisionComponent> FindCollider(std::pair<int32, int32>& CollisionID);
};

