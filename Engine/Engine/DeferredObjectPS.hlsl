Texture2D textureColor : register(t0);
Texture2D textureNormal : register(t1);
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



// TextureColor, TextureNormal 없을 때 쉐이더.
//PS_OUTPUT main(PS_INPUT input)
//{   
//    PS_OUTPUT output = (PS_OUTPUT) 0;
//    output.rt0 = input.color;
    
//    float3 N = normalize(input.normal);
//    output.rt1 = float4(N, 1.0f);
    
//    //float3 encodingN = EncodingNormal(N);
//    //output.rt1 = float4(encodingN, 1.0f);
    
//    output.rt2 = float4(MaterialSpecular, MaterialShineness);
    
//    return output;
//}



// ""TextureColor, TextureNormal 적용 ""
PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
    float4 textColor = textureColor.Sample(g_Sampler, input.uv);
    output.rt0 = textColor;
    
    float4 texNormal = normalize(textureNormal.Sample(g_Sampler, input.uv));
    float3 worldNormal = normalize(mul(texNormal.xyz, input.TBN));
    output.rt1 = float4(worldNormal, 1.0f);
    
    output.rt2 = float4(MaterialSpecular, MaterialShineness);
    
    return output;
}