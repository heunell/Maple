#pragma once
#include "Game/Monsters/MonsterComponent.h"

#include <vector>


class Boss2Component : public MonsterComponent
{
public:
	Boss2Component() = default;
	virtual ~Boss2Component() = default;
	Boss2Component(const Boss2Component&) = delete;
	Boss2Component(Boss2Component&&) = delete;
	Boss2Component& operator=(const Boss2Component&) = delete;
	Boss2Component& operator=(Boss2Component&&) = delete;

private:
	Ptr<class Boss2IdleState> 			 _IdleState;

	std::vector<Ptr<class MonsterState>> _PatternStates;

	Weak<class BossPatternNotice> 		 _PatternNotice;

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Destroy() override;

	Ptr<class MonsterState> SelectPatternState();

	Ptr<class Boss2IdleState> GetIdleState() const;

	void SetPatternNotice(Ptr<class BossPatternNotice> PatternNotice);

	Ptr<class BossPatternNotice> GetPatternNotice() const;
};