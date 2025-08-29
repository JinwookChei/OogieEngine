#pragma once


class Transform final
{
public:
	Transform();

	virtual ~Transform();

	ENGINE_API void SetScale(const DirectX::XMFLOAT4& scale);

	ENGINE_API void SetRotation(const DirectX::XMFLOAT4& rotation);

	ENGINE_API void SetPosition(const DirectX::XMFLOAT4& position);

	ENGINE_API void AddRotaionPitch(float pitch);

	ENGINE_API void AddRotaionYaw(float yaw);

	ENGINE_API void AddRotaionRoll(float roll);

	ENGINE_API void AddPositionX(float x);

	ENGINE_API void AddPositionY(float y);

	ENGINE_API void AddPositionZ(float z);



	ENGINE_API const DirectX::XMMATRIX GetWorldMatrix() const;

	ENGINE_API const DirectX::XMMATRIX GetWorldMatrixTranspose() const;

	ENGINE_API DirectX::XMVECTOR GetPosition() const;

	ENGINE_API DirectX::XMVECTOR ForwardVector() const;

	ENGINE_API DirectX::XMVECTOR RightVector() const;

	ENGINE_API DirectX::XMVECTOR UpVector() const;

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
