#pragma once
#include <memory>
#include <optional>
#include <vector>
#include <span>

#include <glm/glm.hpp>

#include <shader.hpp>
#include <mesh.hpp>

namespace ui {
    struct Anchor {
        glm::vec4 points;

        constexpr Anchor(float left, float top, float right, float bottom) : points(left, top, right, bottom) {}

        glm::vec4 to_position(glm::vec2 position, glm::vec2 size);

        static constexpr Anchor full() { return Anchor(1.0f, 1.0f, 1.0f, 1.0f); }
        static constexpr Anchor centered() { return Anchor(0.5f, 0.5f, 0.5f, 0.5f); }
    };

    class Element {
    protected:
        bool dirty;
        std::optional<Anchor> anchor;
        glm::vec2 position;
        glm::vec2 size;
        std::vector<std::unique_ptr<Element>> children;
    public:
        Element(glm::vec2 position, glm::vec2 size);
        Element(Anchor anchor);

        virtual void update(glm::vec2 parent_position, glm::vec2 parent_size);

        virtual void add_child(std::unique_ptr<Element> element);

        void set_anchor(Anchor anchor);

        glm::vec2 get_position() const { return position; }

        glm::vec2 get_size() const { return size; }

        std::span<std::unique_ptr<Element>> get_children() { return children; }
    };

    class Container : public Element {
    protected:
        glm::vec2 growth_direction;
    public:
        Container(Anchor anchor);

        void add_child(std::unique_ptr<Element> element) override;
    };

    class Drawable : public Element {
    public:
        Drawable(glm::vec2 position, glm::vec2 size) : Element(position, size) {}

        Drawable(Anchor anchor) : Element(anchor) {}

        glm::mat4 get_model_matrix() const;
        
        virtual void draw(const glm::mat4& projection_matrix) = 0;
    };

    class ShaderRectangle : public Drawable {
    private:
        Mesh mesh;
    public:
        Shader shader;

        ShaderRectangle(glm::vec2 position, glm::vec2 size, Shader&& shader);
        
        ShaderRectangle(Anchor anchor, Shader&& shader);

        void draw(const glm::mat4& projection_matrix) override;
    };

    class Context {
    public:
        Context(glm::vec2 viewport_size);

        Element root;

        void update();
        void draw(const glm::mat4& projection_matrix);
    };
}
