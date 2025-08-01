#pragma once
struct SimpleVertex {
	Vector position;
	Vector color;
	Vector normal;
	Vector UV;
	Vector tangent;

	SimpleVertex(Vector pos, Vector tmpcolor, Vector tmpnormal, Vector uv, Vector tan)
		: position(pos), color(tmpcolor), normal(tmpnormal), UV(uv), tangent(tan) {
	}
	SimpleVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 tmpcolor, DirectX::XMFLOAT4 tmpnormal, DirectX::XMFLOAT2 uv)
		: position(pos), color(tmpcolor), normal(tmpnormal), UV(uv) {
	}
};

bool CreateSphere(std::vector<SimpleVertex>* outVertices, std::vector<WORD>* outIndices, float radius = 0.5f);