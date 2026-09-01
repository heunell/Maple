#pragma once
#include "ActorComponent.h"
#include "Game/Skills/Skill.h"
#include <unordered_map>
class SkillComponent : public ActorComponent
{
public:
	SkillComponent();
	virtual ~SkillComponent();

private:
	std::unordered_map<eSkillType, Ptr<Skill>> _Skills;

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<Actor>Owner) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Destroy() override;

	void StartSkill(eSkillType SkillType);

	void UseSkill(eSkillType SkillType, float DeltaTime);

	void StopSkill(eSkillType SkillType);
};

