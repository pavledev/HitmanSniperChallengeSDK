#include <imgui_internal.h>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>

#include <IconsMaterialDesign.h>

#include <Glacier/Actor/ZActorManager.h>
#include <Glacier/Player/ZHitman5.h>
#include <Glacier/ZLevelManager.h>

#include <Actors.h>
#include <Utility/ResourceUtility.h>
#include <Utility/MemoryUtility.h>
#include <Hooks.h>

Actors::Actors()
{
    isOpen = false;
    selectedActorIndex = -1;
    renderActorNames = false;
}

void Actors::OnDrawMenu()
{
    if (ImGui::Button(ICON_MD_MAN " Actors"))
    {
        isOpen = !isOpen;
    }
}

void Actors::OnDrawUI(const bool hasFocus)
{
    if (!hasFocus || !isOpen)
    {
        return;
    }

    ImGui::PushFont(SDK::GetInstance().GetBoldFont());
    ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);

    const bool isWindowVisible = ImGui::Begin(ICON_MD_TOKEN " Actors", &isOpen, ImGuiWindowFlags_NoScrollbar);

    ImGui::PushFont(SDK::GetInstance().GetRegularFont());

    if (isWindowVisible)
    {
        ImGui::BeginChild("left pane", ImVec2(600, 500), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::Checkbox("Render Actor Names", &renderActorNames);

        static char actorName[256]{ "" };

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Actor Name");
        ImGui::SameLine();
        ImGui::InputText("##ActorName", actorName, sizeof(actorName));

        TArrayRef<TEntityRef<ZActor>> actors = ActorManager->GetAliveActors();

        for (size_t i = 0; i < actors.Size(); ++i)
        {
            ZActor* actor = actors[i].GetRawPointer();
            const ZString& actorName2 = actor->GetActorName();

            if (!strstr(actorName2.ToCString(), actorName))
            {
                continue;
            }

            bool isActorSelected = selectedActorIndex == i;

            if (ImGui::Selectable(actorName2.ToCString(), &isActorSelected))
            {
                selectedActorIndex = i;
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("ItemView", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

        if (selectedActorIndex != -1)
        {
            ZActor* actor = actors[selectedActorIndex].GetRawPointer();

            if (ImGui::Button("Teleport actor to hitman"))
            {
                ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

                if (hitman)
                {
                    ZSpatialEntity* hitmanSpatialEntity = hitman->GetSpatialEntity().GetRawPointer();
                    ZSpatialEntity* actorSpatialEntity = actor->GetSpatialEntity().GetRawPointer();

                    actorSpatialEntity->SetWorldPosition(hitmanSpatialEntity->GetWorldPosition());
                }
            }

            if (ImGui::Button("Teleport hitman to actor"))
            {
                ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

                if (hitman)
                {
                    ZSpatialEntity* hitmanSpatialEntity = hitman->GetSpatialEntity().GetRawPointer();
                    ZSpatialEntity* actorSpatialEntity = actor->GetSpatialEntity().GetRawPointer();

                    hitmanSpatialEntity->SetWorldPosition(actorSpatialEntity->GetWorldPosition());
                }
            }

            if (ImGui::Button("Kill Actor"))
            {
                if (actor)
                {
                    actor->GetID().SignalInputPin("KillActor");
                }
            }
        }

        ImGui::EndChild();

        if (ImGui::Button("Teleport all actors to hitman"))
        {
            for (unsigned int i = 0; i < actors.Size(); i++)
            {
                ZHitman5* hitman = LevelManager->GetHitman().GetRawPointer();

                if (hitman)
                {
                    ZSpatialEntity* hitmanSpatialEntity = hitman->GetSpatialEntity().GetRawPointer();
                    ZSpatialEntity* actorSpatialEntity = actors[i]->GetSpatialEntity().GetRawPointer();

                    hitmanSpatialEntity->SetWorldPosition(actorSpatialEntity->GetWorldPosition());
                }
            }
        }

        if (ImGui::Button("Kill all actors"))
        {
            for (unsigned int i = 0; i < actors.Size(); i++)
            {
                actors[i].GetEntityRef().SignalInputPin("KillActor");
            }
        }
    }

    ImGui::PopFont();
    ImGui::End();
    ImGui::PopFont();
}

void Actors::OnDraw3D()
{
    SDK& sdk = SDK::GetInstance();

    if (renderActorNames)
    {
        TArrayRef<TEntityRef<ZActor>> actors = ActorManager->GetAliveActors();

        for (size_t i = 0; i < actors.Size(); ++i)
        {
            ZActor* actor = actors[i].GetRawPointer();
            float4 worldPosition = actor->GetSpatialEntityPtr()->GetWorldPosition();
            SVector2 screenPosition;

            if (sdk.GetDirectXRenderer()->WorldToScreen(SVector3(worldPosition.x, worldPosition.y, worldPosition.z + 2.05f), screenPosition))
            {
                sdk.GetDirectXRenderer()->DrawText2D(actor->GetActorName(), screenPosition, SVector4(1.f, 0.f, 0.f, 1.f), 0.f, 0.5f);
            }
        }
    }
}

DEFINE_MOD(Actors);
