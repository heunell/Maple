#pragma once
#include "Game/Monsters/MonsterComponent.h"

// 보스 패턴 선택
// 루시드의 Idle → PatternSelect → PatternState → Idle 실행
// 직전 패턴 기록
// 골렘·버섯 등 ObjectPool 소유
// 각 패턴의 선택 가능 여부 판단

class BossComponent : public MonsterComponent
{
public:
	BossComponent() = default;
	virtual~BossComponent() = default;
	BossComponent(const BossComponent&) = delete;
	BossComponent(BossComponent&&) = delete;
	BossComponent& operator=(const BossComponent&) = delete;
	BossComponent& operator=(BossComponent&&) = delete;

};

