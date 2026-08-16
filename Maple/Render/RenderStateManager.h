#pragma once
#include "Core/Object.h"
#include "RenderState.h"

class RenderStateManager : public Object
{
public:
	RenderStateManager();

	virtual ~RenderStateManager();

private:
	std::unordered_map<std::string, Ptr<class RenderState>> _RenderStates;

public:
	bool Init();

	virtual void Destroy() override;

	void SetBlendFactor(const std::string& Name, float Red, float Green, float Blue, float Alpha);

	void SetSampleMask(const std::string& Name, UINT Mask);

	void AddBlendDescription(const std::string& Name, bool Enable,
							 D3D11_BLEND Sources, D3D11_BLEND Destination, D3D11_BLEND_OP BlendOpacity, 
							 D3D11_BLEND SourceAlpha, D3D11_BLEND DestinationAlpha, D3D11_BLEND_OP AlphaOpacity, 
							 UINT8 RenderTargetWriteMask);

	bool CreateState(const std::string& Name, bool AlphaToCoverage, bool Indentent);

	bool CreateDepthStencilState(const std::string& Name, bool DepthEnable = true,
		                         D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		                         D3D11_COMPARISON_FUNC DepthFunction   = D3D11_COMPARISON_LESS,
								 bool StencilEnable                    = false,
		                         UINT8 StencilReadMask                 = D3D11_DEFAULT_STENCIL_READ_MASK,
		                         UINT8 StencilWriteMask                = D3D11_DEFAULT_STENCIL_WRITE_MASK,
								 D3D11_DEPTH_STENCILOP_DESC FrontFace  = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP , D3D11_COMPARISON_ALWAYS },
								 D3D11_DEPTH_STENCILOP_DESC BackFace = { D3D11_STENCIL_OP_KEEP , D3D11_STENCIL_OP_KEEP , D3D11_STENCIL_OP_KEEP , D3D11_COMPARISON_ALWAYS });

	template<typename T>
	Ptr<T> FindRenderState(const std::string& Name)
	{
		auto It = _RenderStates.find(Name);

		if (_RenderStates.end() == It)
		{
			return nullptr;
		}

		return Cast<RenderState, T>(It->second);
	}
};

