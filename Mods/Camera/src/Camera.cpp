#include <IconsMaterialDesign.h>

#include "imgui.h"

#include "Glacier/Camera/ZHM5MainCamera.h"
#include "Glacier/ZLevelManager.h"
#include "Glacier/Render/ZRenderPostfilterControllerEntity.h"
#include "Glacier/Render/ZRenderPostfilterParametersEntity.h"
#include "Glacier/Render/SRenderPostfilterParametersColorCorrection.h"

#include "Camera.h"
#include "Hooks.h"
#include <imgui_internal.h>

void Camera::OnDrawMenu()
{
    if (ImGui::Button(ICON_MD_CAMERA " Camera"))
    {
        isOpen = !isOpen;
    }
}

void Camera::OnDrawUI(const bool hasFocus)
{
    if (!hasFocus || !isOpen)
    {
        return;
    }

    ImGui::PushFont(SDK::GetInstance().GetBoldFont());
    ImGui::SetNextWindowSize(ImVec2(1250, 850), ImGuiCond_FirstUseEver);

    const bool isWindowVisible = ImGui::Begin(ICON_MD_CAMERA " Camera", &isOpen, ImGuiWindowFlags_NoScrollbar);

    ImGui::PushFont(SDK::GetInstance().GetRegularFont());

    if (isWindowVisible)
    {
        if (ImGui::Checkbox("LUT And Vignette Effects", &areLUTAndVignetteEffectsEnabled))
        {
            ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

            if (hitman)
            {
                ZRenderPostfilterControllerEntity* renderPostfilterControllerEntity = static_cast<ZRenderPostfilterControllerEntity*>(hitman->GetMainCamera()->GetRenderPostfilterControllerEntity().GetRawPointer());
                ZEntityRef currentPostfilterParametersEntity = renderPostfilterControllerEntity->GetCurrentParametersEntity().GetEntityRef();

                currentPostfilterParametersEntity.SetProperty("m_bColorCorrectionEnabled", areLUTAndVignetteEffectsEnabled);
                currentPostfilterParametersEntity.SetProperty("m_bDepthRemapEnabled", areLUTAndVignetteEffectsEnabled);
                currentPostfilterParametersEntity.SetProperty("m_bVignetteEnabled", areLUTAndVignetteEffectsEnabled);
            }
        }
    }

    ImGui::PopFont();
    ImGui::End();
    ImGui::PopFont();
}

DEFINE_MOD(Camera);
