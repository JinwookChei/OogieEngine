#pragma once

class Light
	: public Actor
{
public:
	Light();

	~Light() override;

	const Color& LightColor() const;

	const Color& AmbientColor() const;

protected:
	Color lightColor_;

	Color ambientColor_;
private:
	void CleanUp() override;
};