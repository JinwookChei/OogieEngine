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

struct VS_INPUT
{
    float3 position : POSITION; 
    float4 color : COLOR;
    float3 normal : NORMAL; 
    float2 uv : TEXCOORD; 
    float4 tangent : TANGENT; 
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
    
    float4 worldPosition = mul(float4(input.position, 1.0f), WorldMatrix); 
    float4 viewPosition = mul(worldPosition, View); 
    output.svPos = mul(viewPosition, Projection);
    output.color = input.color;
    
    float3 N = normalize(mul(input.normal, (float3x3) NormalMatrix));
    float3 T = normalize(mul(input.tangent.xyz, (float3x3) NormalMatrix));
    float3 B = normalize(cross(N, T)) * input.tangent.w;
    
    output.normal = N;
    output.worldPos = worldPosition.xyz;
    output.uv = input.uv;
    output.TBN = float3x3(T, B, N);
    
    return output;
}
