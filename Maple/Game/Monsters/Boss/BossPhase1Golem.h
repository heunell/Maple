#pragma once
#include "Game/Monsters/MonsterBase.h"
#include "BossPatternData.h"

class BossPhase1Golem : public MonsterBase
{
public:
	BossPhase1Golem() = default;
	virtual ~BossPhase1Golem() = default;
	BossPhase1Golem(const BossPhase1Golem&) = delete;
	BossPhase1Golem(BossPhase1Golem&&) = delete;
	BossPhase1Golem& operator=(const BossPhase1Golem&) = delete;
	BossPhase1Golem& operator=(BossPhase1Golem&&) = delete;

private:
	Ptr<class SpriteComponent> _Sprite;

	Weak<class BossPhase1GolemState> _Owner;

	FBossPhase1GolemFallData _FallData;

    bool _Summoning = false;

	bool _Falling   = false;

	bool _Landing   = false;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	void Start(Ptr<class BossPhase1GolemState> Owner, const FVector3D& Position, float GroundY, float FallDuration);

    bool IsStanding() const;
    
	void SetPoolEnable(bool Enable);
};