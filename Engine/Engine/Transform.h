#pragma once

class Transform final 
	: public IEditorBindTransform 
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
public:
	// MeshLoad Delegate
	using TransformUpdateDelegate = std::function<void()>;
	void BindOnTransformUpdateLoaded(TransformUpdateDelegate callback);

private:
	void BroadcastOnTransformUpdate();
	std::vector<TransformUpdateDelegate> OnTransformUpdateLoaded_;
	// MeshLoad Delegate End	

private:
	void TransformUpdate();
	void TransformUpdateTTT();

	Float4 scale_;
	Float4 rotation_;
	Float4 quaternion_;
	Float4 translation_;
	
	Float4x4 scaleMatrix_;
	Float4x4 rotationMatrix_;
	Float4x4 translationMatrix_;
	Float4x4 worldMatrix_;

public:
	const Float4x4& __stdcall GetWorldMatrixForEditor() const override;
	Float4& __stdcall GetActorScaleForEditor() override;
	Float4& __stdcall GetActorRotationForEditor() override;
	Float4& __stdcall GetActorQuaternionForEditor() override;
	Float4& __stdcall GetActorPositionForEditor() override;

	void __stdcall SetActorScaleForEditor(const Float4& scale) override;
	void __stdcall SetActorRotationForEditor(const Float4& rot) override;
	void __stdcall SetActorPositionForEditor(const Float4& pos) override;

	void __stdcall TransformUpdateForEditor() override;
	void __stdcall TransformUpdateForEditorTTT() override;
};