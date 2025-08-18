#pragma once

struct SimpleVertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT4 tangent;

	/*SimpleVertex(Vector pos, Vector tmpcolor, Vector tmpnormal, Vector uv, Vector tan)
		: position(pos), color(tmpcolor), normal(tmpnormal), UV(uv), tangent(tan) {
	}*/

	SimpleVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 tmpcolor, DirectX::XMFLOAT3 tmpnormal, DirectX::XMFLOAT2 uv)
		: position(pos), color(tmpcolor), normal(tmpnormal), UV(uv), tangent() {
	}
};

struct ConstantBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT4 lightDir;
	DirectX::XMFLOAT4 lightColor;
	DirectX::XMFLOAT4 ambientColor;

	DirectX::XMFLOAT3 spotPosition;
	float spotRange;
	DirectX::XMFLOAT3 spotDirection;
	float spotAngle;

	// point light
	DirectX::XMFLOAT3 pointPosition;
	float pointLightRange;

	// °¨¼è °è¼ö
	float attenuationConst;
	float attenuationLinear;
	float attenuationQuad;
	float pad1;
};

bool CreateSphere(std::vector<SimpleVertex>* outVertices, std::vector<WORD>* outIndices, float radius = 0.5f);