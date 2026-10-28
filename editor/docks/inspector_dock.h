#pragma once

#include "ruxon/core/types.h"
#include <string>

namespace ruxon {
namespace editor {
namespace docks {

class InspectorDock {
public:
    InspectorDock();
    void Render();
    
private:
    bool m_Visible = true;
};

} // namespace docks
} // namespace editor
} // namespace ruxon
