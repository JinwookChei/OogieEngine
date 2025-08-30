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
    // PointLight Pos
    float3 worldPos : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{   
    float3 N = normalize(input.normal);
    float3 L = normalize(spotPosition - input.worldPos);
    float3 S = normalize(-spotDirection);
    
    float spotCos = dot(L, S);
    float spotEffect = smoothstep(spotAngle, spotAngle + 0.05, spotCos);
    
    float dist = length(spotPosition - input.worldPos);
    float att = saturate(1 - dist / spotRange);
    
    float diffuse = saturate(dot(N, L)) * spotEffect * att;
    float4 diffuseColor = diffuse * lightColor;
    
    float4 finalColor = input.Color * (diffuseColor + ambientColor);
    
    return finalColor;
}