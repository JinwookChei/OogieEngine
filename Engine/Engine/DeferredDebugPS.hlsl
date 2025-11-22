Texture2D renderTextureAlbedo : register(t0);
Texture2D renderTextureNormal : register(t1);
Texture2D renderTextureSpecular : register(t2);
Texture2D renderTextureDepth : register(t3);

Texture2D renderTextureDebug : register(t4);
Texture2D renderTextureDebugDepth : register(t5);
SamplerState samplers : register(s0);


struct PS_ScreenRect
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


float4 main(PS_ScreenRect input) : SV_TARGET
{   
    float albedoDepth = renderTextureDepth.Sample(samplers, input.uv).r;
    float debugDepth = renderTextureDebugDepth.Sample(samplers, input.uv).r;
    float4 debugPixel = renderTextureDebug.Sample(samplers, input.uv);
    
    clip(debugPixel.w - 0.001f);
    clip(albedoDepth - debugDepth);

    return debugPixel;
}