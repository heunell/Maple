#pragma once
#include "UI.h"

class CursorUI : public UI
{
public:
	CursorUI() = default;
	virtual ~CursorUI() = default;
	CursorUI(const CursorUI&) = delete;
	CursorUI(CursorUI&&) = delete;
	CursorUI& operator=(const CursorUI&) = delete;
	CursorUI& operator=(CursorUI&&) = delete;

private:
	Ptr<class SpriteComponent> _CursorSprite;

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Destroy() override;

private:
	bool AddCursorAnimation(const std::string& AnimationName, bool Loop = false);
};

