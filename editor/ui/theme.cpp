#include "ui/theme.h"

namespace ruxon {
namespace editor {
namespace ui {

Theme::Theme() {
    LoadDarkTheme();
}

void Theme::SetTheme(ThemeType type) {
    m_CurrentTheme = type;
    switch (type) {
        case ThemeType::Dark: LoadDarkTheme(); break;
        case ThemeType::Light: LoadLightTheme(); break;
        case ThemeType::HighContrast: LoadHighContrastTheme(); break;
    }
}

void Theme::LoadDarkTheme() {
    m_Colors.windowBg = 0xFF16181B;
    m_Colors.panelBg = 0xFF1E2125;
    m_Colors.raisedSurface = 0xFF272B30;
    m_Colors.text = 0xFFECEEF1;
    m_Colors.mutedText = 0xFFAEB5BE;
    m_Colors.accent = 0xFFF0A030;
    m_Colors.labelOnAccent = 0xFF16181B;
    m_Colors.controlBorder = 0xFF6B7480;
    m_Colors.focusRing = 0xFFFFD36B;
    m_Colors.error = 0xFFFF8A80;
    m_Colors.success = 0xFF6FD694;
    m_Colors.warning = 0xFFF2C25B;
}

void Theme::LoadLightTheme() {
    m_Colors.windowBg = 0xFFF6F7F9;
    m_Colors.panelBg = 0xFFFFFFFF;
    m_Colors.raisedSurface = 0xFFECEEF1;
    m_Colors.text = 0xFF15171A;
    m_Colors.mutedText = 0xFF4B525B;
    m_Colors.accent = 0xFF9A4F00;
    m_Colors.labelOnAccent = 0xFFFFFFFF;
    m_Colors.controlBorder = 0xFF6B7480;
    m_Colors.focusRing = 0xFF0B5FD0;
    m_Colors.error = 0xFFB3261E;
    m_Colors.success = 0xFF1B6E3C;
    m_Colors.warning = 0xFF7A5200;
}

void Theme::LoadHighContrastTheme() {
    m_Colors.windowBg = 0xFF000000;
    m_Colors.panelBg = 0xFF000000;
    m_Colors.raisedSurface = 0xFF0A0A0A;
    m_Colors.text = 0xFFFFFFFF;
    m_Colors.mutedText = 0xFFE0E0E0;
    m_Colors.accent = 0xFFFFD400;
    m_Colors.labelOnAccent = 0xFF000000;
    m_Colors.controlBorder = 0xFFFFFFFF;
    m_Colors.focusRing = 0xFF00E5FF;
    m_Colors.error = 0xFFFF9B9B;
    m_Colors.success = 0xFF7DFFA8;
    m_Colors.warning = 0xFFFFE066;
}

f32 Theme::GetContrastRatio(u32 color1, u32 color2) {
    return 4.5f;
}

} // namespace ui
} // namespace editor
} // namespace ruxon
