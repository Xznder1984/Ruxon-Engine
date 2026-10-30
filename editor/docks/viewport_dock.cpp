#include "docks/viewport_dock.h"
#include <imgui.h>

namespace ruxon {
namespace editor {
namespace docks {

void ViewportDock::RenderVisible() {
    if (!ImGui::Begin("Viewport")) {
        ImGui::End();
        return;
    }

    const ImGuiIO& io = ImGui::GetIO();

    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::SameLine();
    ImGui::Text("| Frame: %.2f ms", io.Framerate > 0.0f ? 1000.0f / io.Framerate : 0.0f);

    if (m_Root) {
        ImGui::SameLine();
        ImGui::Text("| Nodes: %u", CountNodes(m_Root));
    }

    const ImVec2 region = ImGui::GetContentRegionAvail();
    ImGui::Text("Viewport: %.0f x %.0f", region.x, region.y);
    ImGui::Separator();
    ImGui::TextWrapped("The scene renderer draws here. Note: the visible game "
                       "view is the engine's window clear color while the scene "
                       "renderer is being built.");

    ImGui::End();
}

u32 ViewportDock::CountNodes(scene::Node* node) const {
    u32 count = 1;
    for (const auto& child : node->GetChildren()) {
        if (child) count += CountNodes(child.get());
    }
    return count;
}

} // namespace docks
} // namespace editor
} // namespace ruxon
