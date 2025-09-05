#include <IconsMaterialDesign.h>

#include "imgui.h"

#include "Global.h"
#include "HUD.h"

void HUD::OnDrawMenu()
{
    uiDisableHUD = *reinterpret_cast<int**>(BaseAddress + 0xD4AA58);
    isHUDVisible = *uiDisableHUD == 0;

    if (ImGui::Checkbox(ICON_MD_MONITOR " HUD", &isHUDVisible))
    {
        *uiDisableHUD = !*uiDisableHUD;
    }
}

DEFINE_MOD(HUD);
