#pragma once


class Sphere
	: public Actor
{
public:
	Sphere();

	~Sphere() override;

	void BeginPlay() override;

	void Tick(double deltaTime) override;

	void Render() override;

protected:
	void CleanUp() override;

private:
};
