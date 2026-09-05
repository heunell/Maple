#pragma once

#include "UI/UI.h"
#include "Game/Monsters/Boss/BossPatternData.h"

class BossPatternNotice : public UI
{
public:
	BossPatternNotice() = default;
	virtual ~BossPatternNotice() = default;
	BossPatternNotice(const BossPatternNotice&) = delete;
	BossPatternNotice(BossPatternNotice&&) = delete;
	BossPatternNotice& operator=(const BossPatternNotice&) = delete;
	BossPatternNotice& operator=(BossPatternNotice&&) = delete;

private:
	Ptr<class SpriteComponent> _LeftSprite;

	Ptr<class SpriteComponent> _CenterSprite;

	Ptr<class SpriteComponent> _RightSprite;

	Ptr<class TextBlock>	   _TextBlock;

	FBossPatternNoticeData     _NoticeData;

	float _ElapsedTime = 0.f;

	bool _Finished = true;

private:
	void SetNoticeEnable(bool Enable);

	void SetNoticeOpacity(float Opacity);

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Destroy() override;

	void Show(const std::wstring& Text);

	bool IsFinished() const;
};