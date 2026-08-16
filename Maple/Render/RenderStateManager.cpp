#include "pch.h"
#include "RenderStateManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"

RenderStateManager::RenderStateManager()
{}

RenderStateManager::~RenderStateManager()
{}

bool RenderStateManager::Init()
{
    AddBlendDescription("AlphaBlend",
                         true,
                         D3D11_BLEND_SRC_ALPHA,   
                         D3D11_BLEND_INV_SRC_ALPHA,
                         D3D11_BLEND_OP_ADD,                 
                         D3D11_BLEND_ONE,
                         D3D11_BLEND_ZERO,
                         D3D11_BLEND_OP_ADD,
                         D3D11_COLOR_WRITE_ENABLE_ALL);

    CreateState("AlphaBlend", false, false);

    CreateDepthStencilState("DepthStencil", false);

    return true;
}

void RenderStateManager::Destroy()
{
    for (auto& It : _RenderStates)
    {
        DESTROY(It.second);
    }
}

void RenderStateManager::SetBlendFactor(const std::string& Name, float Red, float Green, float Blue, float Alpha)
{
    Ptr<BlendState> State = FindRenderState<BlendState>(Name);

    if (nullptr == State)
    {
        State = New<BlendState>();

        _RenderStates[Name] = State;
    }

    State->SetBlendFactor(Red, Green, Blue, Alpha);
}

void RenderStateManager::SetSampleMask(const std::string& Name, UINT Mask)
{
    Ptr<BlendState> State = FindRenderState<BlendState>(Name);

    if (nullptr == State)
    {
        State = New<BlendState>();

        _RenderStates[Name] = State;
    }

    State->SetSampleMask(Mask);
}

void RenderStateManager::AddBlendDescription(const std::string& Name, bool Enable, D3D11_BLEND Sources, D3D11_BLEND Destination, D3D11_BLEND_OP BlendOpacity, D3D11_BLEND SourceAlpha, D3D11_BLEND DestinationAlpha, D3D11_BLEND_OP AlphaOpacity, UINT8 RenderTargetWriteMask)
{
    Ptr<BlendState> State = FindRenderState<BlendState>(Name);

    if (nullptr == State)
    {
        State = New<BlendState>();

        _RenderStates[Name] = State;
    }

    State->AddBlendDescription(Enable, Sources, Destination, BlendOpacity, SourceAlpha, DestinationAlpha, AlphaOpacity, RenderTargetWriteMask);
}

bool RenderStateManager::CreateState(const std::string& Name, bool AlphaToCoverage, bool Indentent)
{
    Ptr<BlendState> State = FindRenderState<BlendState>(Name);

    if (nullptr == State)
    {
        State = New<BlendState>();

        _RenderStates[Name] = State;
    }

    if (!State->CreateState(AlphaToCoverage, Indentent))
    {
        DESTROY(State);

        return false;
    }

    return true;
}

bool RenderStateManager::CreateDepthStencilState(const std::string& Name, bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunction, bool StencilEnable, UINT8 StencilReadMask, UINT8 StencilWriteMask, D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
    Ptr<DepthStencilState> State = FindRenderState<DepthStencilState>(Name);

    if (nullptr != State)
    {
        return false;
    }

    State = New<DepthStencilState>();

    if (!State->CreateState(DepthEnable, DepthWriteMask, DepthFunction, StencilEnable, StencilReadMask, StencilWriteMask, FrontFace, BackFace))
    {
        DESTROY(State);

        return false;
    }

    _RenderStates[Name] = State;

    return true;
}
