#pragma once

struct IEditorBindCamera
{
	virtual const Float4x4& __stdcall GetViewMatrix() const = 0;
	virtual const Float4x4& __stdcall GetProjectionMatrix() const = 0;
	virtual IRenderTarget* __stdcall GetGBufferRenderTargetForEditor() const = 0;
	//virtual IRenderTarget* __stdcall GetParticleRenderTargetForEditor() const = 0;
	//virtual IRenderTarget* __stdcall GetDebugRenderTargetForEditor() const = 0;
	virtual IRenderTarget* __stdcall GetFinalRenderTargetForEditor() const = 0;
};


struct IEditorBindTransform
{
	virtual const Float4x4& __stdcall GetMatrixForEditor() const = 0;
	virtual Float4& __stdcall GetActorScaleForEditor() = 0;
	virtual Float4& __stdcall GetActorRotationForEditor() = 0;
	virtual Float4& __stdcall GetActorQuaternionForEditor() = 0;	
	virtual Float4& __stdcall GetActorPositionForEditor() = 0;

	virtual void __stdcall SetActorScaleForEditor(const Float4& scale) = 0;
	virtual void __stdcall SetActorRotationForEditor(const Float4& rot) = 0;
	virtual void __stdcall SetActorPositionForEditor(const Float4& pos) = 0;

	virtual void __stdcall TransformUpdateForEditor() = 0;
	virtual void __stdcall TransformUpdateForEditorTTT() = 0;
};

struct IEditorBindPickedActor
{
	virtual IEditorBindTransform& __stdcall GetTransformForEditor() = 0;
	virtual E_ACTOR_TYPE __stdcall GetActorTypeForEditor() const = 0;

	// Light	
	// 0 -> DirectionLight
	// 1 -> SpotLight
	// 2 -> PointLight
	virtual int __stdcall GetLightTypeForEditor() const = 0;

	virtual Color __stdcall GetDiffuseColorForEditor() const  = 0;
	virtual Color __stdcall GetSpecularColorForEditor() const  = 0;
	virtual Color __stdcall GetAmbientColorForEditor() const  = 0;
	virtual void __stdcall SetDiffuseColorForEditor(const Color& color) = 0;
	virtual void __stdcall SetSpecularColorForEditor(const Color& color) = 0;
	virtual void __stdcall SetAmbientColorForEditor(const Color& color) = 0;

	virtual float __stdcall GetLightIntensity() const = 0;
	virtual float __stdcall GetLightRange() const = 0;
	virtual float __stdcall GetLightSmooth() const = 0;
	virtual float __stdcall GetLightInnerAngle() const = 0;
	virtual float __stdcall GetLightOuterAngle() const = 0;
	virtual void __stdcall SetLightIntensity(float intensity) = 0;
	virtual void __stdcall SetLightRange(float range) = 0;
	virtual void __stdcall SetLightSmooth(float smooth) = 0;
	virtual void __stdcall SetLightInnerAngle(float innerAngle) = 0;
	virtual void __stdcall SetLightOuterAngle(float outerAngle) = 0;

	virtual IMaterial* __stdcall GetMaterialForEditor() const = 0;
};