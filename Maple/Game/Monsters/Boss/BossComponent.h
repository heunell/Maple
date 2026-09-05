#pragma once
#include "Game/Monsters/MonsterComponent.h"
#include <vector>

// 보스 State 등록과 패턴 선택을 담당한다.
class BossComponent : public MonsterComponent
{
public:
	BossComponent() = default;
	virtual ~BossComponent() = default;
	BossComponent(const BossComponent&) = delete;
	BossComponent(BossComponent&&) = delete;
	BossComponent& operator=(const BossComponent&) = delete;
	BossComponent& operator=(BossComponent&&) = delete;

private:
	Ptr<class BossIdleState> _IdleState;

	std::vector<Ptr<class MonsterState>> _PatternStates;

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Destroy() override;

	Ptr<class MonsterState> SelectPatternState();

	Ptr<class BossIdleState> GetIdleState() const;

	void ResetBattle();
};