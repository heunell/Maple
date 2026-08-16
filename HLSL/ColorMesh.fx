#include "Share.fx"
struct VertexShader_Input_Color
{
    float3 Posision : POSITION;
   
    float4 Color : Color;
};

struct VertexShader_Output_Color
{
    float4 Posision : SV_POSITION;
    
    float4 Color : COLOR;
};

VertexShader_Output_Color ColorMeshVS(VertexShader_Input_Color Input)
{
    VertexShader_Output_Color Output = (VertexShader_Output_Color) 0;
    
    Output.Posision = mul(float4(Input.Posision, 1.f), gWVP);
    
    Output.Color = Input.Color;
    
    return Output;
}

PixelShader_Output_Single ColorMeshPS(VertexShader_Output_Color Input)
{
    PixelShader_Output_Single Output = (PixelShader_Output_Single) 0;
   
    Output.Color = Input.Color;
    
    return Output;
}

float4 FrameMeshVS(float3 Position : POSITION) : SV_POSITION
{
    float4 Output = (float4) 0;
    float3 InputPosition = Position;
    Output = mul(float4(InputPosition, 1.f), gWVP);
    return Output;
}

PixelShader_Output_Single FrameMeshPS(float4 Position : SV_POSITION)
{
    PixelShader_Output_Single Output = (PixelShader_Output_Single) 0;
    Output.Color = gColor;
    return Output;
}