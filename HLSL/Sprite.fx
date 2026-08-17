#include "Share.fx"

struct VertexShader_Input_Texture
{
    float3 Position : POSITIONT;
    float2 UV : TEXCOORD;
};

struct VertexShader_Output_Texture
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

cbuffer Sprite : register(b2)
{
    float4 gSpriteTint;
}

VertexShader_Output_Texture SpriteVertexShader(VertexShader_Input_Texture Input)
{
    VertexShader_Output_Texture Output = (VertexShader_Output_Texture) 0;
    
    float3 Position = Input.Position;
    
    Output.UV = UpdateAnimation2D(Input.UV);
    
    return Output;
}

PixelShader_Output_Single SpritePixelShader(VertexShader_Output_Texture Input)
{
    PixelShader_Output_Single Output = (PixelShader_Output_Single) 0;
    
    float4 Color = gBaseTexture.Sample(gBaseSample, Input.UV);
    
    Output.Color = Color * gSpriteTint;
    
    return Output;
}