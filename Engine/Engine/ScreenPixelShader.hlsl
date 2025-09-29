Texture2D renderTexture : register(t0);
SamplerState samplers : register(s0);

struct PS_ScreenRect
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


float4 main(PS_ScreenRect input) : SV_TARGET
{
    float4 color = renderTexture.Sample(samplers, input.uv);
    
    color.a = saturate(color.a);
    
    return color;
}