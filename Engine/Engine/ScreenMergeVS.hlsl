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
    float2 calcPosition = input.pos * ScreenScale + ScreenOffset;
    output.pos = float4(calcPosition, 0.0f, 1.0f);
    output.uv = input.uv;
    
    return output;
}