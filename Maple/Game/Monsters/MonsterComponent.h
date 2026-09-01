#pragma once
#include "Component/ActorComponent.h"

// 일반 몬스터 FSM 실행 기반
// 현재 State 보관 및 전환
// 골렘과 버섯도 사용 가능

class MonsterComponent : public ActorComponent
{
public:
	MonsterComponent() = default;
	virtual ~MonsterComponent() = default;
	MonsterComponent(const MonsterComponent&) = delete;
	MonsterComponent(MonsterComponent&&) = delete;
	MonsterComponent& operator=(const MonsterComponent&) = delete;
	MonsterComponent& operator=(MonsterComponent&&) = delete;

};