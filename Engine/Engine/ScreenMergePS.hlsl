Texture2D renderTextureAlbedo : register(t0);
Texture2D renderTextureNormal : register(t1);
Texture2D renderTextureMaterial : register(t2);
Texture2D renderTextureDepth : register(t3);
Texture2D renderTexture : register(t4);
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



struct PS_ScreenRect
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


//float4 main(PS_ScreenRect input) : SV_TARGET
//{
//    float4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
    
//    if (gBufferTextureNum == 0)
//    {
//        color = renderTextureAlbedo.Sample(samplers, input.uv);
//    }
//    else if (gBufferTextureNum == 1)
//    {
//        color = renderTextureNormal.Sample(samplers, input.uv);
//    }
//    else if (gBufferTextureNum == 2)
//    {
//        color = renderTextureMaterial.Sample(samplers, input.uv);
//    }
//    else if (gBufferTextureNum == 3)
//    {
//        color = renderTextureDepth.Sample(samplers, input.uv);
//    }
//}

float4 main(PS_ScreenRect input) : SV_TARGET
{    
    return renderTexture.Sample(samplers, input.uv);
}

// Depth Shading
//float4 main(PS_ScreenRect input) : SV_TARGET
//{
//    float depth = renderTextureDepth.Sample(samplers, input.uv).r;
//    //// 비선형 → 선형 변환
//    float near = 0.1f;
//    float far = 100.0f;
//    float linearDepth = (near * far) / (far - depth * (far - near));
//    linearDepth /= far; // 0~1로 정규화
//    return float4(1.0 - linearDepth.xxx, 1.0f);
//}


// Direction Light 적용
//float4 main(PS_ScreenRect input) : SV_TARGET
//{
//    //Direction LightTest
//    float4 color = renderTextureAlbedo.Sample(samplers, input.uv);
//    float4 normal = renderTextureNormal.Sample(samplers, input.uv);
//    float depth = renderTextureDepth.Sample(samplers, input.uv).r;

//    float3 lightDir = -spotDirection;
//    float nDotL = dot(normalize((float3) normal), normalize(lightDir));
//    float nDotlClamp = saturate(nDotL);
    
//    float3 diffuseLight = nDotlClamp * lightColor.rgb * color.rgb;
//    float4 finalColor = float4(diffuseLight, 1.0);
//    return finalColor;
//}


// ScreenPixel 좌표 + Depth 값을 이용해서 WorldPos 구한 후 Point Light 적용.
//float4 main(PS_ScreenRect input) : SV_TARGET
//{
//    float4 color = renderTextureAlbedo.Sample(samplers, input.uv);
//    float4 normal = renderTextureNormal.Sample(samplers, input.uv);
//    float depth = renderTextureDepth.Sample(samplers, input.uv).r;
//    clip(normal.w - 0.0001f);
    
//    float screen_x = input.uv.x * 2560.0f;
//    float screen_y = input.uv.y * 1440.0f;
    
//    float ndc_x = (2.0f * screen_x) / 2560.0f - 1.0f;
//    float ndc_y = 1.0f - (2.0f * screen_y) / 1440.0f;
//    float ndc_z = depth;
//    float ndc_w = 1.0f;
    
//    float4 ndcPos = { ndc_x, ndc_y, ndc_z, ndc_w };
    
//    float4 viewPos = mul(ndcPos, InvProjectTransform);
//    viewPos /= viewPos.w;
    
//    float4 worldPos = mul(viewPos, InvViewTransform);

//    float3 L = normalize(SpotPosition - worldPos.xyz);
//    float3 S = normalize(-SpotDirection);
    
//    float spotCos = dot(L, S);
//    float spotEffect = smoothstep(SpotAngle, SpotAngle + 0.05, spotCos);
    
//    float dist = length(SpotPosition - worldPos.xyz);
//    float att = saturate(1 - dist / SpotRange);
    
//    float diffuse = saturate(dot(normal.xyz, L)) * spotEffect * att;
//    float4 diffuseColor = diffuse * LightColor;
    
//    float4 finalColor = color * (diffuseColor + AmbientColor);
//    return finalColor;
//}