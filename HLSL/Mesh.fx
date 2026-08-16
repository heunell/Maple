#include "Share.fx"

struct VertexShader_Input_Textrue
{
    float3 Position : POSITION;
    
    float2 UV : TEXCOORD;
};

struct VertexShader_Output_Texture
{
    float4 Position : SV_Position;
    
    float2 UV : TEXCOORD;
};

VertexShader_Output_Texture MaterialMeshVS(VertexShader_Input_Textrue Input)
{
    VertexShader_Output_Texture Output = (VertexShader_Output_Texture) 0;
    
    Output.Position = mul(float4(Input.Position, 1.f), gWVP);
    
    Output.UV = Input.UV;
    
    return Output;
}

PixelShader_Output_Single DefaultMaterialPS(VertexShader_Output_Texture Input)
{
    PixelShader_Output_Single Output = (PixelShader_Output_Single) 0;
    
    float4 Colors = gBaseTexture.Sample(gBaseSample, Input.UV);
    
    Colors.rgb *= gMaterialBaseColor.rgb;
    
    Colors.a *= gMaterialOpacity;
    
    Output.Color = Colors;

    return Output;
}