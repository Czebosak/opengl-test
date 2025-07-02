#include "ui.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace ui {
    glm::vec4 Anchor::to_position(glm::vec2 position, glm::vec2 size) {
        return glm::vec4(
            position + (size * glm::vec2(points.x, points.y)),
            position + (size * (1.0f - glm::vec2(points.z + points.x, points.w + points.y)))
        );
    }

    Element::Element(glm::vec2 position, glm::vec2 size) : dirty(false), anchor(std::nullopt), position(position), size(size) {}

    Element::Element(Anchor anchor) : dirty(true), anchor(std::make_optional(anchor)) {}

    void Element::update(glm::vec2 parent_position, glm::vec2 parent_size) {
        if (dirty && anchor.has_value()) {
            glm::vec4 new_data = (*anchor).to_position(parent_position, parent_size);
            position.x = new_data.x;
            position.y = new_data.y;
            size.x = new_data.z;
            size.y = new_data.w;

            dirty = false;
        }

        for (const std::unique_ptr<Element>& child : children) {
            (*child).update(position, size);

            if (dirty) {
                (*child).dirty = true;
            }
        }
    }

    void Element::set_anchor(Anchor anchor) {
        this->anchor = anchor;
        dirty = true;
    }

    void Element::add_child(std::unique_ptr<Element> element) {
        children.emplace_back(std::move(element));
    }

    Container::Container(Anchor anchor) : Element(anchor) {}

    void Container::add_child(std::unique_ptr<Element> element) {
        children.emplace_back(std::move(element));
        dirty = true;
    }

    glm::mat4 Drawable::get_model_matrix() const {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        return model;
    }

    ShaderRectangle::ShaderRectangle(glm::vec2 position, glm::vec2 size, Shader&& shader, float scale)
    : Element(position, size), Drawable(position, size, scale), mesh(Mesh::rectangle(glm::vec2(1.0f))), shader(std::move(shader)) {}

    ShaderRectangle::ShaderRectangle(Anchor anchor, Shader&& shader, float scale)
    : Element(anchor), Drawable(anchor, scale), mesh(Mesh::rectangle(glm::vec2(1.0f))), shader(std::move(shader)) {}

    void ShaderRectangle::draw(const glm::mat4& projection_matrix) {
        shader.bind();
        shader.set_mvp(projection_matrix * get_model_matrix());
        shader.set_uniform_v2("scale", size.x * scale, size.y * scale);
        mesh.bind();
        mesh.draw();
    }

    void Context::draw_element(Element* element, const glm::mat4& projection_matrix) {
        for (const std::unique_ptr<Element>& child : element->get_children()) {
            if (Drawable* drawable = dynamic_cast<Drawable*>(child.get())) {
                drawable->draw(projection_matrix);
            }

            draw_element(child.get(), projection_matrix);
        }
    }

    void Context::handle_element_mouse_button_input(Element *element, float x, float y, MouseButton mouse_button, MouseButtonAction mouse_button_action) {
        for (const std::unique_ptr<Element>& child : element->get_children()) {
            //printf("scp 2 ");
            if (Interactable* interactable = dynamic_cast<Interactable*>(child.get())) {
                glm::vec2 pos = interactable->get_position();
                glm::vec2 size = interactable->get_size();

                //std::cout << pos.x << ", " << pos.y << "   " << size.x << ", " << size.y << "    " << x << ", " << y << std::endl;


                //std::cout << (pos.x <= x && x <= pos.x + size.x) << ", " << (pos.y <= y && y <= pos.y + size.y) << std::endl;

                // std::cout << pos.y << " <= " << y << " && " << y << " <= " << pos.y + size.y << std::endl;

                // std::cout << (pos.y <= y && y <= pos.y + size.y) << std::endl;

                if (
                    (pos.x <= x && x <= pos.x + size.x) &&
                    (pos.y <= y && y <= pos.y + size.y)
                ) {
                    if (interactable->on_mouse_action_callback) {
                        interactable->on_mouse_action_callback(interactable, mouse_button, mouse_button_action);
                    }
                }
            }

            handle_element_mouse_button_input(child.get(), x, y, mouse_button, mouse_button_action);
        }
    }

    Context::Context(glm::vec2 viewport_size) : root(glm::vec2(0.0f), viewport_size) {}

    void Context::update() {
        root.update(glm::vec2(0.0f), glm::vec2(0.0f));
    }

    void Context::draw(const glm::mat4& projection_matrix) {
        draw_element(&root, projection_matrix);
    }
    
    void Context::handle_mouse_button_input(float x, float y, MouseButton mouse_button, MouseButtonAction mouse_button_action) {
        handle_element_mouse_button_input(&root, x, y, mouse_button, mouse_button_action);
    }

    void Context::handle_mouse_movement(float x, float y) {}
}
