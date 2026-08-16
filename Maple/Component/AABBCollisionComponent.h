#pragma once
#include "CollisionComponent.h"

class AABBCollisionComponent : public CollisionComponent
{
public:
	AABBCollisionComponent();

	virtual ~AABBCollisionComponent();

protected:
	FVector2D _BoxSize;

	FAABB2D _Box;

public:
	const FVector2D& GetBoxSize() const;

	const FAABB2D& GetBox() const;

	void SetBoxSize(const FVector2D& Size);

	void SetBoxSize(float X, float Y);

	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;

	virtual bool Collision(Weak<CollisionComponent> Destination) override;
};

