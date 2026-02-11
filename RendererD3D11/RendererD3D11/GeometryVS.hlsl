//cbuffer CBPerFrame : register(b0)
//{
//    matrix View;
//    matrix Projection;
    
//    matrix InvViewTransform;
//    matrix InvProjectTransform;
    
//    float2 ScreenOffset;
//    float2 ScreenScale;
    
//    float4 CamPos;
//};

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
    
    float3 MaterialSpecular;
    float MaterialShineness;
};

struct VS_INPUT
{
    float3 position : POSITION; // 정점 위치
    float4 color : COLOR; // 정점 색상
    float3 normal : NORMAL; // (사용 안 함)
    float2 uv : TEXCOORD; // (사용 안 함)
    float4 tangent : TANGENT; // (사용 안 함)
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


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    float4 worldPosition = mul(float4(input.position, 1.0f), World); // 월드 포지션 ( 원점을 기준으로 얼마만큼 떨어져있나 )
    float4 viewPosition = mul(worldPosition, View); // 뷰 포지션 ( 카메라를 기준으로 둔 포지션 ) ( 카메라 기준이란? 카메라의 포지션을 0, 0, 0 으로 본다 ) ( 카메라 기준이란? 카메라를 원점으로 만든다. )
    output.svPos = mul(viewPosition, Projection);
    output.color = input.color;
    
    float3 N = normalize(mul(input.normal, (float3x3) World));
    float3 T = normalize(mul(input.tangent.xyz, (float3x3) World));
    float3 B = cross(N, T) * input.tangent.w;
    
    float3x3 normalMatrix = (float3x3) World;
    output.normal = mul(input.normal, normalMatrix);
    output.worldPos = worldPosition.xyz;
    output.uv = input.uv;
    output.TBN = float3x3(T, B, N);
    
    return output;
}
