//Texture2D g_Texture : register(t0);
//Texture2D g_NormalMap : register(t1);
SamplerState g_Sampler : register(s0);

cbuffer CBPerObject : register(b1)
{
    matrix World;
    
    float3 MaterialSpecular;
    float MaterialShineness;
};

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

float3 EncodingNormal(float3 normal)
{
    return 0.5f * (normalize(normal) + 1.0f);
}

float3 DecodingNormal(float3 normal)
{
    return normalize(normal * 2.0f - 1.0f);
}

PS_OUTPUT main(PS_INPUT input)
{   
    PS_OUTPUT output = (PS_OUTPUT) 0;
    output.rt0 = input.color;
    
    float3 N = normalize(input.normal);
    output.rt1 = float4(N, 1.0f);
    
    //float3 encodingN = EncodingNormal(N);
    //output.rt1 = float4(encodingN, 1.0f);
    
    output.rt2 = float4(MaterialSpecular, MaterialShineness);
    
    return output;
}