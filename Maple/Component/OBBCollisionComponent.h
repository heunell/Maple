#pragma once
#include "CollisionCOmponent.h"

class OBBCollisionComponent : public CollisionComponent
{
public:
	OBBCollisionComponent();

	virtual ~OBBCollisionComponent();

protected:
	FVector2D _BoxSize;

	FOBB2D _Box;

public:
	const FOBB2D& GetBox() const;

	const FVector2D& GetBoxSize() const;

	void SetBoxSize(const FVector2D& BoxSize);

	void SetBoxSize(float X, float Y);

	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;

	virtual bool Collision(Weak<CollisionComponent> Destination);

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;

};

