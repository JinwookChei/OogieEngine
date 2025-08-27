#pragma once


class Transform final
{
public:
	Transform();

	~Transform();

	void SetScale(const DirectX::XMFLOAT4& scale);

	void SetRotation(const DirectX::XMFLOAT4& rotation);

	void AddRotaionY(float addRotation);

	void SetPosition(const DirectX::XMFLOAT4& position);

	const DirectX::XMMATRIX GetWorldMatrix() const;

	const DirectX::XMMATRIX GetWorldMatrixTranspose() const;

	DirectX::XMVECTOR GetPosition() const;

	DirectX::XMVECTOR ForwardVector() const;

	DirectX::XMVECTOR UpVector() const;

	DirectX::XMVECTOR RightVector() const;

private:
	void TransformUpdate();

	DirectX::XMFLOAT4 scale_;
	DirectX::XMFLOAT4 rotation_;
	DirectX::XMFLOAT4 quaternion_;
	DirectX::XMFLOAT4 position_;

	DirectX::XMMATRIX scaleMatrix_;
	DirectX::XMMATRIX rotationMatrix_;
	DirectX::XMMATRIX positionMatrix_;
	DirectX::XMMATRIX worldMatrix_;
};
