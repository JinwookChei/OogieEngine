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
    matrix WorldMatrix;
    matrix NormalMatrix;
    float4 Scale;
};

cbuffer CBPerMaterial : register(b2)
{
    float3 MaterialSpecular;
    float MaterialShineness;
};


cbuffer CBPerLight : register(b3)
{
    float4 LightDiffuse;
    float4 LightSpecular;
    float4 LightAmbient;

    float3 LightDirection;
    float LightRange;
    float3 LightPosition;

    float Smooth;
    float InnerAngle;
    float OuterAngle;
    
    float AttenuationConst;
    float AttenuationLinear;
    float AttenuationQuad;

	// 0 -> DirectionLight
	// 1 -> SpotLight
	// 2 -> PointLight
    float LightType;
    float LightIntensity;
    
    float IsFirst;
};

struct PS_INPUT
{
    float4 svPos : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3x3 TBN : TEXCOORD2;
};

struct PS_OUTPUT
{
    float4 rt0 : SV_TARGET0;
    float4 rt1 : SV_TARGET1;
    float4 rt2 : SV_TARGET2;
    float4 rt3 : SV_TARGET3;
};

float3 EncodingNormal(float3 normal)
{
    return 0.5f * (normalize(normal) + 1.0f);
}

float3 DecodingNormal(float3 normal)
{
    return normalize(normal * 2.0f - 1.0f);
}

//// ""TextureColor, TextureNormal 적용 ""
//PS_OUTPUT main(PS_INPUT input)
//{
//    PS_OUTPUT output = (PS_OUTPUT) 0;
    
//    output.rt0 = input.color;
//    output.rt1 = float4(input.normal, 1.0f);
//    output.rt2 = float4(MaterialSpecular, MaterialShineness);
//    output.rt3 = float4(input.worldPos, 1.0f);
//    return output;
//}



float4 main(PS_INPUT input) : SV_TARGET
{
    float4 albedo = input.color;
    float4 normal = float4(input.normal, 0.0f);
    float4 specular = float4(MaterialSpecular, 1.0f);
    float4 position = float4(input.worldPos, 1.0f);
    
    if (IsFirst)
    {
        float4 tmp = albedo * 0.2f;
        return float4(tmp.xyz, 1.0f);
        //return float4(0.2f, 0.0f, 0.0f, 1.0f);
    }
    // 물체를 비추는 픽셀인지 아닌지 검사.
    //clip(normal.w - 0.0001f);

    // DirectionLight
    if (LightType == 0)
    {
        float3 N = normalize(normal.xyz);
        float3 L = normalize(-LightDirection);
        float3 V = normalize(CamPos.xyz - position.xyz);
        float3 R = normalize(reflect(-L, N));
        
        // Diffuse
        float diffuseFactor = saturate(dot(N, L));
        clip(diffuseFactor - 0.0001f);
        float3 diffuseColor = diffuseFactor * LightDiffuse.rgb * albedo.rgb;

        // Specular
        float rDotV = saturate(dot(R, V));
        float shineness = specular.w * 64.0f;
        float specualrFactor = pow(rDotV, shineness);
        float3 specularColor = specualrFactor * LightSpecular.rgb * specular.rgb;
        float3 finalColor = float3(diffuseColor + specularColor);
        return float4(finalColor * LightIntensity, 1.0f);
    }
    
    // SpotLight
    else if (LightType == 1)
    {
        float3 N = normalize(normal.xyz);
        float3 toEye = normalize(CamPos.xyz - position.xyz);
        float3 toLight = LightPosition - position.xyz;
        float dist = length(toLight);
        
        clip(LightRange - dist);
        toLight /= dist;
        
        // Diffuse
        float diffuseFactor = saturate(dot(toLight, N));
        float3 diffuseColor = diffuseFactor * LightDiffuse.rgb * albedo.rgb;
        
        // Specular
        float3 R = reflect(-toLight, N);
        float shineness = specular.w * 64.0f;
        float specFactor = pow(max(dot(R, toEye), 0.0f), shineness);
        float3 specularColor = specFactor * LightSpecular.rgb * specular.rgb;
        
        //// Att        
        float innerCone = cos(radians(InnerAngle));
        float outerCone = cos(radians(OuterAngle));
        float theta = dot(-toLight, LightDirection);
        float spotIntensity = smoothstep(outerCone, innerCone, theta);
        float spot = pow(max(spotIntensity, 0.0f), Smooth);
        float3 attCoeffs = float3(AttenuationConst, AttenuationLinear, AttenuationQuad);
        float attenuation = 1.0f / dot(attCoeffs, float3(1.0f, dist, dist * dist));
        float att = spot * attenuation;
        
        float3 finalColor = float3(diffuseColor + specularColor) * att;
        return float4(finalColor * LightIntensity, 1.0f);
    }
    
    // PointLight
    else
    {
        float3 N = normalize(normal.xyz);
        float3 toEye = normalize(CamPos.xyz - position.xyz);
        float3 toLight = LightPosition - position.xyz;
        float3 lightVec = LightPosition - position.xyz;
        
        float dist = length(lightVec);
        clip(LightRange - dist);
        lightVec /= dist;
        
        // Diffuses
        float diffuseFactor = saturate(dot(lightVec, N));
        clip(diffuseFactor - 0.0001f);
        float3 diffuseColor = diffuseFactor * LightDiffuse.rgb * albedo.rgb;
        
        // Specular
        float3 R = reflect(-lightVec, N);
        float shineness = specular.w * 64.0f;
        float specularFactor = pow(max(dot(R, toEye), 0.0f), shineness);
        float3 specularColor = specularFactor * LightDiffuse.rgb * specular.rgb;
        
        // Att
        float3 attCoeffs = float3(AttenuationConst, AttenuationLinear, AttenuationQuad);
        float att = 1.0f / dot(attCoeffs, float3(1.0f, dist, dist * dist));
        
        float3 finalColor = float3(diffuseColor + specularColor) * att;
        return float4(finalColor * LightIntensity, 1.0f);
    }
    
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}