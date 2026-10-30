#pragma once

#include "ruxon/core/types.h"
#include "ruxon/scene/node.h"
#include "ruxon/math/math.h"
#include <string>
#include <vector>

namespace ruxon {
namespace editor {
namespace docks {

// Center dock: live game preview plus engine stats. The preview renders the
// actual scene graph directly — 2D nodes as transformed sprites, 3D nodes as
// positional wireframe cubes — with a controllable preview camera.
class ViewportDock {
public:
    void SetRoot(scene::Node* root) { m_Root = root; }
    // Clicking a preview node selects it here (shared with other docks).
    void SetSelectionTarget(scene::Node** selection) { m_Selection = selection; }

    void Show() { m_Visible = true; }
    void Hide() { m_Visible = false; }
    bool IsVisible() const { return m_Visible; }

    void Render() { if (m_Visible) RenderVisible(); }

private:
    scene::Node* FindNodeByName(scene::Node* node, const std::string& name) const;
    void RenderVisible();
    u32 CountNodes(scene::Node* node) const;

    struct Sprite2D {
        math::vec2 position;
        f32 rotationDeg;
        math::vec2 scale;
        u32 color;
        std::string name;
    };

    struct Cube3D {
        math::vec3 position;
        u32 color;
        std::string name;
    };

    void CollectScene(scene::Node* node, std::vector<Sprite2D>& out2D, std::vector<Cube3D>& out3D) const;
    u32 ColorForName(const std::string& name) const;

    bool m_Visible = true;
    scene::Node* m_Root = nullptr;
    scene::Node** m_Selection = nullptr;

    // 0 = 2D preview, 1 = 3D preview
    int m_Mode = 0;

    // 2D preview camera: world point shown at the panel center, plus zoom.
    math::vec2 m_Cam2DPos{0.0f, 0.0f};
    f32 m_Cam2DZoom = 1.0f;

    // 3D preview camera: orbit around the origin.
    f32 m_YawDeg = 45.0f;
    f32 m_PitchDeg = 30.0f;
    f32 m_Distance = 10.0f;
};

} // namespace docks
} // namespace editor
} // namespace ruxon
