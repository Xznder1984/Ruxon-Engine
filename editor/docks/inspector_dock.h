#pragma once

#include "ruxon/core/types.h"
#include "ruxon/scene/node.h"
#include <string>

namespace ruxon {
namespace editor {
namespace docks {

// Right dock: edits the currently selected node's real properties —
// name and transform — through direct mutators on the scene node.
class InspectorDock {
public:
    void SetSelectionTarget(scene::Node** selection) { m_Selection = selection; }

    void Show() { m_Visible = true; }
    void Hide() { m_Visible = false; }
    bool IsVisible() const { return m_Visible; }

    void Render() { if (m_Visible) RenderVisible(); }

private:
    void RenderVisible();

    bool m_Visible = true;
    scene::Node** m_Selection = nullptr;
    scene::Node* m_Editing = nullptr;  // node whose name buffer is loaded
    char m_NameBuffer[128] = {};
};

} // namespace docks
} // namespace editor
} // namespace ruxon
