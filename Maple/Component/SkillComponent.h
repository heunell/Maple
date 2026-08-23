#pragma once
#include "ActorComponent.h"

class SkillComponent : public ActorComponent
{
public:
	SkillComponent();
	virtual ~SkillComponent();

private:
	Ptr<class SongOfHeaven> _SongOfHeaven;

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<Actor>Owner) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Destroy() override;

	void StartSkill(float DeltaTime);

	void UseSkill(float DeltaTime);

	void StopSkill(float DeltaTime);
};

