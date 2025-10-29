#pragma once

class Light
	: public Actor
{
public:
	Light();

	~Light() override;

	const Color& DiffuseColor() const;

	const Color& SpecularColor() const;
	
	const Color& AmbientColor() const;
	
private:
	void CleanUp() override;

	Color diffuseColor_;

	Color specularColor_;

	Color ambientColor_;
};