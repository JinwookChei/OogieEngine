Texture2D renderTextureDepth : register(t3);
SamplerState g_Sampler : register(s0);

struct PSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};


float4 main(PSInput input) : SV_TARGET0
{

    return input.color;
}