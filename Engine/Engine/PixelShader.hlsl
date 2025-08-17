Texture2D g_Texture : register(t0);
Texture2D g_NormalMap : register(t1);
SamplerState g_Sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 color;
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
    float3 worldPos : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3x3 TBN : TEXCOORD2;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 texColor = g_Texture.Sample(g_Sampler, input.uv);
    float3 normalTxture = UnpackNormal(g_NormalMap.Sample(g_Sampler, input.uv));
    float3 normalW = normalize(mul(normalTxture, input.TBN));
    
    float3 N = normalize(normalW);
    float3 L = normalize(pointPosition - input.worldPos);
    float dist = length(pointPosition - input.worldPos);
    
    float att = 1.0f / (attenuationConst + attenuationLinear * dist + attenuationQuad * dist * dist);
    
    att *= step(dist, pointRange);
    
    float diffuse = saturate(dot(N, L)) * att;
    float4 diffuseColor = diffuse * lightColor;
    
    float3 normalColor = 0.5f * N + 0.5f;
    float gray = dot(normalColor, float3(0.299f, 0.587f, 0.114f));
    normalColor = float3(gray, gray, gray);
    
    float4 finalColor = float4(normalColor * (diffuseColor.xyz + ambientColor.xyz), 1.0f);
    
    return finalColor;

}
