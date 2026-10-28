#include "ruxon/scene/node.h"

namespace ruxon {
namespace scene {

bool Node::IsInGroup(const std::string& group) const {
    for (const auto& g : m_Groups) {
        if (g == group) return true;
    }
    return false;
}

} // namespace scene
} // namespace ruxon
