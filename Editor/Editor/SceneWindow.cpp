#include "stdafx.h"
#include "EditorWidget.h"
#include "EditorWindow.h"
#include "EditorCore.h"
#include "SceneWindow.h"

SceneWindow::SceneWindow()
	: /*mEditorCameraObject(nullptr)
	, mEditorCamera(nullptr)*/
	 ViewportFocused(false)
	, ViewportHovered(false)
	, GuizmoType(-1)
	, ViewportBounds{}
	, ViewportSize{}
{
	SetName("Scene");
	SetSize(ImVec2(300, 600));
}

SceneWindow::~SceneWindow()
{
	CleanUp();
}

void SceneWindow::OnRender()
{
	Begin();

	Update();
	Render();

	End();
}

void SceneWindow::Begin()
{
	bool Active = (bool)GetState();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin(GetName().c_str(), &Active, GetFlag());
}

void SceneWindow::End()
{
	// imgui scene view viewport
	const auto viewportMinRegion = ImGui::GetWindowContentRegionMin(); // ¾ÀºäÀÇ ÃÖ¼Ò ÁÂÇ¥
	const auto viewportMaxRegion = ImGui::GetWindowContentRegionMax(); // ¾ÀºäÀÇ ÃÖ´ë ÁÂÇ¥
	const auto viewportOffset = ImGui::GetWindowPos(); // ¾ÀºäÀÇ À§Ä¡

	constexpr int letTop = 0;
	constexpr int rightBottom = 1;
	ViewportBounds[letTop] = Float2{ viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	ViewportBounds[rightBottom] = Float2{ viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


	// get the camera render target view
	// rendering framebuffer image to the sceneview
	//ya::graphics::RenderTarget* frameBuffer = mEditorCamera->GetRenderTarget();
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	Float2 ViewportSize = Float2{ viewportPanelSize.x, viewportPanelSize.y };
	//ya::graphics::Texture* texture = frameBuffer->GetAttachmentTexture(0);

	
	EditorCore* pEditorCore = (EditorCore*)Editor::GetEditor();
	void* pSRV = pEditorCore->pBoundCamera_->GetFinalRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo);
	//void* pSRV = pEditorCore->pBoundCamera_->GetParticleRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo);
	ImGui::Image
	(
		(ImTextureID)pSRV,
		ImVec2{ ViewportSize.X, ViewportSize.Y }, 
		ImVec2{ 0, 0 }, ImVec2{ 1, 1 }
	);

	//// To do : guizmo
	//ya::GameObject* selectedObject = ya::renderer::selectedObject;
	//if (selectedObject && GuizmoType != -1)
	//{
	//	ImGuizmo::SetOrthographic(false);
	//	ImGuizmo::SetDrawlist();
	//	ImGuizmo::SetGizmoSizeClipSpace(0.15f);
	//	ImGuizmo::SetRect(ViewportBounds[0].x, ViewportBounds[0].y
	//		, ViewportBounds[1].x - ViewportBounds[0].x, ViewportBounds[1].y - ViewportBounds[0].y);

	//	// To do : guizmo...
	//	// game view camera setting

	//	// Scene Camera
	//	const ya::math::Matrix& viewMatrix = mEditorCamera->GetViewMatrix();
	//	const ya::math::Matrix& projectionMatrix = mEditorCamera->GetProjectionMatrix();

	//	// Object Transform
	//	ya::Transform* transform = selectedObject->GetComponent<ya::Transform>();
	//	ya::math::Matrix worldMatrix = transform->GetWorldMatrix();

	//	// snapping
	//	bool snap = ya::Input::GetKey(ya::eKeyCode::Leftcontrol);
	//	float snapValue = 0.5f;

	//	// snap to 45 degrees for rotation
	//	if (GuizmoType == ImGuizmo::OPERATION::ROTATE)
	//		snapValue = 45.0f;

	//	float snapValues[3] = { snapValue, snapValue, snapValue };

	//	ImGuizmo::Manipulate(*viewMatrix.m, *projectionMatrix.m, static_cast<ImGuizmo::OPERATION>(GuizmoType)
	//		, ImGuizmo::WORLD, *worldMatrix.m, nullptr, snap ? snapValues : nullptr);

	//	if (ImGuizmo::IsUsing())
	//	{
	//		// Decompose matrix to translation, rotation and scale
	//		float translation[3];
	//		float rotation[3];
	//		float scale[3];
	//		ImGuizmo::DecomposeMatrixToComponents(*worldMatrix.m, translation, rotation, scale);

	//		// delta rotation from the current rotation
	//		ya::math::Vector3 deltaRotation = Vector3(rotation) - transform->GetRotation();
	//		deltaRotation = transform->GetRotation() + deltaRotation;

	//		// set the new transform
	//		transform->SetScale(Vector3(scale));
	//		transform->SetRotation(Vector3(deltaRotation));
	//		transform->SetPosition(Vector3(translation));
	//	}
	//}

	// repair the default render target
	//ya::graphics::GetDevice()->BindDefaultRenderTarget();

	ImGui::End();
	ImGui::PopStyleVar();
}

void SceneWindow::Update()
{
	for (EditorWidget* widget : widgets_)
	{
		widget->Update();
	}
}

void SceneWindow::Render()
{

	for (EditorWidget* editor : widgets_)
	{
		editor->Render();
	}
}

void SceneWindow::CleanUp()
{

}
