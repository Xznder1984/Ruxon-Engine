#pragma once

namespace ruxon {
namespace editor {
namespace docks {

class ViewportDock {
public:
    ViewportDock();
    void Render();
    
private:
    bool m_Visible = true;
};

} // namespace docks
} // namespace editor
} // namespace ruxon
