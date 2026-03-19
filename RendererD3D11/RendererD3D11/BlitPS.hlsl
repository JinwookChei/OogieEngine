Texture2D FinalRenderTex : register(t0);
SamplerState Sampler : register(s0);

struct PS_ScreenRect
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_ScreenRect input) : SV_TARGET
{
    float4 pixelColor = FinalRenderTex.Sample(Sampler, input.uv);
    //clip(pixelColor.w - 0.0001f);
    return pixelColor;
}