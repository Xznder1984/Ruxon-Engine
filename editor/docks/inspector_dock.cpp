#include "docks/inspector_dock.h"
#include <imgui.h>
#include <cstring>

namespace ruxon {
namespace editor {
namespace docks {

void InspectorDock::RenderVisible() {
    if (!ImGui::Begin("Inspector")) {
        ImGui::End();
        return;
    }

    scene::Node* node = m_Selection ? *m_Selection : nullptr;
    if (!node) {
        ImGui::TextDisabled("Select a node in the Scene Tree to edit it.");
        ImGui::End();
        return;
    }

    // Reload the name buffer when the selection changes.
    if (node != m_Editing) {
        m_Editing = node;
        std::strncpy(m_NameBuffer, node->GetName().c_str(), sizeof(m_NameBuffer) - 1);
        m_NameBuffer[sizeof(m_NameBuffer) - 1] = '\0';
    }

    if (ImGui::InputText("Name", m_NameBuffer, sizeof(m_NameBuffer))) {
        node->SetName(std::string(m_NameBuffer));
    }

    if (auto* n2d = dynamic_cast<scene::Node2D*>(node)) {
        ImGui::SeparatorText("Transform (2D)");
        math::vec2 pos = n2d->GetPosition();
        if (ImGui::DragFloat2("Position", &pos.x, 0.5f)) n2d->SetPosition(pos);

        f32 rot = n2d->GetRotation();
        if (ImGui::DragFloat("Rotation (deg)", &rot, 0.5f)) n2d->SetRotation(rot);

        math::vec2 scale = n2d->GetScale();
        if (ImGui::DragFloat2("Scale", &scale.x, 0.02f, 0.01f, 100.0f)) n2d->SetScale(scale);
    } else if (auto* n3d = dynamic_cast<scene::Node3D*>(node)) {
        ImGui::SeparatorText("Transform (3D)");
        math::vec3 pos = n3d->GetPosition();
        if (ImGui::DragFloat3("Position", &pos.x, 0.1f)) n3d->SetPosition(pos);
    } else {
        ImGui::SeparatorText("Node");
        ImGui::TextDisabled("This node type has no editable transform.");
    }

    ImGui::End();
}

} // namespace docks
} // namespace editor
} // namespace ruxon
