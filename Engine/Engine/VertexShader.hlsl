cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    
    float4 lightDir;
    float4 lightColor;
    float4 ambientColor;
    
    float3 spotPosition;
    float spotRange;
    float3 spotDirection;
    float spotAngle;
    
    float3 pointPosition;
    float pointRange;
    
    float attenuationConst;
    float attenuationLinear;
    float attenuationQuad;
    float pad1;
}

struct VS_INPUT
{
    float3 position : POSITION; // ���� ��ġ
    float4 color : COLOR; // ���� ����
    float3 normal : NORMAL; // (��� �� ��)
    float2 uv : TEXCOORD; // (��� �� ��)
    float4 tangent : TANGENT; // (��� �� ��)
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

//PS_INPUT main(VS_INPUT input)
//{
//    PS_INPUT output;

//    // �� ��ǥ -> ���� ��ǥ
//    float4 worldPos = mul(float4(input.position, 1.0f), World);

//    // ���� ��ǥ -> �� ��ǥ
//    float4 viewPos = mul(worldPos, View);

//    // �� ��ǥ -> Ŭ�� ��ǥ
//    output.position = mul(viewPos, Projection);

//    // ���� ���� �״�� �ѱ�
//    output.color = input.color;

//    return output;
//}


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

//// SV_Semantic vs �Ϲ� Semantic
//// HLSL���� �ø�ƽ(Semantic)�� ���̴��� � �ǹ��� �����͸� �ְ�޴����� ��Ÿ���� ������ "�±�"�Դϴ�. 
//// �� �ø�ƽ�� ũ�� �� ������ �����ϴ�. �ϳ��� SV_�� �����ϴ� �ý��� �ø�ƽ�̰�, �ٸ� �ϳ��� ����ڰ� ������ �Ϲ� �ø�ƽ�Դϴ�.
//// ���� SV_ �ø�ƽ�� 'System Value'�� ���ڷ�, GPU�� ����̹�, Direct3D �ý����� �ڵ����� ���� �����ϰų� Ư���ϰ� �ؼ��ϴ� �����Դϴ�.
//// ���� ��� SV_POSITION�� ���� ���̴��� ����� �� �ʿ��� ���ε�, �̰��� ��ȯ�� ������ ��ġ�� �ǹ��ϸ�,
//// GPU�� �ļ� ���������ο��� ȭ��� �ȼ� ��ġ�� ����ϱ� ���� �ݵ�� �����մϴ�. �� SV_TARGET�� �ȼ� ���̴����� ���������� ������ ����� �� ����ϴµ�, 
//// �̰� ���� GPU�� ��� ���ۿ� ����� �� ���̴� �ý��� ���Դϴ�. ����ڰ� ���� �غ��ϰų� ���۷� �Ѱ��ִ� ���� �ƴ϶� GPU�� ó�� �帧�� �ڵ����� ó���ϰ� �Ѱ��ݴϴ�.
//// ���� �̷� �ø�ƽ�� �ݵ�� �ش� ��ġ���� ���Ǿ�� �ϸ�, ������ ���̴��� ����� �۵����� �ʽ��ϴ�. �̷� ������ '�ʼ���'�̰� 'Ư���� �ǹ�'�� �����ϴ�.
//// �ݸ�, �Ϲ� �ø�ƽ�� ����ڰ� ���� �����Ͽ� ���ؽ� ���۳� ���̴� ����� ����ü�� ���� �ѱ�� �������Դϴ�. 
//// ��ǥ������ POSITION, NORMAL, TEXCOORD ���� �͵��� �ֽ��ϴ�. ���� ���, �޽� �����Ϳ� ����� ���� ��ġ, ���� ����, �ؽ�ó ��ǥ ���� 
//// ���̴��� �ѱ� �� �̷� �ø�ƽ�� ����մϴ�. �� ������ ����ڰ� �ʿ信 ���� �����ϸ�, GPU�� �ڵ����� ä���ִ� ���� �ƴ϶� 
//// ���ø����̼� �ڵ峪 ���ؽ� ���ۿ� ����ڰ� ���� ���� �״�� ���̴��� �޽��ϴ�. �׷��� �� �ø�ƽ���� '����� ������'�̸� '��� ������ ���� �����Ӱ�' ���˴ϴ�.
//// �ٽ����� ���̴� �ٷ� �� �κ��Դϴ�. SV_ �ø�ƽ�� GPU�� ���̴� ������ ���� �ݵ�� �˾ƾ� �ϴ� ���̰� �ý����� ���� ����������, 
//// �Ϲ� �ø�ƽ�� ������ ����ڰ� �ʿ信 ���� �����ϰ� �ѱ�� �������Դϴ�. ���� ��� � ���̴��� ������ ����Ϸ��� NORMAL �ø�ƽ�� �ʿ��� �� ������, 
//// �̰��� �������� ������� �����̸� �ý����� �������� �ʽ��ϴ�. �׷��� SV_POSITION�� �ݵ�� �ʿ��ϸ� ���� ���̴����� �� ���� ������� ������ ������ ��ü�� �Ұ����մϴ�.
//// Ȱ�� �鿡���� ���̰� �ֽ��ϴ�. SV �ø�ƽ�� ���������� ����, ������ �ĺ�, ��ġ ����, ��� ��� ���� �� �ý������� �ǹ̰� ���ϸ�, 
//// �Ϲ� �ø�ƽ�� �޽��� ���� �����ͳ� �ؽ�ó ��ǥó�� �׷��Ƚ� ǥ�� ��ü�� �ʿ��� �����͸� �����ϴ� �� ���˴ϴ�.
//// ���������, SV �ø�ƽ�� �ý��� ������ �ʼ� ���� ���޿��̰�, �Ϲ� �ø�ƽ�� ����ڰ� ������ ������ ���޿��Դϴ�. 
//// �� ���� �����ϰ� ������ �°� ����ϴ� ���� HLSL ���̴� ������ �߿��� �⺻�Դϴ�.
