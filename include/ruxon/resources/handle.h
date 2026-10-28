#pragma once

#include "ruxon/core/types.h"

namespace ruxon {
namespace resources {

template<typename T>
class Handle {
public:
    Handle() : m_id(0) {}
    Handle(u32 id) : m_id(id) {}
    
    bool IsValid() const { return m_id != 0; }
    u32 GetId() const { return m_id; }
    
    bool operator==(const Handle& other) const { return m_id == other.m_id; }
    bool operator!=(const Handle& other) const { return m_id != other.m_id; }
    
private:
    u32 m_id;
};

} // namespace resources
} // namespace ruxon
