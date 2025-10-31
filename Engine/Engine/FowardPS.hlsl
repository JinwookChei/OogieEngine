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


cbuffer CBPerObject : register(b1)
{
    matrix World;
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

float4 main(PS_INPUT input) : SV_TARGET
{   
    //float3 N = normalize(input.normal);
    //float3 L = normalize(spotPosition - input.worldPos);
    //float3 S = normalize(-spotDirection);
    
    //float spotCos = dot(L, S);
    //float spotEffect = smoothstep(spotAngle, spotAngle + 0.05, spotCos);
    
    //float dist = length(spotPosition - input.worldPos);
    //float att = saturate(1 - dist / spotRange);
    
    //float diffuse = saturate(dot(N, L)) * spotEffect * att;
    //float4 diffuseColor = diffuse * lightColor;
    
    //float4 finalColor = input.color * (diffuseColor + ambientColor);
    
    float4 finalColor = input.color;
    return finalColor;
    
}