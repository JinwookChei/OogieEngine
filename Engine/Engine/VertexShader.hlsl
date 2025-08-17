cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 color;
    float4 lightDir;
    float4 lightColor;
    float4 ambientColor;
    
    float3 spotPosition;
    float spotRange;
    float3 spotDirection;
    float spotAngle;
}

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
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3x3 TBN : TEXCOORD2;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    float4 worldPosition = mul(float4(input.position, 1.0f), World); // ���� ������ ( ������ �������� �󸶸�ŭ �������ֳ� )
    float4 viewPosition = mul(worldPosition, View); // �� ������ ( ī�޶� �������� �� ������ ) ( ī�޶� �����̶�? ī�޶��� �������� 0, 0, 0 ���� ���� ) ( ī�޶� �����̶�? ī�޶� �������� �����. )
    output.Pos = mul(viewPosition, Projection);
    output.Color = input.color;
    
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