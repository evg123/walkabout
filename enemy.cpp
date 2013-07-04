#include "enemy.hpp"
#include "walkabout.hpp"

namespace texture {
    GLint enemy_1_up = 0;
    GLint enemy_1_down = 0;
    GLint enemy_1_left = 0;
    GLint enemy_1_right = 0;
}

Enemy::Enemy()
{
	height_ = ENEMY_HEIGHT;
	width_ = ENEMY_WIDTH;
	hp = 200;
    texture_up = texture::enemy_1_up;
    texture_down = texture::enemy_1_down;
    texture_left = texture::enemy_1_left;
    texture_right = texture::enemy_1_right;
}

Enemy::~Enemy()
{
}

void Enemy::handle_action(double elapsed)
{
	Drawable::handle_action(elapsed);

	if (action == ACT_NONE) {
		if (walk->is_visible(world_x(), world_y(), player->world_x(), player->world_y())) {
			move_towards(player->world_x(), player->world_y());
		} else {
			// move around randomly
			int act = rand()%9+1;
			if (act > 4) {
				set_action(ACT_WAIT, .75);
			} else {
				set_action(act, width()/speed()*3);
			}
		}
	}
}

void Enemy::move_towards(int posX, int posY)
{
	int offX = world_x() - posX;
	int offY = world_y() - posY;

	int rX, rY;

	if (offX == 0) {
		rX = 0;
	} else {
		rX = rand()%abs(offX);
	}

	if (offY == 0) {
		rY = 0;
	} else {
		rY = rand()%abs(offY);
	}

	if (rX - rY > 0) {
		// move in the X direction
		if (offX > 0) {
			// TODO is this right?
			set_action(ACT_MOVE_LEFT, width()/speed());
		} else {
			set_action(ACT_MOVE_RIGHT, width()/speed());
		}
	} else {
		// Y direction 
		if (offY > 0) {
			// TODO is this right?
			set_action(ACT_MOVE_UP, width()/speed());
		} else {
			set_action(ACT_MOVE_DOWN, width()/speed());
		}
	}
}
