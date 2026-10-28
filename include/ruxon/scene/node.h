#pragma once

#include "ruxon/core/types.h"
#include "ruxon/math/math.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

namespace ruxon {
namespace scene {

class Node {
public:
    Node() = default;
    Node(const std::string& name) : m_Name(name) {}
    virtual ~Node() = default;
    
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }
    
    Node* GetParent() const { return m_Parent; }
    void SetParent(Node* parent) { m_Parent = parent; }
    
    void AddChild(std::shared_ptr<Node> child) {
        child->SetParent(this);
        m_Children.push_back(child);
    }
    
    void RemoveChild(Node* child) {
        m_Children.erase(
            std::remove_if(m_Children.begin(), m_Children.end(),
                [child](const std::shared_ptr<Node>& c) { return c.get() == child; }),
            m_Children.end()
        );
    }
    
    const std::vector<std::shared_ptr<Node>>& GetChildren() const { return m_Children; }
    
    virtual void Ready() {}
    virtual void Process(f64 delta) {}
    virtual void PhysicsProcess(f64 delta) {}
    
    template<typename T>
    T* GetNode(const std::string& path);
    
    void AddToGroup(const std::string& group) { m_Groups.push_back(group); }
    bool IsInGroup(const std::string& group) const;
    
private:
    std::string m_Name;
    Node* m_Parent = nullptr;
    std::vector<std::shared_ptr<Node>> m_Children;
    std::vector<std::string> m_Groups;
    std::unordered_map<std::string, std::string> m_Properties;
};

class Node2D : public Node {
public:
    Node2D() = default;
    Node2D(const std::string& name) : Node(name) {}
    
    math::vec2 GetPosition() const { return m_Position; }
    void SetPosition(const math::vec2& pos) { m_Position = pos; }
    
    f32 GetRotation() const { return m_Rotation; }
    void SetRotation(f32 rot) { m_Rotation = rot; }
    
    math::vec2 GetScale() const { return m_Scale; }
    void SetScale(const math::vec2& scale) { m_Scale = scale; }
    
protected:
    math::vec2 m_Position = {0.0f, 0.0f};
    f32 m_Rotation = 0.0f;
    math::vec2 m_Scale = {1.0f, 1.0f};
};

class Node3D : public Node {
public:
    Node3D() = default;
    Node3D(const std::string& name) : Node(name) {}
    
    math::vec3 GetPosition() const { return m_Position; }
    void SetPosition(const math::vec3& pos) { m_Position = pos; }
    
protected:
    math::vec3 m_Position = {0.0f, 0.0f, 0.0f};
};

} // namespace scene
} // namespace ruxon
