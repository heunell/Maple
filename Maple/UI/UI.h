#pragma once
#include "Object/Actor.h"

class UI : public Actor
{
public:
	UI() = default;
	virtual ~UI() = default;
	UI(const UI&) = delete;
	UI(UI&&) = delete;
	UI& operator=(const UI&) = delete;
	UI& operator=(UI&&) = delete;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

protected:
	Ptr<class SpriteComponent> CreateSprite(const std::string& ComponentName, const std::string& AnimationName);

	Ptr<class ProgressBar> CreateProgressBar(const std::string& ComponentName, const std::string& AnimationName);
};

