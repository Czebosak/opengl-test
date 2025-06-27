#include "ui.hpp"

#include <glm/ext/matrix_transform.hpp>

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

    void Container::add_child(std::unique_ptr<Element> element) {
        children.emplace_back(std::move(element));
        dirty = true;
    }

    glm::mat4 Drawable::get_model_matrix() const {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        return model;
    }

    ShaderRectangle::ShaderRectangle(glm::vec2 position, glm::vec2 size, Shader&& shader)
    : Drawable(position, size), mesh(Mesh::rectangle(glm::vec2(1.0f))), shader(std::move(shader)) {}

    ShaderRectangle::ShaderRectangle(Anchor anchor, Shader&& shader)
    : Drawable(anchor), mesh(Mesh::rectangle(glm::vec2(1.0f))), shader(std::move(shader)) {}

    void ShaderRectangle::draw(const glm::mat4& projection_matrix) {
        shader.bind();
        shader.set_mvp(projection_matrix * get_model_matrix());
        printf("%f, %f | %f, %f\n", position.x, position.y, size.x, size.y);
        shader.set_uniform_v2("scale", size.x, size.y);
        mesh.bind();
        mesh.draw();
    }

    Context::Context(glm::vec2 viewport_size) : root(glm::vec2(0.0f), viewport_size) {}

    void Context::update() {
        root.update(glm::vec2(0.0f), glm::vec2(0.0f));
    }

    void Context::draw(const glm::mat4& projection_matrix) {
        for (const std::unique_ptr<Element>& child : root.get_children()) {
            if (Drawable* drawable = dynamic_cast<Drawable*>(child.get())) {
                drawable->draw(projection_matrix);
            }
        }
    }
}
