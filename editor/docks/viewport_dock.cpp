#include "docks/viewport_dock.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <functional>
#include <optional>

namespace ruxon {
namespace editor {
namespace docks {

namespace {

// Stable, deterministic hue per node name so colors identify nodes reliably.
ImU32 ColorForNameImpl(const std::string& name) {
    const size_t h = std::hash<std::string>{}(name);
    const f32 hue = static_cast<f32>(h % 360);
    const f32 sat = 0.62f;
    const f32 val = 0.95f;
    const f32 c = val * sat;
    const f32 frac = std::fmod(hue / 60.0f, 2.0f);
    const f32 x = c * (1.0f - std::fabs(frac - 1.0f));
    const f32 m = val - c;
    f32 r = 0, g = 0, b = 0;
    if (hue < 60)       { r = c; g = x; }
    else if (hue < 120) { r = x; g = c; }
    else if (hue < 180) { g = c; b = x; }
    else if (hue < 240) { g = x; b = c; }
    else if (hue < 300) { r = x; b = c; }
    else                { r = c; b = x; }
    return IM_COL32(static_cast<int>((r + m) * 255),
                    static_cast<int>((g + m) * 255),
                    static_cast<int>((b + m) * 255), 255);
}

// 2D preview: +x right, +y up on screen. Node2D sprites are 100x100 world
// units scaled by the node's scale.
constexpr f32 kBaseHalf = 50.0f;

ImVec2 WorldToScreen2D(const ImVec2& center, const math::vec2& world,
                       const math::vec2& camPos, f32 zoom) {
    return ImVec2(center.x + (world.x - camPos.x) * zoom,
                  center.y - (world.y - camPos.y) * zoom);
}

math::vec2 ScreenToWorld2D(const ImVec2& center, const ImVec2& screen,
                           const math::vec2& camPos, f32 zoom) {
    return math::vec2(camPos.x + (screen.x - center.x) / zoom,
                      camPos.y - (screen.y - center.y) / zoom);
}

} // anonymous namespace

void ViewportDock::RenderVisible() {
    if (!ImGui::Begin("Game Preview")) {
        ImGui::End();
        return;
    }

    const ImGuiIO& io = ImGui::GetIO();

    // Live engine stats, real values only.
    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::SameLine();
    ImGui::Text("| Frame: %.2f ms", io.Framerate > 0.0f ? 1000.0f / io.Framerate : 0.0f);
    if (m_Root) {
        ImGui::SameLine();
        ImGui::Text("| Nodes: %u", CountNodes(m_Root));
    }

    // Preview mode.
    ImGui::RadioButton("2D", &m_Mode, 0);
    ImGui::SameLine();
    ImGui::RadioButton("3D", &m_Mode, 1);
    ImGui::SameLine();
    ImGui::TextDisabled(m_Mode == 0
        ? "Left-drag pan · Wheel zoom · Click a node to select"
        : "Left-drag orbit · Wheel dolly · Click a node to select");

    ImGui::Separator();

    const ImVec2 origin = ImGui::GetCursorScreenPos();
    const ImVec2 avail = ImGui::GetContentRegionAvail();
    if (avail.x < 1.0f || avail.y < 1.0f) {
        ImGui::End();
        return;
    }

    // Interaction surface: active for left/right drag, reports hover.
    ImGui::InvisibleButton("preview_surface", avail,
                           ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool hovered = ImGui::IsItemHovered();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const ImVec2 center(origin.x + avail.x * 0.5f, origin.y + avail.y * 0.5f);

    std::vector<Sprite2D> sprites;
    std::vector<Cube3D> cubes;
    if (m_Root) CollectScene(m_Root, sprites, cubes);

    dl->PushClipRect(origin, ImVec2(origin.x + avail.x, origin.y + avail.y), true);

    if (m_Mode == 0) {
        // ── 2D camera controls ──────────────────────────────────────────
        if (hovered && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            const ImVec2 d = io.MouseDelta;
            m_Cam2DPos.x -= d.x / m_Cam2DZoom;
            m_Cam2DPos.y += d.y / m_Cam2DZoom;
        }
        if (hovered && io.MouseWheel != 0.0f) {
            const math::vec2 anchor = ScreenToWorld2D(center, io.MousePos, m_Cam2DPos, m_Cam2DZoom);
            m_Cam2DZoom *= io.MouseWheel > 0.0f ? 1.1f : (1.0f / 1.1f);
            m_Cam2DZoom = std::clamp(m_Cam2DZoom, 0.05f, 500.0f);
            m_Cam2DPos.x = anchor.x - (io.MousePos.x - center.x) / m_Cam2DZoom;
            m_Cam2DPos.y = anchor.y - (center.y - io.MousePos.y) / m_Cam2DZoom;
        }

        // ── grid ────────────────────────────────────────────────────────
        f32 step = 1.0f;
        for (int i = 0; i < 14 && step * m_Cam2DZoom < 40.0f; ++i) step *= 2.0f;
        const math::vec2 worldMin = ScreenToWorld2D(center, origin, m_Cam2DPos, m_Cam2DZoom);
        const math::vec2 worldMax = ScreenToWorld2D(center,
            ImVec2(origin.x + avail.x, origin.y + avail.y), m_Cam2DPos, m_Cam2DZoom);

        for (f32 gx = std::floor(worldMin.x / step) * step; gx <= worldMax.x; gx += step) {
            const float sx = WorldToScreen2D(center, math::vec2(gx, 0), m_Cam2DPos, m_Cam2DZoom).x;
            dl->AddLine(ImVec2(sx, origin.y), ImVec2(sx, origin.y + avail.y),
                        IM_COL32(255, 255, 255, 22));
        }
        for (f32 gy = std::floor(worldMin.y / step) * step; gy <= worldMax.y; gy += step) {
            const float sy = WorldToScreen2D(center, math::vec2(0, gy), m_Cam2DPos, m_Cam2DZoom).y;
            dl->AddLine(ImVec2(origin.x, sy), ImVec2(origin.x + avail.x, sy),
                        IM_COL32(255, 255, 255, 22));
        }

        // World axes through the origin.
        const float axisY = WorldToScreen2D(center, math::vec2(0, 0), m_Cam2DPos, m_Cam2DZoom).y;
        const float axisX = WorldToScreen2D(center, math::vec2(0, 0), m_Cam2DPos, m_Cam2DZoom).x;
        dl->AddLine(ImVec2(origin.x, axisY), ImVec2(origin.x + avail.x, axisY), IM_COL32(240, 80, 80, 140));
        dl->AddLine(ImVec2(axisX, origin.y), ImVec2(axisX, origin.y + avail.y), IM_COL32(90, 220, 120, 140));

        // ── 2D nodes ────────────────────────────────────────────────────
        for (const auto& s : sprites) {
            const float rot = glm::radians(s.rotationDeg);
            const float cr = std::cos(rot), sr = std::sin(rot);
            const float halfW = kBaseHalf * std::max(0.01f, s.scale.x) * m_Cam2DZoom;
            const float halfH = kBaseHalf * std::max(0.01f, s.scale.y) * m_Cam2DZoom;
            const ImVec2 c = WorldToScreen2D(center, s.position, m_Cam2DPos, m_Cam2DZoom);

            // Screen-space y grows downward, so rotate with inverted angle.
            const ImVec2 pts[4] = {
                ImVec2(c.x + (-halfW * cr - halfH * sr), c.y + ( halfW * sr - halfH * cr)),
                ImVec2(c.x + ( halfW * cr - halfH * sr), c.y + (-halfW * sr - halfH * cr)),
                ImVec2(c.x + ( halfW * cr + halfH * sr), c.y + (-halfW * sr + halfH * cr)),
                ImVec2(c.x + (-halfW * cr + halfH * sr), c.y + ( halfW * sr + halfH * cr)),
            };
            const ImU32 col = s.color;
            dl->AddQuadFilled(pts[0], pts[1], pts[2], pts[3], (col & 0x00FFFFFFu) | (0xD0u << 24));
            dl->AddQuad(pts[0], pts[1], pts[2], pts[3], col, 1.5f);

            if (m_Cam2DZoom > 0.25f) {
                dl->AddText(ImVec2(c.x + 6.0f, c.y + 2.0f), IM_COL32(255, 255, 255, 210),
                            s.name.c_str());
            }

            // Click-to-select (hit test in sprite-local space).
            if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
                !ImGui::IsMouseDragging(ImGuiMouseButton_Left, 6.0f)) {
                const math::vec2 local = ScreenToWorld2D(center, io.MousePos, m_Cam2DPos, m_Cam2DZoom) - s.position;
                const ImVec2 worldHalf(halfW / m_Cam2DZoom, halfH / m_Cam2DZoom);
                // Undo rotation (CCW by -rot).
                const float lx =  local.x * cr + local.y * sr;
                const float ly = -local.x * sr + local.y * cr;
                if (std::fabs(lx) <= worldHalf.x && std::fabs(ly) <= worldHalf.y && m_Selection) {
                    if (scene::Node* found = FindNodeByName(m_Root, s.name)) *m_Selection = found;
                }
            }
        }
    } else {
        // ── 3D camera controls ──────────────────────────────────────────
        if (hovered && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            const ImVec2 d = io.MouseDelta;
            m_YawDeg += d.x * 0.4f;
            m_PitchDeg = std::clamp(m_PitchDeg - d.y * 0.4f, -89.0f, 89.0f);
        }
        if (hovered && io.MouseWheel != 0.0f) {
            m_Distance *= io.MouseWheel > 0.0f ? 0.9f : (1.0f / 0.9f);
            m_Distance = std::clamp(m_Distance, 0.5f, 500.0f);
        }

        // ── matrices ────────────────────────────────────────────────────
        const float aspect = avail.x / avail.y;
        const float pitch = glm::radians(m_PitchDeg);
        const float yaw = glm::radians(m_YawDeg);
        const math::vec3 eye(m_Distance * std::cos(pitch) * std::sin(yaw),
                             m_Distance * std::sin(pitch),
                             m_Distance * std::cos(pitch) * std::cos(yaw));
        const glm::mat4 view = glm::lookAt(eye, math::vec3(0.0f), math::vec3(0.0f, 1.0f, 0.0f));
        const glm::mat4 proj = glm::perspective(glm::radians(55.0f), aspect, 0.1f, 10000.0f);

        auto project = [&](const math::vec3& w) -> std::optional<ImVec2> {
            const glm::vec4 clip = proj * view * glm::vec4(w, 1.0f);
            if (clip.w <= 0.0001f) return std::nullopt;
            const glm::vec3 ndc = glm::vec3(clip) / clip.w;
            if (ndc.z > 1.0f || ndc.z < -1.0f) return std::nullopt;
            return ImVec2(origin.x + (ndc.x * 0.5f + 0.5f) * avail.x,
                          origin.y + (0.5f - ndc.y * 0.5f) * avail.y);
        };

        // ── ground grid (adaptive step, projected) ─────────────────────
        const f32 extents = 12.0f;
        for (int axis = 0; axis < 2; ++axis) {
            for (int i = -static_cast<int>(extents); i <= static_cast<int>(extents); ++i) {
                const f32 v = static_cast<f32>(i);
                std::optional<ImVec2> a, b;
                if (axis == 0) { a = project(math::vec3(v, 0, -extents)); b = project(math::vec3(v, 0, extents)); }
                else           { a = project(math::vec3(-extents, 0, v)); b = project(math::vec3(extents, 0, v)); }
                if (a && b) dl->AddLine(*a, *b, IM_COL32(255, 255, 255, 26));
            }
        }

        // World axes.
        if (auto a = project(math::vec3(0.0f, 0.0f, 0.0f)), b = project(math::vec3(2, 0, 0)); a && b)
            dl->AddLine(*a, *b, IM_COL32(240, 80, 80, 200), 2.0f);
        if (auto a = project(math::vec3(0.0f, 0.0f, 0.0f)), b = project(math::vec3(0, 2, 0)); a && b)
            dl->AddLine(*a, *b, IM_COL32(90, 220, 120, 200), 2.0f);
        if (auto a = project(math::vec3(0.0f, 0.0f, 0.0f)), b = project(math::vec3(0, 0, 2)); a && b)
            dl->AddLine(*a, *b, IM_COL32(80, 120, 240, 200), 2.0f);

        // ── 3D nodes as wireframe cubes ────────────────────────────────
        constexpr f32 kHalf = 0.5f;
        for (const auto& cu : cubes) {
            const math::vec3 corners[8] = {
                {cu.position.x - kHalf, cu.position.y - kHalf, cu.position.z - kHalf},
                {cu.position.x + kHalf, cu.position.y - kHalf, cu.position.z - kHalf},
                {cu.position.x + kHalf, cu.position.y - kHalf, cu.position.z + kHalf},
                {cu.position.x - kHalf, cu.position.y - kHalf, cu.position.z + kHalf},
                {cu.position.x - kHalf, cu.position.y + kHalf, cu.position.z - kHalf},
                {cu.position.x + kHalf, cu.position.y + kHalf, cu.position.z - kHalf},
                {cu.position.x + kHalf, cu.position.y + kHalf, cu.position.z + kHalf},
                {cu.position.x - kHalf, cu.position.y + kHalf, cu.position.z + kHalf},
            };
            std::optional<ImVec2> p[8];
            for (int i = 0; i < 8; ++i) p[i] = project(corners[i]);
            const int edges[12][2] = {{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}};
            for (const auto& e : edges) {
                if (p[e[0]] && p[e[1]]) dl->AddLine(*p[e[0]], *p[e[1]], cu.color, 1.4f);
            }
            if (auto c = project(cu.position)) {
                dl->AddText(ImVec2(c->x + 8.0f, c->y + 2.0f), IM_COL32(255, 255, 255, 210),
                            cu.name.c_str());
            }

            // Click-to-select: nearest projected center within 24 px.
            if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
                !ImGui::IsMouseDragging(ImGuiMouseButton_Left, 6.0f)) {
                if (auto c = project(cu.position)) {
                    const float dx = io.MousePos.x - c->x;
                    const float dy = io.MousePos.y - c->y;
                    if (dx * dx + dy * dy < 24.0f * 24.0f && m_Selection) {
                        if (scene::Node* found = FindNodeByName(m_Root, cu.name)) *m_Selection = found;
                    }
                }
            }
        }
    }

    dl->PopClipRect();

    ImGui::End();
}

u32 ViewportDock::CountNodes(scene::Node* node) const {
    u32 count = 1;
    for (const auto& child : node->GetChildren()) {
        if (child) count += CountNodes(child.get());
    }
    return count;
}

void ViewportDock::CollectScene(scene::Node* node,
                                std::vector<Sprite2D>& out2D,
                                std::vector<Cube3D>& out3D) const {
    for (const auto& child : node->GetChildren()) {
        if (!child) continue;
        if (auto* n2d = dynamic_cast<scene::Node2D*>(child.get())) {
            out2D.push_back({n2d->GetPosition(), n2d->GetRotation(), n2d->GetScale(),
                             ColorForName(child->GetName()), child->GetName()});
        } else if (auto* n3d = dynamic_cast<scene::Node3D*>(child.get())) {
            out3D.push_back({n3d->GetPosition(), ColorForName(child->GetName()),
                             child->GetName()});
        }
        CollectScene(child.get(), out2D, out3D);
    }
}

u32 ViewportDock::ColorForName(const std::string& name) const {
    return ColorForNameImpl(name);
}

scene::Node* ViewportDock::FindNodeByName(scene::Node* node, const std::string& name) const {
    if (!node) return nullptr;
    if (node->GetName() == name) return node;
    for (const auto& child : node->GetChildren()) {
        if (scene::Node* found = FindNodeByName(child.get(), name)) return found;
    }
    return nullptr;
}

} // namespace docks
} // namespace editor
} // namespace ruxon
