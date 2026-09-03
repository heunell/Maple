#pragma once
#include "Object/Actor.h"

enum class eSkillType
{
	// Attack
    SongOfHeaven,
    Anemoi,
	VortexSphere,

	// Buff
    SharpEyes,
    CKnight,
    StormBringer,
    GloryOfGuardians,
    DeSpell,

    END
};

class Skill : public Actor
{
public:
	Skill() = default;
	virtual ~Skill() = default;

protected:
	Weak<class Actor> _Owner;

	float _CoolDown = 0.f;

	float _RemainCoolDown = 0.f;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner);

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) = 0;

	virtual void Start() = 0;

	virtual void Update(float DeltaTime) = 0;

	virtual void End() = 0;

	virtual void Destroy() override;

	bool CanUse() const;

	void StartCoolDown();
};

