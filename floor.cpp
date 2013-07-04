#include "floor.hpp"

namespace texture {
    GLint floor = 0;
}

Floor::Floor()
{
	height_ = FLOOR_HEIGHT;
	width_ = FLOOR_WIDTH;
    update_model_matrix();
    texture_up = texture_down = texture_left = texture_right = texture::floor;
}

Floor::~Floor()
{

}

void Floor::update_model_matrix() {
    model_matrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(gl_x(), gl_y(), -0.1f));
    model_matrix_ = glm::scale(model_matrix_, glm::vec3(gl_width(), gl_height(), 0.0f));
}

void Floor::set_size(int width, int height) {
    Drawable::set_size(width, height);
    update_model_matrix();
}

void Floor::set_world_pos(int posX, int posY) {
    Drawable::set_world_pos(posX, posY);
    update_model_matrix();
}
