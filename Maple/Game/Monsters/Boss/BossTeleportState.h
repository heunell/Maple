#pragma once

#include "Game/Monsters/MonsterState.h"
#include "BossPatternData.h"

class BossTeleportState : public MonsterState
{
public:
	BossTeleportState() = default;
	virtual ~BossTeleportState() = default;
	BossTeleportState(const BossTeleportState&) = delete;
	BossTeleportState(BossTeleportState&&) = delete;
	BossTeleportState& operator=(const BossTeleportState&) = delete;
	BossTeleportState& operator=(BossTeleportState&&) = delete;

private:
	Weak<class BossComponent> _Owner;

	Ptr<class BossTeleportEffect> _TeleportEffect;

	FBossTeleportPatternData _PatternData;

	bool _EffectStarted = false;

	bool _Teleported = false;

private:
	void StartTeleportEffect();

public:
	bool Init(Ptr<class BossComponent> Owner);

	virtual void Enter(Ptr<class MonsterComponent> Monster) override;

	virtual Ptr<MonsterState> Tick(Ptr<class MonsterComponent> Monster, float DeltaTime) override;

	void TeleportPlayer();

	virtual void Reset() override;

	virtual void Destroy() override;
};