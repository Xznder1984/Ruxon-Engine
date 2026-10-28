#include "ruxon/core/log.h"
#include "ruxon/core/application.h"
#include "ruxon/ruxon.h"
#include "ui/theme.h"

class RuxonEditor : public ruxon::core::Application {
public:
    RuxonEditor() : Application("Ruxon Editor") {
        ruxon::core::Log::Info("Ruxon Editor initialized");
        ruxon::editor::ui::Theme::Get().SetTheme(ruxon::editor::ui::ThemeType::Dark);
    }
};

ruxon::core::Application* ruxon::core::CreateApplication() {
    return new RuxonEditor();
}
