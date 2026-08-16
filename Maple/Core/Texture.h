#pragma once
#include "Asset.h"
#include "DirectXTex.h"

struct FTextureInfo
{
	DirectX::ScratchImage _Image;
	
	ComPtr<ID3D11ShaderResourceView> _Srv;
	
	uint32 _Width = 0;
	
	uint32 _Height = 0;
	
	std::wstring _FileName;
};

class Texture : public Asset
{
public:
	Texture() = default;
	virtual ~Texture() = default;

protected:
	std::vector<FTextureInfo> _TextureInfos;

public:
	const FTextureInfo* GetTexture(int32 Index) const;

	bool LoadTexture(const std::wstring& FileName);

	void SetShader(int32 RegisterNum, int32 ShaderBufferType, int32 TextureIndex);
	
	void ResetShader(int32 RegisterNum, int32 ShaderBufferType);

	virtual void Destroy() override;

protected:
	bool CreateResourcesView(int32 Index);
};

