#pragma once

class Renderer;

class Actor
{
public:
	Actor();

	virtual ~Actor();

	Renderer* pRenderer_;
};
