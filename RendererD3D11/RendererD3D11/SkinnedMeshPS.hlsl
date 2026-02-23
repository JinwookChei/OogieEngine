Texture2D TextureColor : register(t0);
Texture2D TextureNormal : register(t1);
SamplerState Sampler : register(s0);

cbuffer CBPerObject : register(b1)
{
    matrix WorldMatrix;
    matrix NormalMatrix;
    
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

// ""TextureColor, TextureNormal Àû¿ë ""
PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    float4 textColor = TextureColor.Sample(Sampler, input.uv);
    clip(textColor.a - 0.09);
   
    float3 texNormal = TextureNormal.Sample(Sampler, input.uv).xyz;
    texNormal = DecodingNormal(texNormal);
    float3 worldNormal = normalize(mul(texNormal, input.TBN));
    
    output.rt0 = textColor;
    //output.rt0 = float4(input.normal, 1.0f);
    output.rt1 = float4(worldNormal, 1.0f);
    output.rt2 = float4(MaterialSpecular, MaterialShineness);
    
    return output;
}
