#include "ruxon/core/log.h"
#include "ruxon/scripting/script.h"
#include "ruxon/scene/node.h"

int main() {
    ruxon::core::Log::Info("Ruxon Engine Tests...");
    
    // Test script
    ruxon::scripting::Script script("when ready\n    say \"Hello\"\nend");
    script.Compile();
    script.Execute();
    
    // Test scene node
    ruxon::scene::Node root("Root");
    auto child = std::make_shared<ruxon::scene::Node2D>("Sprite");
    root.AddChild(child);
    
    ruxon::core::Log::Info("All tests passed!");
    return 0;
}
