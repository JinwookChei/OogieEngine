#pragma once

enum class E_VERTEX_FORMAT
{
	SIMPLE,
	SCREEN_RECT
};


struct SimpleVertex
{
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

struct ScreenRectVertex
{
	Float2 position;
	Float2 UV;
};