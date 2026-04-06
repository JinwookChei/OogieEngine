#include "stdafx.h"
#include "EditorWidget.h"
#include "EditorWindow.h"
#include "EditorCore.h"
#include "SceneWindow.h"

SceneWindow::SceneWindow()
	: ViewportFocused(false)
	, ViewportHovered(false)
	, GuizmoType(ImGuizmo::TRANSLATE)
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

//void SceneWindow::OnRender()
//{
//	Begin();
//
//	Update();
//	Render();
//
//	End();
//}

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


	void* pSRV = GBoundCamera->GetFinalRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo);
	//void* pSRV = pEditorCore->pBoundCamera_->GetParticleRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo);
	//void* pSRV = pEditorCore->pBoundCamera_->GetGBufferRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Normal);
	//void* pSRV = pEditorCore->pBoundCamera_->GetFinalRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Normal);
	//void* pSRV = pEditorCore->pBoundCamera_->GetDebugRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo);
	ImGui::Image
	(
		(ImTextureID)pSRV,
		ImVec2{ ViewportSize.X, ViewportSize.Y },
		ImVec2{ 0, 0 }, ImVec2{ 1, 1 }
	);

	ImGuiIO& io = ImGui::GetIO();

	if (!io.WantTextInput)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_1))
		{
			GuizmoType = ImGuizmo::SCALE;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_2))
		{
			GuizmoType = ImGuizmo::ROTATE;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_3))
		{
			GuizmoType = ImGuizmo::TRANSLATE;
		}
	}
	//// To do : guizmo
	if (GPickedActor && GuizmoType != -1)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetGizmoSizeClipSpace(0.07f);
		ImGuizmo::SetRect
		(
			ViewportBounds[0].X,
			ViewportBounds[0].Y,
			ViewportBounds[1].X - ViewportBounds[0].X,
			ViewportBounds[1].Y - ViewportBounds[0].Y
		);

		const Float4x4& viewMatrix = GBoundCamera->GetViewMatrix();
		const Float4x4& projectionMatrix = GBoundCamera->GetProjectionMatrix();
		IEditorBindTransform& actorTransform = GPickedActor->GetTransformForEditor();
		const Float4x4& worldMatrix = actorTransform.GetMatrixForEditor();
		ImGuizmo::Manipulate
		(
			(const float*)&viewMatrix
			, (const float*)&projectionMatrix
			, (ImGuizmo::OPERATION)GuizmoType
			, ImGuizmo::LOCAL
			, (float*)&worldMatrix
			, nullptr
		);

		actorTransform.TransformUpdateForEditorTTT();
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void SceneWindow::CleanUp()
{

}
