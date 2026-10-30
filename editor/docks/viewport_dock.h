#pragma once

#include "ruxon/core/types.h"
#include "ruxon/scene/node.h"

namespace ruxon {
namespace editor {
namespace docks {

// Center dock: live engine stats and the render target. Real data only —
// frame timing from the UI library, node count from the actual scene graph.
class ViewportDock {
public:
    void SetRoot(scene::Node* root) { m_Root = root; }

    void Show() { m_Visible = true; }
    void Hide() { m_Visible = false; }
    bool IsVisible() const { return m_Visible; }

    void Render() { if (m_Visible) RenderVisible(); }

private:
    void RenderVisible();
    u32 CountNodes(scene::Node* node) const;

    bool m_Visible = true;

    scene::Node* m_Root = nullptr;
};

} // namespace docks
} // namespace editor
} // namespace ruxon
