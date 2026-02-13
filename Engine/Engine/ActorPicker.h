#pragma once

class Actor;
class ActorPicker
{
public:
	ActorPicker();
	~ActorPicker();

	static ActorPicker* GetInstance();;

	void Tick(double deltaTime);

	void ScreenToWorldRay(Ray* pOutRay, const Float2& screenPos);

	bool TryPickObject(const Ray& ray);
	
	bool RaycastBroadPhase(float* pOutDistance, const Ray& ray, Actor* pActor);

	bool RaycastNarrowPhase(float* pOutDistance, const Ray& ray, Actor* pActor);

	Actor* GetPickedActor() const;

private:
	void CleanUp();

	Actor* pPickedActor_;

	float curPickedActorDiff_;

	Float2 pickedMousePos_;
};
