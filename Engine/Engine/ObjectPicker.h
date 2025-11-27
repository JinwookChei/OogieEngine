#pragma once

class Actor;
class ObjectPicker
{
public:
	ObjectPicker();
	~ObjectPicker();

	static ObjectPicker* GetInstance();;

	void Tick(double deltaTime);

	//void RayCastFromScreen(Float4* pOutPos, Float4* pOutDir, const Float2& screenPos);
	void RayCastFromScreen(const Float2& screenPos);

	Actor* GetPickedActor() const;

private:
	void CleanUp();

	Actor* pPickedActor_;

	float curPickedActorDiff_;

	Float2 pickedMousePos_;
};
