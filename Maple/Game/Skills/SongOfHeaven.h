#pragma once
#include "Skill.h"
#include <vector>

enum class eSongOfHeavenState
{
	Idle,
	Ready,
	Start,
	Loop,
	End,

	Max
};

struct FSongOfHeavenProjectile
{
	Ptr<class SceneComponent> Root;

	Ptr<class AABBCollisionComponent> Collision;

	float LifeTime = 1.f;
	
	float CurrentLifeTime = 0.f;

	float Direction = 1.f;
};

class SongOfHeaven : public Skill
{
public:
	SongOfHeaven();
	virtual ~SongOfHeaven();

private:
	eSongOfHeavenState _State = eSongOfHeavenState::Idle;

	std::vector<FSongOfHeavenProjectile> _Projectiles;

	std::vector<Ptr<class SpriteComponent>> _HitEffectSprites;

	float _FireInterval = 0.17f; 

	float _FireTimer = 0.f;      

	void NextState();

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner) override;

	virtual void Collision(float DeltaTime) override;

	virtual void Tick(float DeltaTime);

	virtual void Start() override;

	virtual void Update(float DeltaTime) override;

	virtual void End() override;

	virtual void Destroy() override;

	void Fire(float DeltaTime);

	void Hit(const FVector3D& Position);
};

