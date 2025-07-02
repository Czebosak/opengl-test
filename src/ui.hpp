#pragma once
#include <memory>
#include <optional>
#include <vector>
#include <span>
#include <functional>

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

        virtual ~Element() = default;

        virtual void update(glm::vec2 parent_position, glm::vec2 parent_size);

        virtual void add_child(std::unique_ptr<Element> element);

        void set_anchor(Anchor anchor);

        glm::vec2 get_position() const { return position; }

        glm::vec2 get_size() const { return size; }

        std::vector<std::unique_ptr<Element>>& get_children() { return children; }
    };

    class Container : virtual public Element {
    protected:
        glm::vec2 growth_direction;
    public:
        Container(Anchor anchor);

        void add_child(std::unique_ptr<Element> element) override;
    };

    class Drawable : virtual public Element {
    public:
        float scale;

        Drawable(glm::vec2 position, glm::vec2 size, float scale = 1.0f) : Element(position, size), scale(scale) {}

        Drawable(Anchor anchor, float scale = 1.0f) : Element(anchor), scale(scale) {}

        glm::mat4 get_model_matrix() const;
        
        virtual void draw(const glm::mat4& projection_matrix) = 0;
    };

    enum MouseButton {
        LEFT = 0,
        RIGHT = 1
    };

    enum MouseButtonAction {
        RELEASED = 0,
        PRESSED = 1
    };

    class Interactable : virtual public Element {
    public:
        Interactable(glm::vec2 position, glm::vec2 size) : Element(position, size) {}

        Interactable(Anchor anchor) : Element(anchor) {}

        std::function<void(Element*, MouseButton, MouseButtonAction)> on_mouse_action_callback;
        std::function<void(Element*)> on_hover_callback;
        std::function<void(Element*)> stop_hover_callback;
    };

    class ShaderRectangle : public Drawable {
    private:
        Mesh mesh;
    public:
        Shader shader;

        ShaderRectangle(glm::vec2 position, glm::vec2 size, Shader&& shader, float scale = 1.0f);
        
        ShaderRectangle(Anchor anchor, Shader&& shader, float scale = 1.0f);

        void draw(const glm::mat4& projection_matrix) override;
    };

    class Button : public Interactable {
    public:
        Button(glm::vec2 position, glm::vec2 size, std::function<void(Element*, MouseButton, MouseButtonAction)> on_mouse_action_callback) : Element(position, size), Interactable(position, size) {
            this->on_mouse_action_callback = on_mouse_action_callback;
        }

        Button(Anchor anchor, std::function<void(Element*, MouseButton, MouseButtonAction)> on_mouse_action_callback) : Element(anchor), Interactable(anchor) {
            this->on_mouse_action_callback = on_mouse_action_callback;
        }
    };

    class ShaderButton : public Button, public ShaderRectangle {
    public:
        ShaderButton(glm::vec2 position, glm::vec2 size, Shader&& shader, std::function<void(Element*, MouseButton, MouseButtonAction)> on_mouse_action_callback, float scale = 1.0f)
        : Element(position, size),
        Button(position, size, on_mouse_action_callback),
        ShaderRectangle(position, size, std::move(shader), scale) {}

        ShaderButton(Anchor anchor, Shader&& shader, std::function<void(Element*, MouseButton, MouseButtonAction)> on_mouse_action_callback, float scale = 1.0f)
        : Element(anchor),
        Button(anchor, on_mouse_action_callback),
        ShaderRectangle(anchor, std::move(shader), scale) {}
    };

    class Context {
    private:
        void draw_element(Element* element, const glm::mat4& projection_matrix);

        void handle_element_mouse_button_input(Element *element, float x, float y, MouseButton mouse_button, MouseButtonAction mouse_button_action);
    public:
        Context(glm::vec2 viewport_size);

        Element root;

        void update();
        void draw(const glm::mat4& projection_matrix);

        void handle_mouse_button_input(float x, float y, MouseButton mouse_button, MouseButtonAction mouse_button_action);
        void handle_mouse_movement(float x, float y);
    };
}
