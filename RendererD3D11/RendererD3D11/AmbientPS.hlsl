Texture2D renderTextureAlbedo : register(t0);
SamplerState samplers : register(s0);

cbuffer CBPerFrame : register(b0)
{
    matrix View;
    matrix Projection;
    matrix InvViewTransform;
    matrix InvProjectTransform;
    float2 ScreenOffset;
    float2 ScreenScale;
    float2 ScreenResolution;
    float2 Pad0;
    float4 CamPos;
};


struct PS_ScreenRect
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float3 EncodingNormal(float3 normal)
{
    return 0.5f * (normalize(normal) + 1.0f);
}

float3 DecodingNormal(float3 normal)
{
    return normalize(normal * 2.0f - 1.0f);
}

float4 main(PS_ScreenRect input) : SV_TARGET
{
    float4 albedo = renderTextureAlbedo.Sample(samplers, input.uv);
    return albedo * float4(0.05f, 0.05f, 0.05f, 1.0f);
}
