Texture2D g_Texture : register(t0);
Texture2D g_NormalMap : register(t1);
SamplerState g_Sampler : register(s0);

struct PS_INPUT
{
    float4 svPos : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD0; // PointLight Pos
    float2 uv : TEXCOORD1;
    float3x3 TBN : TEXCOORD2;
};

struct PS_OUTPUT
{
    float4 rt0 : SV_TARGET0;
    float4 rt1 : SV_TARGET1;
    float4 rt2 : SV_TARGET2;
};


PS_OUTPUT main(PS_INPUT input)
{   
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.rt0 = input.color;
    
    float3 N = normalize(input.normal);
    float3 tmpN = N;
    //float3 tmpN = 0.5f * (N + 1.0f);  
    output.rt1 = float4(tmpN, 1.0f);
    
    return output;
}