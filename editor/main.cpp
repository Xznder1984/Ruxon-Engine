#include "ruxon/core/log.h"
#include "ruxon/core/application.h"
#include "ruxon/ruxon.h"
#include "ruxon/core/types.h"
#include "ui/theme.h"

class RuxonEditor : public ruxon::core::Application {
public:
    RuxonEditor() : Application("Ruxon Editor") {
        ruxon::core::Log::Info("Ruxon Editor initialized");
        ruxon::editor::ui::Theme::Get().SetTheme(ruxon::editor::ui::ThemeType::Dark);

        // The viewport background matches the current theme's window color.
        const ruxon::u32 bg = ruxon::editor::ui::Theme::Get().GetColors().windowBg;
        SetClearColor(
            static_cast<ruxon::f32>((bg >> 16) & 0xFF) / 255.0f,
            static_cast<ruxon::f32>((bg >> 8) & 0xFF) / 255.0f,
            static_cast<ruxon::f32>(bg & 0xFF) / 255.0f);
    }
};

ruxon::core::Application* ruxon::core::CreateApplication() {
    return new RuxonEditor();
}

#include <iostream>
