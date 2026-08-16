#pragma once
#include "Shader.h"
#include <vector>

class GraphicShader : public Shader
{
public:
	GraphicShader();
	virtual ~GraphicShader();

private:
	ComPtr<ID3D11InputLayout> _InputLayout = nullptr;

	std::vector<D3D11_INPUT_ELEMENT_DESC> _Description;

	uint32 _InputLayoutSize = 0;

	ComPtr<ID3D11VertexShader> _VertexShader = nullptr;

	ComPtr<ID3DBlob> _VertexShaderBlob = nullptr;

	ComPtr<ID3D11PixelShader> _PixelShader = nullptr;

	ComPtr<ID3DBlob> _PixelShaderBlob = nullptr;


public:
	virtual bool Init() override;

	virtual void Destroy() override;

	virtual void SetShader() final;

	void AddInputLayourDescription(const char* Sementic, uint32 SementicIndex, DXGI_FORMAT Format, uint32 InputSlot,
								   uint32 Size, D3D11_INPUT_CLASSIFICATION InputSlotClass, uint32 InstanceDataStepRate);

	bool CreateInputLayout();

	bool LoadVertexShader(const char* EntryName, const char* FileName);

	bool LoadPixelShader(const char* EntryName, const char* FileName);
};

