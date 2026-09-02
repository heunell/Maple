#pragma once

#include "Object/Actor.h"

class BossTeleportEffect : public Actor
{
public:
	BossTeleportEffect() = default;
	virtual ~BossTeleportEffect() = default;
	BossTeleportEffect(const BossTeleportEffect&) = delete;
	BossTeleportEffect(BossTeleportEffect&&) = delete;
	BossTeleportEffect& operator=(const BossTeleportEffect&) = delete;
	BossTeleportEffect& operator=(BossTeleportEffect&&) = delete;

private:
	Ptr<class SpriteComponent> _Sprite;

	Weak<class BossTeleportState> _Owner;

	Weak<class Actor> _Target;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	void Start(Ptr<class BossTeleportState> Owner, Ptr<class Actor> Target);

	void SetEffectEnable(bool Enable);
};