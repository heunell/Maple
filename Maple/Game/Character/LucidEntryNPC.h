#pragma once
#include "Object/Actor.h"
#include <chrono>

class LucidEntryNPC : public Actor
{
public:
	LucidEntryNPC() = default;
	virtual ~LucidEntryNPC() = default;
	LucidEntryNPC(const LucidEntryNPC&) = delete;
	LucidEntryNPC(LucidEntryNPC&&) = delete;
	LucidEntryNPC& operator=(const LucidEntryNPC&) = delete;
	LucidEntryNPC& operator=(LucidEntryNPC&&) = delete;

private:
	Ptr<class SpriteComponent> _Sprite;

	Ptr<class InteractionComponent> _Interaction;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Destroy() override;

private:
	void Interaction();

	void ChangeToLucidPhase1();
};

