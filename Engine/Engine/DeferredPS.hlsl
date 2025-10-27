Texture2D g_Texture : register(t0);
Texture2D g_NormalMap : register(t1);
SamplerState g_Sampler : register(s0);


cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    
    float4 lightDir;
    float4 lightColor;
    float4 ambientColor;
    
    float3 spotPosition;
    float spotRange;
    float3 spotDirection;
    float spotAngle;
    
    float3 pointPosition;
    float pointRange;
    
    float attenuationConst;
    float attenuationLinear;
    float attenuationQuad;
    float pad1;
}

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
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
    output.rt0 = input.Color;
    
    float3 N = normalize(input.normal);
    float3 L = normalize(spotPosition - input.worldPos);
    float3 S = normalize(-spotDirection);
    
    float3 tmpN = N;
    //float3 tmpN = 0.5f * (N + 1.0f);
    output.rt1 = float4(tmpN, 1.0f);
    
    //
    float spotCos = dot(L, S);
    float spotEffect = smoothstep(spotAngle, spotAngle + 0.05, spotCos);
    
    float dist = length(spotPosition - input.worldPos);
    float att = saturate(1 - dist / spotRange);
    
    float diffuse = saturate(dot(N, L)) * spotEffect * att;
    float4 diffuseColor = diffuse * lightColor;
    
    float4 finalColor = input.Color * (diffuseColor + ambientColor);
    //
    
    return output;
}