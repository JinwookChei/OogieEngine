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

    float3 LightDirection; // Spot, Direction이랑 공유.
    float LightRange;
    float3 LightPosition;
    float LightAngle;

    float AttenuationConst;
    float AttenuationLinear;
    float AttenuationQuad;

	// 0 -> DirectionLight
	// 1 -> SpotLight
	// 2 -> PointLight
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
        float3 R = normalize(reflect(-L, N));
        
        // Diffuse
        float diffuseFactor = max(dot(N, L), 0.0f);
        float3 diffuseColor = diffuseFactor * LightDiffuse.rgb * albedo.rgb;

        // Specular
        float rDotV = max(dot(R, V), 0.0f);
        float shininessFactor = pow(rDotV, 16);

        float3 materialSpecular = float3(0.7f, 0.7f, 0.7f);
        float3 specularColor = shininessFactor * LightSpecular.rgb * materialSpecular;

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
        float3 V = normalize(CamPos.xyz - worldPos.xyz);
        float3 R = normalize(reflect(-L, N));
        float3 S = normalize(LightPosition - worldPos.xyz);
    
        // Diffuse
        float spotCos = dot(L, S);
        float spotEffect = smoothstep(LightAngle, LightAngle + 0.05, spotCos);
    
        float dist = length(LightPosition - worldPos.xyz);
        float att = saturate(1 - dist / LightRange);
    
        float diffuseFactor = saturate(dot(N, L)) * spotEffect * att;
        float3 diffuseColor = diffuseFactor * LightDiffuse.rgb * albedo.rgb;
    
        // Specular
        float rDotV = max(dot(R, V), 0.0f);
        float shininessFactor = pow(rDotV, 16);

        // 거리/스포트 효과만 곱하기
        float lightFalloff = spotEffect * att;
        
        float3 materialSpecular = float3(0.7f, 0.7f, 0.7f);
        float3 specularColor = lightFalloff * shininessFactor * LightSpecular.rgb * materialSpecular;
        
        // Ambient
        float3 ambientColor = LightAmbient.rgb * albedo.rgb;
        
        // Emissive
        float3 emissiveColor = 0.0f;
        
        return float4(diffuseColor + specularColor + ambientColor + emissiveColor, 1.0f);
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