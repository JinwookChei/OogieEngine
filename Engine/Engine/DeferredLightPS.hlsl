Texture2D renderTextureAlbedo : register(t0);
Texture2D renderTextureNormal : register(t1);
Texture2D renderTextureMaterial : register(t2);
Texture2D renderTextureDepth : register(t3);
SamplerState samplers : register(s0);

cbuffer CBPerFrame : register(b0)
{
    matrix View;
    matrix Projection;
    
    matrix InvViewTransform;
    matrix InvProjectTransform;
    
    float2 ScreenOffset;
    float2 ScreenScale;
};


cbuffer CBPerSpotLight : register(b1)
{
    float4 LightColor;
    float4 AmbientColor;

    float3 SpotPosition;
    float SpotAngle;
    float3 SpotDirection;
    float SpotRange;
};

struct PS_ScreenRect
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_ScreenRect input) : SV_TARGET
{ 
    float4 color = renderTextureAlbedo.Sample(samplers, input.uv);
    float4 normal = renderTextureNormal.Sample(samplers, input.uv);
    float depth = renderTextureDepth.Sample(samplers, input.uv).r;
    
    clip(normal.w - 0.0001f);
    
    float screen_x = input.uv.x * 2560.0f;
    float screen_y = input.uv.y * 1440.0f;
    
    float ndc_x = (2.0f * screen_x) / 2560.0f - 1.0f;
    float ndc_y = 1.0f - (2.0f * screen_y) / 1440.0f;
    float ndc_z = depth;
    float ndc_w = 1.0f;
    
    float4 ndcPos = { ndc_x, ndc_y, ndc_z, ndc_w };
    
    float4 viewPos = mul(ndcPos, InvProjectTransform);
    viewPos /= viewPos.w;
    
    float4 worldPos = mul(viewPos, InvViewTransform);

    float3 L = normalize(SpotPosition - worldPos.xyz);
    float3 S = normalize(-SpotDirection);
    
    float spotCos = dot(L, S);
    float spotEffect = smoothstep(SpotAngle, SpotAngle + 0.05, spotCos);
    
    float dist = length(SpotPosition - worldPos.xyz);
    float att = saturate(1 - dist / SpotRange);
    
    float diffuse = saturate(dot(normal.xyz, L)) * spotEffect * att;
    float4 diffuseColor = diffuse * LightColor;
    
    float4 finalColor = color * (diffuseColor + AmbientColor);
    return finalColor;
}