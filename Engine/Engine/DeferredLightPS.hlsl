Texture2D renderTextureAlbedo : register(t0);
Texture2D renderTextureNormal : register(t1);
Texture2D renderTextureSpecular : register(t2);
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
    float2 ScreenResolution;
    float2 Pad0;
    
    float4 CamPos;
};


cbuffer CBPerSpotLight : register(b1)
{
    float4 LightDiffuse;
    float4 LightSpecular;
    float4 LightAmbient;

    float3 LightDirection; // Spot, DirectionÀÌ¶û °øÀ¯.
    float LightRange;
    float3 LightPosition;

    float SpotExponent;
    float InnerAngle;
    float OuterAngle;
    
    float AttenuationConst;
    float AttenuationLinear;
    float AttenuationQuad;

	// 0 -> DirectionLight
	// 1 -> SpotLight
	// 2 -> PointLight
    float LightType;
    
    
    float2 Pad;
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
    float4 specular = renderTextureSpecular.Sample(samplers, input.uv);
    float depth = renderTextureDepth.Sample(samplers, input.uv).r;
    
    clip(normal.w - 0.0001f);
    
    // Calc WorldPos From Depth + PixelPosition
    float screen_x = input.uv.x * ScreenResolution.x;
    float screen_y = input.uv.y * ScreenResolution.y;
    float ndc_x = (2.0f * screen_x) / ScreenResolution.x - 1.0f;
    float ndc_y = 1.0f - (2.0f * screen_y) / ScreenResolution.y;
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
        float diffuseFactor = saturate(dot(N, L));
        clip(diffuseFactor - 0.0001f);
        float3 diffuseColor = diffuseFactor * LightDiffuse.rgb * albedo.rgb;

        // Specular
        float rDotV = saturate(dot(R, V));
        float shineness = specular.a;
        float specualrFactor = pow(rDotV, shineness);
        float3 specularColor = specualrFactor * LightSpecular.rgb * specular.rgb;

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
        float3 toEye = normalize(CamPos.xyz - worldPos.xyz);
        float3 toLight = LightPosition - worldPos.xyz;
        float dist = length(toLight);
        
        clip(LightRange - dist);
        toLight /= dist;
        
        // Ambient
        float3 ambientColor = LightAmbient.rgb * albedo.rgb;
        
        // Diffuse
        float diffuseFactor = dot(toLight, N);
        clip(diffuseFactor - 0.0001f);
        float3 diffuseColor = diffuseFactor * LightDiffuse.rgb * albedo.rgb;
        
        // Specular
        float3 R = reflect(-toLight, N);
        float shineness = specular.a;
        float specFactor = pow(max(dot(R, toEye), 0.0f), shineness);
        float3 specularColor = specFactor * LightSpecular.rgb * specular.rgb;
        
        // Emissive
        float3 emissiveColor = 0.0f;
        
        //// Att        
        float innerCone = cos(radians(InnerAngle)); 
        float outerCone = cos(radians(OuterAngle));
        float theta = dot(-toLight, LightDirection);
        float spotIntensity = smoothstep(outerCone, innerCone, theta);
        float spot = pow(max(spotIntensity, 0.0f), SpotExponent);
        float3 attCoeffs = float3(AttenuationConst, AttenuationLinear, AttenuationQuad);
        float attenuation = 1.0f / dot(attCoeffs, float3(1.0f, dist, dist * dist));
        float att = spot * attenuation;
        
        float3 finalColor = float3(diffuseColor + specularColor + ambientColor + emissiveColor) * att;
        return float4(finalColor, 1.0f);
    }
    // PointLight
    else
    {        
        //return float4(specular.rgb, 1.0f);
        //return float4(specular.w,0.0f, 0.0f, 1.0f);
        
        float3 N = normalize(normal.xyz);
        float3 toEye = normalize(CamPos.xyz - worldPos.xyz);
        float3 toLight = LightPosition - worldPos.xyz;
        float3 lightVec = LightPosition - worldPos.xyz;
        
        float dist = length(lightVec);
        //clip(LightRange - dist);
        lightVec /= dist;
        
        // Diffuse
        float diffuseFactor = dot(lightVec, N);
        //clip(diffuseFactor - 0.0001f);
        float3 diffuseColor = diffuseFactor * LightDiffuse.rgb * albedo.rgb;
        
        // Specular
        float3 R = reflect(-lightVec, N);
        float shineness = pow(2, ((specular.w * 10) - 1));
        float specularFactor = pow(max(dot(R, toEye), 0.0f), shineness);
        float3 materialSpecular = float3(0.7f, 0.7f, 0.7f);
        float specularColor = specularFactor * LightSpecular.rgb * specular.rgb;
        
        // Ambient
        float3 ambientColor = LightAmbient.rgb * albedo.rgb;
        
        // °¨¼è
        float3 attCoeffs = float3(AttenuationConst, AttenuationLinear, AttenuationQuad);
        float att = 1.0f / dot(attCoeffs, float3(1.0f, dist, dist * dist));
        
        // Emissive
        float3 emissiveColor = 0.0f;
        
        float3 finalColor = float3(diffuseColor + specularColor + ambientColor + emissiveColor) * att;
        return float4(finalColor, 1.0f);
    }
    
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}