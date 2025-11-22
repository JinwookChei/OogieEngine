cbuffer CBViewProj : register(b0)
{
    matrix view;
    matrix Proj;
};

struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VSOutput main(VSInput input)
{
    VSOutput output;
            
    float4 viewPos = mul(float4(input.pos, 1.0f), view);
    output.pos = mul(viewPos, Proj);
    output.color = input.color;
    return output;
}