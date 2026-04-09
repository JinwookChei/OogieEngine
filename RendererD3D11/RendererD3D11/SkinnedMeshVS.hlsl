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

cbuffer CBPerObject : register(b1)
{
    matrix WorldMatrix;
    matrix NormalMatrix;
    float4 Scale;
    float3 MaterialSpecular;
    float MaterialShineness;
};

// TODO : StructBuffer로 바꿔야할듯
cbuffer CBPerAnimation : register(b2)
{
    float4x4 BoneTransforms[200];
}

struct VS_INPUT
{
    float3 position : POSITION; // 정점 위치
    float4 color : COLOR; // 정점 색상
    float3 normal : NORMAL; // (사용 안 함)
    float2 uv : TEXCOORD; // (사용 안 함)
    float4 tangent : TANGENT; // (사용 안 함)
    
    uint4 boneIndices : BONEINDICES;
    float4 blendWeights : BLENDWEIGHT;
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
    
    float4x4 skinTransform = 0;
    skinTransform += mul(input.blendWeights.x, BoneTransforms[(uint) input.boneIndices.x]);
    skinTransform += mul(input.blendWeights.y, BoneTransforms[(uint) input.boneIndices.y]);
    skinTransform += mul(input.blendWeights.z, BoneTransforms[(uint) input.boneIndices.z]);
    skinTransform += mul(input.blendWeights.w, BoneTransforms[(uint) input.boneIndices.w]);
    
    float4 skinnedPos = mul(float4(input.position, 1.0f), skinTransform);
    float4 worldPos = mul(skinnedPos, WorldMatrix);
    float4 viewPos = mul(worldPos, View);
    output.svPos = mul(viewPos, Projection);
    output.worldPos = worldPos.xyz;
    output.color = input.color;
    output.uv = input.uv;
    
    float3 N = mul(input.normal, (float3x3) skinTransform);
    float3 T = mul(input.tangent.xyz, (float3x3) skinTransform);
    
    // World (scale 없음 가정)
    N = normalize(mul(N, (float3x3) NormalMatrix));
    T = normalize(mul(T, (float3x3) NormalMatrix));
    float3 B = normalize(cross(N, T)) * input.tangent.w;
   
    output.normal = N;
    output.TBN = float3x3(T, B, N);
    
    return output;
}
