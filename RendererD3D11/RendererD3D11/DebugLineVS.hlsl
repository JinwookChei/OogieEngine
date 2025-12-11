cbuffer CBViewProj : register(b0)
{
    matrix View;
    matrix Projection;
    
    matrix InvViewTransform;
    matrix InvProjectTransform;
    
    float2 ScreenOffset;
    float2 ScreenScale;
    
    float4 CamPos;
};

struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VSOutput main(VSInput input)
{
    VSOutput output;
            
    float4 viewPos = mul(float4(input.pos, 1.0f), View);
    output.pos = mul(viewPos, Projection);
    output.color = input.color;
    return output;
}