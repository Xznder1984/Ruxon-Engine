#pragma once

#include "ruxon/core/types.h"
#include "ruxon/scene/node.h"
#include <string>
#include <functional>

namespace ruxon {
namespace editor {
namespace docks {

// Left dock: holographic scene tree. Operates on the real scene graph —
// adding nodes mutates the root; clicking selects for the Inspector.
class SceneTreeDock {
public:
    SceneTreeDock();

    void SetRoot(scene::Node* root) { m_Root = root; }
    // The dock writes selections through this pointer to the editor's slot.
    void SetSelectionTarget(scene::Node** selection) { m_Selection = selection; }

    void Show() { m_Visible = true; }
    void Hide() { m_Visible = false; }
    bool IsVisible() const { return m_Visible; }

    void Render() { if (m_Visible) RenderVisible(); }

private:
    void RenderVisible();
    void RenderChildren(scene::Node* node);

    bool m_Visible = true;
    scene::Node* m_Root = nullptr;
    scene::Node** m_Selection = nullptr;
    u32 m_Node2DCount = 0;
    u32 m_Node3DCount = 0;
};

} // namespace docks
} // namespace editor
} // namespace ruxon
