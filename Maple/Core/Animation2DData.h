#pragma once
#include "Asset.h"

class Animation2DData : public Asset
{
public:
	Animation2DData();
	virtual ~Animation2DData();

private:
	Ptr<class Texture> _Texture;
	
	eAnimationTextureType _Type = eAnimationTextureType::END;

	std::vector<FAnimationFrame> _Frames;

public:
	eAnimationTextureType GetAnimationTextureType() const;

	Ptr<class Texture> GetTexture() const;

	const FAnimationFrame& GetFrame(int32 Index) const;

	int32 GetFrameCount() const;

	void SetAnimationTextureType(eAnimationTextureType Type);

	void SetTexture(const std::string& Name);

	void SetTexture(Ptr<class Texture> Textures);

	void SetTexture(const std::string& Name, const std::wstring& FileName);

	void AddFrame(const FVector2D& Start, const FVector2D& Size);

	void AddFrame(float StartX, float StartY, float SizeX, float SizeY);

	virtual void Destroy() override;
};

