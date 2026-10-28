#pragma once

#include "ruxon/core/types.h"
#include <string>

namespace ruxon {
namespace editor {
namespace ui {

enum class ThemeType {
    Dark,
    Light,
    HighContrast
};

struct ThemeColors {
    u32 windowBg;
    u32 panelBg;
    u32 raisedSurface;
    u32 text;
    u32 mutedText;
    u32 accent;
    u32 labelOnAccent;
    u32 controlBorder;
    u32 focusRing;
    u32 error;
    u32 success;
    u32 warning;
};

class Theme {
public:
    static Theme& Get() {
        static Theme instance;
        return instance;
    }
    
    void SetTheme(ThemeType type);
    ThemeType GetCurrentTheme() const { return m_CurrentTheme; }
    const ThemeColors& GetColors() const { return m_Colors; }
    static f32 GetContrastRatio(u32 color1, u32 color2);
    
private:
    Theme();
    void LoadDarkTheme();
    void LoadLightTheme();
    void LoadHighContrastTheme();
    
    ThemeType m_CurrentTheme = ThemeType::Dark;
    ThemeColors m_Colors;
};

} // namespace ui
} // namespace editor
} // namespace ruxon
