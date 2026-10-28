#pragma once

#ifdef RUXON_PLATFORM_WINDOWS
extern ruxon::core::Application* ruxon::core::CreateApplication();
int main(int argc, char** argv) {
    auto app = ruxon::core::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
#else
extern ruxon::core::Application* ruxon::core::CreateApplication();
int main(int argc, char** argv) {
    auto app = ruxon::core::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
#endif
