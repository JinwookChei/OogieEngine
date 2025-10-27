#pragma once

struct SimpleVertex {
	Float3 position;
	Float4 color;
	Float3 normal;
	Float2 UV;
	Float4 tangent;

	SimpleVertex(Float3 pos, Float4 tmpcolor, Float3 tmpnormal, Float2 uv, Float4 tan)
		: position(pos), color(tmpcolor), normal(tmpnormal), UV(uv), tangent(tan) {
	}

	SimpleVertex(Float3 pos, Float4 tmpcolor, Float3 tmpnormal, Float2 uv)
		: position(pos), color(tmpcolor), normal(tmpnormal), UV(uv), tangent() {
	}
};

struct ConstantBuffer
{
	Float4x4 world;
	Float4x4 view;
	Float4x4 projection;

	Float4 lightDir;
	Float4 lightColor;
	Float4 ambientColor;

	Float3 spotPosition;
	float spotRange;
	Float3 spotDirection;
	float spotAngle;

	// point light
	Float3 pointPosition;
	float pointLightRange;

	// 감쇠 계수
	float attenuationConst;
	float attenuationLinear;
	float attenuationQuad;
	float pad1;
};

struct ScreenRectVertex
{
	Float2 position;
	Float2 UV;
};


struct ScreenRectConstant
{
	Float4x4 invProjectTransform;
	Float4x4 invViewTransform;

	Float2 offset;
	Float2 scale;

	Float4 lightColor;
	Float4 ambientColor;

	Float3 spotPosition;
	float spotAngle;
	Float3 spotDirection;
	float spotRange;
	


	//// TODO : Test용도
	//int gBufferTextureNum_ = 0;
	//int pad1 = 0;
	//int pad2 = 0;
	//int pad3 = 0;
};
