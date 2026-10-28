#pragma once

#include <functional>
#include <vector>

namespace ruxon {
namespace core {

template<typename... Args>
class Event {
public:
    using Handler = std::function<void(Args...)>;
    
    void Subscribe(Handler handler) {
        m_handlers.push_back(handler);
    }
    
    void Emit(Args... args) {
        for (auto& handler : m_handlers) {
            handler(args...);
        }
    }
    
private:
    std::vector<Handler> m_handlers;
};

} // namespace core
} // namespace ruxon
