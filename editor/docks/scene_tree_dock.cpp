#include "docks/scene_tree_dock.h"
#include <imgui.h>

namespace ruxon {
namespace editor {
namespace docks {

SceneTreeDock::SceneTreeDock() = default;

void SceneTreeDock::RenderVisible() {
    if (!ImGui::Begin("Scene Tree")) {
        ImGui::End();
        return;
    }

    if (m_Root) {
        if (ImGui::Button("Add 2D Node")) {
            auto child = std::make_shared<scene::Node2D>(
                "Node2D" + std::to_string(++m_Node2DCount));
            m_Root->AddChild(child);
            if (m_Selection) *m_Selection = child.get();
        }
        ImGui::SameLine();
        if (ImGui::Button("Add 3D Node")) {
            auto child = std::make_shared<scene::Node3D>(
                "Node3D" + std::to_string(++m_Node3DCount));
            m_Root->AddChild(child);
            if (m_Selection) *m_Selection = child.get();
        }

        ImGui::Separator();
        ImGui::TextUnformatted(m_Root->GetName().c_str());
        RenderChildren(m_Root);
    } else {
        ImGui::TextDisabled("No scene loaded.");
    }

    ImGui::End();
}

void SceneTreeDock::RenderChildren(scene::Node* node) {
    const auto& children = node->GetChildren();
    for (const auto& child : children) {
        if (!child) continue;
        ImGui::PushID(static_cast<const void*>(child.get()));
        const bool selected = m_Selection && *m_Selection == child.get();
        const bool isLeaf = child->GetChildren().empty();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (isLeaf) flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        if (selected) flags |= ImGuiTreeNodeFlags_Selected;

        const bool opened = ImGui::TreeNodeEx(child->GetName().c_str(), flags);
        if (ImGui::IsItemClicked() && m_Selection) {
            *m_Selection = child.get();
        }
        if (opened && !isLeaf) {
            RenderChildren(child.get());
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

} // namespace docks
} // namespace editor
} // namespace ruxon
