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
    
    float4 CamPos;
};


cbuffer CBPerSpotLight : register(b1)
{
    float4 LightDiffuse;
    float4 LightSpecular;
    float4 LightAmbient;

	// DirectionLight
    float3 LightDirection; // spotDirectionÀÌ¶û °øÀ¯.

	// SpotLight
    float SpotRange;
    float3 SpotPosition;
    float SpotAngle;

	// PointLight
    float3 PointPosition;
    float PointRange;
    
    float AttenuationConst;
    float AttenuationLinear;
    float AttenuationQuad;
    float LightType;
};

struct PS_ScreenRect
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_ScreenRect input) : SV_TARGET
{
    float4 albedo = renderTextureAlbedo.Sample(samplers, input.uv);
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
    
    // DirectionLight
    if (LightType == 0)
    {
        float3 N = normalize(normal.xyz);
        float3 L = normalize(-LightDirection); 
        float3 V = normalize(CamPos.xyz - worldPos.xyz); 

        // Diffuse
        float nDotL = max(dot(N, L), 0.0f);
        float3 diffuseColor = nDotL * LightDiffuse.rgb * albedo.rgb;

        // Specular
        float3 R = normalize(reflect(-L, N)); 
        float rDotV = max(dot(R, V), 0.0f);
        float adjShininess = pow(rDotV, 16);

        float3 materialSpecular = float3(0.7f, 0.7f, 0.7f);
        float3 specularColor = adjShininess * LightSpecular.rgb * materialSpecular;

        // Ambient
        float3 ambientColor = LightAmbient.rgb * albedo.rgb;

        // Emissive
        float3 emissiveColor = 0.0f;

        return float4(diffuseColor + specularColor + ambientColor + emissiveColor, 1.0f);
    }
    // SpotLight
    else if (LightType == 1)
    {
        float3 N = normalize(normal.xyz);
        float3 L = normalize(-LightDirection);
        float3 S = normalize(SpotPosition - worldPos.xyz);
    
        // Diffuse
        float spotCos = dot(L, S);
        float spotEffect = smoothstep(SpotAngle, SpotAngle + 0.05, spotCos);
    
        float dist = length(SpotPosition - worldPos.xyz);
        float att = saturate(1 - dist / SpotRange);
    
        float diffuseFactor = saturate(dot(N, L)) * spotEffect * att;
        float3 diffuseColor = diffuseFactor * LightDiffuse.rgb * albedo.rgb;
    
        float4 finalColor = float4(diffuseColor.rgb, 1.0f);
        return finalColor;
    }
    // PointLight
    else
    {
        
    }
    
    
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    //float screen_x = input.uv.x * 2560.0f;
    //float screen_y = input.uv.y * 1440.0f;
    
    //float ndc_x = (2.0f * screen_x) / 2560.0f - 1.0f;
    //float ndc_y = 1.0f - (2.0f * screen_y) / 1440.0f;
    //float ndc_z = depth;
    //float ndc_w = 1.0f;
    
    //float4 ndcPos = { ndc_x, ndc_y, ndc_z, ndc_w };
    
    //float4 viewPos = mul(ndcPos, InvProjectTransform);
    //viewPos /= viewPos.w;
    
    //float4 worldPos = mul(viewPos, InvViewTransform);

    //float3 L = normalize(SpotPosition - worldPos.xyz);
    //float3 S = normalize(-LightDirection);
    
    //float spotCos = dot(L, S);
    //float spotEffect = smoothstep(SpotAngle, SpotAngle + 0.05, spotCos);
    
    //float dist = length(SpotPosition - worldPos.xyz);
    //float att = saturate(1 - dist / SpotRange);
    
    //float diffuse = saturate(dot(normal.xyz, L)) * spotEffect * att;
    //float4 diffuseColor = diffuse * LightDiffuse;
    
    //float4 finalColor = albedo * (diffuseColor + LightAmbient);
    //return finalColor;
}