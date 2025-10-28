cbuffer CBPerMergeFrame : register(b0)
{
    matrix InvProjectTransform;
    matrix InvViewTransform;
    
    float2 Offset;
    float2 Scale;
    
    float4 LightColor;
    float4 AmbientColor;

    float3 SpotPosition;
    float SpotAngle;
    float3 SpotDirection;
    float SpotRange;
}

struct VS_ScreenRect
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct PS_ScreenRect
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


PS_ScreenRect main(VS_ScreenRect input)
{
    PS_ScreenRect output = (PS_ScreenRect) 0;
    float2 calcPosition = input.pos * Scale + Offset;
    output.pos = float4(calcPosition, 0.0f, 1.0f);
    output.uv = input.uv;
    
    return output;
}