#include "ruxon/core/log.h"
#include "ruxon/core/application.h"
#include "ruxon/ruxon.h"

class RuxonPlayer : public ruxon::core::Application {
public:
    RuxonPlayer() : Application("Ruxon Player") {
        ruxon::core::Log::Info("Ruxon Player initialized");
    }
};

ruxon::core::Application* ruxon::core::CreateApplication() {
    return new RuxonPlayer();
}
