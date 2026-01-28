#pragma once

struct Ray
{
	Float4 origin_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	Float4 dir_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	float maxDistance_ = 0.0f;
};

struct AABB
{
	Float3 minPos_ = { 0.0f, 0.0f, 0.0f };
	Float3 maxPos_ = { 0.0f, 0.0f, 0.0f };
};

struct Triangle
{
	Float3 a_ = { 0.0f, 0.0f, 0.0f };
	Float3 b_ = { 0.0f, 0.0f, 0.0f };
	Float3 c_ = { 0.0f, 0.0f, 0.0f };
};