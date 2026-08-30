#pragma once
#include "SubManager.h"
#include <unordered_map>

class AnimationManager : public SubManager
{
	friend class LucidEntry;
	friend class LucidPhase1;
	friend class LucidPhase2;
	
public:
	AnimationManager();
	virtual ~AnimationManager();

private:
	bool LoadAnimationFile(const std::wstring& FileName, const std::string& DataName);

	std::unordered_map<std::string, Ptr<class Animation2DData>> _Animation;

public:
	bool Init();

	Ptr<class Animation2DData> FindAnimation(const std::string& Name);

	bool CreateAnimation(const std::string& Name, eAnimationTextureType Type);

	bool SetAnimationTextureType(const std::string& Name, eAnimationTextureType Type);

	void SetTexture(const std::string& Name, Ptr<class Texture> Texture);

	void SetTexture(const std::string& Name, const std::string& TextureName);

	void SetTexture(const std::string& Name, const std::string& TextureName, const std::wstring& FileName);

	void AddFrame(const std::string& Name, const FVector2D& UVLeftTop, const FVector2D& UVRightBottom, const FVector2D& Size, const FVector2D& Offset, float Delay);
	
	virtual void Destroy() override;
};

