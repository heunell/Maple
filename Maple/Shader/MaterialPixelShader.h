#pragma once
#include "Shader.h"

class MaterialPixelShader : public Shader
{
public:
	MaterialPixelShader();
	virtual ~MaterialPixelShader();

private:
	ComPtr<ID3D11PixelShader> _PixelShader = nullptr;

	ComPtr<ID3DBlob> _PixelShaderBlob = nullptr;

public:
	virtual bool Init() override;
	
	virtual void SetShader() final;

	bool LoadPixelShader(const char* EntryName, const char* FileName);
	
	virtual void Destroy() override;
};

