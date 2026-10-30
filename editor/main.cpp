#include "ruxon/core/log.h"
#include "ruxon/core/application.h"
#include "ruxon/ruxon.h"
#include "ruxon/core/types.h"
#include "ruxon/scene/node.h"
#include "ui/theme.h"
#include "docks/scene_tree_dock.h"
#include "docks/inspector_dock.h"
#include "docks/viewport_dock.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
// DockBuilder is an internal API on purpose: our default layout is built once.
#include <imgui_internal.h>

#include <SDL3/SDL.h>
#include <filesystem>
#include <cstdlib>
#include <string>

class RuxonEditor : public ruxon::core::Application {
public:
    RuxonEditor() : Application("Ruxon Editor") {
        ruxon::core::Log::Info("Ruxon Editor initialized");

        ruxon::editor::ui::Theme::Get().SetTheme(ruxon::editor::ui::ThemeType::Dark);
        ApplyClearColorFromTheme();

        // Real scene root; docks mutate it directly.
        m_SceneRoot = std::make_shared<ruxon::scene::Node>();
        m_SceneRoot->SetName("Root");

        InitImGui();
        ApplyImGuiTheme();

        m_SceneTreeDock.SetRoot(m_SceneRoot.get());
        m_SceneTreeDock.SetSelectionTarget(&m_SelectedNode);
        m_InspectorDock.SetSelectionTarget(&m_SelectedNode);
        m_ViewportDock.SetRoot(m_SceneRoot.get());
    }

    ~RuxonEditor() override {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

protected:
    void OnFrame() override {
        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        BuildInitialDockLayout();
        DrawMainMenuBar();

        // Dockspace backing the docks.
        const ImGuiID dockspaceID = ImGui::GetID("RuxonDockSpace");
        ImGui::DockSpaceOverViewport(dockspaceID, ImGui::GetMainViewport());

        m_SceneTreeDock.Render();
        m_InspectorDock.Render();
        m_ViewportDock.Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

private:
    void InitImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // accessibility
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Keep layout state inside the user's app-support dir, not the cwd (an
        // .app launched from Finder runs with "/" as cwd).
        if (const char* home = std::getenv("HOME")) {
            namespace fs = std::filesystem;
            const fs::path dir = fs::path(home) / "Library" / "Application Support" / "Ruxon";
            std::error_code ec;
            fs::create_directories(dir, ec);
            static std::string iniPath = (dir / "editor_layout.ini").string();
            io.IniFilename = iniPath.c_str();
        }

        m_SDLWindow = static_cast<SDL_Window*>(GetWindow().GetNativeWindow());
        GetWindow().SetEventCallback([](void* e) {
            if (e) ImGui_ImplSDL3_ProcessEvent(static_cast<SDL_Event*>(e));
        });

        ImGui_ImplSDL3_InitForOpenGL(m_SDLWindow, nullptr);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ApplyClearColorFromTheme() {
        const ruxon::editor::ui::ThemeColors& colors =
            ruxon::editor::ui::Theme::Get().GetColors();
        SetClearColor(
            static_cast<ruxon::f32>((colors.windowBg >> 16) & 0xFF) / 255.0f,
            static_cast<ruxon::f32>((colors.windowBg >> 8) & 0xFF) / 255.0f,
            static_cast<ruxon::f32>(colors.windowBg & 0xFF) / 255.0f);
    }

    void ApplyImGuiTheme() {
        const ruxon::editor::ui::ThemeColors& t =
            ruxon::editor::ui::Theme::Get().GetColors();

        const ImU32 windowBg = ToImU32(t.windowBg);
        const ImU32 panelBg = ToImU32(t.panelBg);
        const ImU32 raised = ToImU32(t.raisedSurface);
        const ImU32 text = ToImU32(t.text);
        const ImU32 muted = ToImU32(t.mutedText);
        const ImU32 accent = ToImU32(t.accent);
        const ImU32 border = ToImU32(t.controlBorder);

        ImGuiStyle& s = ImGui::GetStyle();
        s.WindowRounding = 4.0f;
        s.FrameRounding = 3.0f;
        s.GrabRounding = 3.0f;
        s.TabRounding = 4.0f;
        s.WindowPadding = ImVec2(8, 8);
        s.FramePadding = ImVec2(6, 4);
        s.ItemSpacing = ImVec2(8, 6);

        ImVec4* c = s.Colors;
        c[ImGuiCol_WindowBg] = U32ToVec4(windowBg);
        c[ImGuiCol_ChildBg] = U32ToVec4(panelBg);
        c[ImGuiCol_PopupBg] = U32ToVec4(raised);
        c[ImGuiCol_MenuBarBg] = U32ToVec4(panelBg);
        c[ImGuiCol_Text] = U32ToVec4(text);
        c[ImGuiCol_TextDisabled] = U32ToVec4(muted);
        c[ImGuiCol_Border] = U32ToVec4(border);
        c[ImGuiCol_FrameBg] = U32ToVec4(raised);
        c[ImGuiCol_FrameBgHovered] = U32ToVec4(border);
        c[ImGuiCol_FrameBgActive] = U32ToVec4(border);
        c[ImGuiCol_TitleBg] = U32ToVec4(panelBg);
        c[ImGuiCol_TitleBgActive] = U32ToVec4(raised);
        c[ImGuiCol_Tab] = U32ToVec4(panelBg);
        c[ImGuiCol_TabHovered] = U32ToVec4(accent);
        c[ImGuiCol_TabActive] = U32ToVec4(raised);
        c[ImGuiCol_TabSelected] = U32ToVec4(raised);
        c[ImGuiCol_TabUnfocused] = U32ToVec4(panelBg);
        c[ImGuiCol_TabUnfocusedActive] = U32ToVec4(raised);
        c[ImGuiCol_Header] = U32ToVec4(raised);
        c[ImGuiCol_HeaderHovered] = U32ToVec4(accent);
        c[ImGuiCol_HeaderActive] = U32ToVec4(accent);
        c[ImGuiCol_Button] = U32ToVec4(raised);
        c[ImGuiCol_ButtonHovered] = U32ToVec4(accent);
        c[ImGuiCol_ButtonActive] = U32ToVec4(border);
        c[ImGuiCol_CheckMark] = U32ToVec4(accent);
        c[ImGuiCol_SliderGrab] = U32ToVec4(accent);
        c[ImGuiCol_SliderGrabActive] = U32ToVec4(border);
        c[ImGuiCol_Separator] = U32ToVec4(border);
        c[ImGuiCol_DockingPreview] = U32ToVec4(accent);
        c[ImGuiCol_DockingEmptyBg] = U32ToVec4(windowBg);
        c[ImGuiCol_ResizeGrip] = U32ToVec4(border);
        c[ImGuiCol_ResizeGripHovered] = U32ToVec4(accent);
        c[ImGuiCol_ResizeGripActive] = U32ToVec4(border);
        c[ImGuiCol_ScrollbarBg] = U32ToVec4(panelBg);
        c[ImGuiCol_ScrollbarGrab] = U32ToVec4(raised);
        c[ImGuiCol_ScrollbarGrabHovered] = U32ToVec4(accent);
        c[ImGuiCol_ScrollbarGrabActive] = U32ToVec4(border);
        c[ImGuiCol_NavCursor] = U32ToVec4(accent);
    }

    static ImU32 ToImU32(ruxon::u32 argb) {
        // Engine colors are 0xAARRGGBB; ImU32 color is IM_COL32(r, g, b, a).
        const ruxon::u8 a = static_cast<ruxon::u8>((argb >> 24) & 0xFF);
        const ruxon::u8 r = static_cast<ruxon::u8>((argb >> 16) & 0xFF);
        const ruxon::u8 g = static_cast<ruxon::u8>((argb >> 8) & 0xFF);
        const ruxon::u8 b = static_cast<ruxon::u8>(argb & 0xFF);
        return IM_COL32(r, g, b, a);
    }

    static ImVec4 U32ToVec4(ImU32 c) {
        return ImVec4(
            static_cast<float>(((c >> IM_COL32_R_SHIFT) & 0xFF)) / 255.0f,
            static_cast<float>(((c >> IM_COL32_G_SHIFT) & 0xFF)) / 255.0f,
            static_cast<float>(((c >> IM_COL32_B_SHIFT) & 0xFF)) / 255.0f,
            static_cast<float>(((c >> IM_COL32_A_SHIFT) & 0xFF)) / 255.0f);
    }

    void DrawMainMenuBar() {
        if (!ImGui::BeginMainMenuBar()) return;

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", "Cmd+Q")) Stop();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Theme")) {
            using T = ruxon::editor::ui::ThemeType;
            if (ImGui::MenuItem("Dark")) SetTheme(T::Dark);
            if (ImGui::MenuItem("Light")) SetTheme(T::Light);
            if (ImGui::MenuItem("High Contrast")) SetTheme(T::HighContrast);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            bool st = m_SceneTreeDock.IsVisible();
            bool in = m_InspectorDock.IsVisible();
            bool vp = m_ViewportDock.IsVisible();
            if (ImGui::MenuItem("Scene Tree", nullptr, &st)) st ? m_SceneTreeDock.Show() : m_SceneTreeDock.Hide();
            if (ImGui::MenuItem("Inspector", nullptr, &in)) in ? m_InspectorDock.Show() : m_InspectorDock.Hide();
            if (ImGui::MenuItem("Viewport", nullptr, &vp)) vp ? m_ViewportDock.Show() : m_ViewportDock.Hide();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    void SetTheme(ruxon::editor::ui::ThemeType type) {
        ruxon::editor::ui::Theme::Get().SetTheme(type);
        ApplyClearColorFromTheme();
        ApplyImGuiTheme();
    }

    // Builds the default docking arrangement (Scene Tree | Viewport | Inspector)
    // exactly once, the first frame a dockspace exists.
    void BuildInitialDockLayout() {
        const ImGuiID dockspaceID = ImGui::GetID("RuxonDockSpace");
        ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspaceID);
        if (node != nullptr && node->IsSplitNode()) return;

        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->WorkSize);

        ImGuiID leftID = 0, rightID = 0, centerID = 0;
        ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.22f, &leftID, &centerID);
        ImGui::DockBuilderSplitNode(centerID, ImGuiDir_Right, 0.26f, &rightID, &centerID);

        ImGui::DockBuilderDockWindow("Scene Tree", leftID);
        ImGui::DockBuilderDockWindow("Game Preview", centerID);
        ImGui::DockBuilderDockWindow("Inspector", rightID);
        ImGui::DockBuilderFinish(dockspaceID);
        m_LayoutBuilt = true;
    }

    std::shared_ptr<ruxon::scene::Node> m_SceneRoot;
    ruxon::scene::Node* m_SelectedNode = nullptr;
    SDL_Window* m_SDLWindow = nullptr;

    ruxon::editor::docks::SceneTreeDock m_SceneTreeDock;
    ruxon::editor::docks::InspectorDock m_InspectorDock;
    ruxon::editor::docks::ViewportDock m_ViewportDock;

    bool m_LayoutBuilt = false;
};

ruxon::core::Application* ruxon::core::CreateApplication() {
    return new RuxonEditor();
}

#include <iostream>
