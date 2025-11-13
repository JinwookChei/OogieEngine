#pragma once

struct BoundVolume_AABB
{
	float minX;
	float minY;
	float minZ;
};


class BoundVolume
{
public:
	BoundVolume();

	~BoundVolume();

	void UpdateBoundVolume(IMesh* pMesh);
	
private:
	void CleanUp();

	BoundVolume_AABB boundVolume_;
};
