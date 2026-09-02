#pragma once
#include "Game/Monsters/MonsterBase.h"

class BossMushroom : public MonsterBase
{
public:
	BossMushroom() = default;
	virtual ~BossMushroom() = default;
	BossMushroom(const BossMushroom&) = delete;
	BossMushroom(BossMushroom&&) = delete;
	BossMushroom& operator=(const BossMushroom&) = delete;
	BossMushroom& operator=(BossMushroom&&) = delete;

private:
	Ptr<class SpriteComponent> _Sprite;

	Weak<class BossMushroomState> _Owner;

	int32 _Direction = 0;

	float _EndX = 0.f;

	float _MoveSpeed = 35.f;

	bool _Moving = false;

	bool _Dying = false;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	void Start(Ptr<class BossMushroomState> Owner, const FVector3D& Position, int32 Direction, float EndX);

	void SetPoolEnable(bool Enable);
};