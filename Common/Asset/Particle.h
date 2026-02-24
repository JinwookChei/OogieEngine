#pragma once

struct ComputeParticleData
{
	float deltaTime;
	uint32_t particleNum;
	float accTime;
	uint32_t particleType;
};

struct RenderParticleData
{
	Float4x4 world;
	Float4x4 viewProj;
	Float3 cameraRight;
	float startSize;
	Float3 cameraUp;
	float endSize;
	Float4 startColor;
	Float4 endColor;
};


struct ParticleTTTTT
{
	Float3 position;
	float age;
	Float3 velocity;
	float lifeTime;
};