#pragma once

class Actor;
class ObjectPicker
{
public:
	ObjectPicker();
	~ObjectPicker();

	static ObjectPicker* GetInstance();;

	void Tick(double deltaTime);

	void RayCastFromScreen(const Float2& screenPos);

	Actor* GetPickedActor() const;

private:
	void CleanUp();

	Actor* pPickedActor_;

	Float2 pickedMousePos_;
};
