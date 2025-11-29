#pragma once

class Transform final 
	: public IImGuiBindTransform 
{
public:
	Transform();

	~Transform();

	ENGINE_API void SetScale(const Float4& scale);

	ENGINE_API void SetRotation(const Float4& rotation);

	ENGINE_API void AddRotaion(const Float4& offset);

	ENGINE_API void AddRotaionX(float offset);

	ENGINE_API void AddRotaionY(float offset);

	ENGINE_API void AddRotaionZ(float offset);

	ENGINE_API void SetPosition(const Float4& position);

	ENGINE_API void AddPosition(const Float4& offset);

	ENGINE_API void AddPositionX(float offset);

	ENGINE_API void AddPositionY(float offset);

	ENGINE_API void AddPositionZ(float offset);

	ENGINE_API const Float4x4& __stdcall GetWorldMatrix() const;

	ENGINE_API const Float4x4 __stdcall GetWorldMatrixTranspose() const;
	
	ENGINE_API const Float4& __stdcall GetScale() const;

	ENGINE_API const Float4& __stdcall GetRotation() const;

	ENGINE_API const Float4& __stdcall GetQuaternion() const;

	ENGINE_API const Float4& __stdcall GetPosition() const;

	ENGINE_API Float4 __stdcall ForwardVector() const;
	ENGINE_API Float4 __stdcall RightVector() const;
	ENGINE_API Float4 __stdcall UpVector() const;


	// MeshLoad Delegate
public:
	using TransformUpdateDelegate = std::function<void()>;
	void BindOnTransformUpdateLoaded(TransformUpdateDelegate callback);

private:
	void BroadcastOnTransformUpdate();
	std::vector<TransformUpdateDelegate> OnTransformUpdateLoaded_;
	// MeshLoad Delegate End	

private:
	void TransformUpdate();

	Float4 scale_;
	Float4 rotation_;
	Float4 quaternion_;
	Float4 position_;
	

	Float4x4 scaleMatrix_;
	Float4x4 rotationMatrix_;
	Float4x4 positionMatrix_;
	Float4x4 worldMatrix_;

public:
	Float4* __stdcall GetActorScaleForImGui() override;
	Float4* __stdcall GetActorRotationForImGui() override;
	Float4* __stdcall GetActorQuaternionForImGui() override;
	Float4* __stdcall GetActorPositionForImGui() override;

	void __stdcall TransformUpdateForImGui() override;
};