#pragma once

#include "ruxon/core/types.h"
#include <string>

namespace ruxon {
namespace editor {
namespace docks {

class SceneTreeDock {
public:
    SceneTreeDock();
    void Render();
    void Show() { m_Visible = true; }
    void Hide() { m_Visible = false; }
    bool IsVisible() const { return m_Visible; }
    
private:
    bool m_Visible = true;
    std::string m_SelectedNode;
};

} // namespace docks
} // namespace editor
} // namespace ruxon
