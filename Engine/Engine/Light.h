#pragma once

class Light
	: public Actor
{
public:
	Light();

	~Light() override;

	const DirectX::XMFLOAT4& LightColor() const;

	const DirectX::XMFLOAT4& AmbientColor() const;

protected:
	DirectX::XMFLOAT4 lightColor_;

	DirectX::XMFLOAT4 ambientColor_;
private:
	void CleanUp() override;
};