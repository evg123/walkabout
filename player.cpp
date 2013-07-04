#include "player.hpp"

namespace texture {
    GLint player_up = 0;
    GLint player_down = 0;
    GLint player_left = 0;
    GLint player_right = 0;
}

Player::Player()
	: Drawable()
{
	height_ = PLAYER_HEIGHT;
	width_ = PLAYER_WIDTH;
    texture_up = texture::player_up;
    texture_down = texture::player_down;
    texture_left = texture::player_left;
    texture_right = texture::player_right;
}

void Player::handle_action(double elapsed)
{
	// drop down to the more generic actions that Drawable can handle
	Drawable::handle_action(elapsed);
}

Player::~Player()
{
}

list<Projectile*> Player::fire_projectile(int direction)
{
    list<Projectile*> projs;
    
    if (attack_type_ == ATK_TYPE_SHOT || attack_type_ == ATK_TYPE_LINE) {
        Projectile *proj = new Projectile(this);
        proj->set_world_pos(hand_x(), hand_y());
        proj->set_facing_(direction);
        proj->set_action(direction, 100);
        
        set_action(ACT_FIRE_PROJ, .5); // this delay doesnt actually get enforced
        cooldown = .5;

        projs.push_back(proj);
        
    } else if (attack_type_ == ATK_TYPE_CONE) {
        int modRange = attack_range_ / 8;
		set_action(ACT_FIRE_PROJ, 1);
		cooldown = 1;
		switch (direction)
		{
		case DIR_UP:
			for (int i = 0; i < modRange; i+=1000) {
				int start = hand_x() - i + 1000;
				for (int j = 0; j < i*2-1000; j+=1000)
				{
					Projectile *proj = new Projectile(this);
					proj->set_world_pos(start+j, hand_y()-i);
					proj->set_facing_(direction);
					proj->set_action(ACT_WAIT, 1.5);
					proj->set_rot_speed(.008);
					projs.push_back(proj);
				}
			}
			return projs;
		case DIR_DOWN:
			for (int i = 0; i < modRange; i+=1000) {
				int start = hand_x() - i + 1000;
				for (int j = 0; j < i*2-1000; j+=1000)
				{
					Projectile *proj = new Projectile(this);
					proj->set_world_pos(start+j, hand_y()+i);
					proj->set_facing_(direction);
					proj->set_action(ACT_WAIT, 1.5);
					proj->set_rot_speed(.008);
					projs.push_back(proj);
				}
			}
			return projs;
		case DIR_LEFT:
			for (int i = 0; i < modRange; i+=1000) {
				int start = hand_y() - i + 1000;
				for (int j = 0; j < i*2-1000; j+=1000)
				{
					Projectile *proj = new Projectile(this);
					proj->set_world_pos(hand_x()-i, start+j);
					proj->set_facing_(direction);
					proj->set_action(ACT_WAIT, 1.5);
					proj->set_rot_speed(.008);
					projs.push_back(proj);
				}
			}
			return projs;
		case DIR_RIGHT:
			for (int i = 0; i < modRange; i+=1000) {
				int start = hand_y() - i + 1000;
				for (int j = 0; j < i*2-1000; j+=1000)
				{
					Projectile *proj = new Projectile(this);
					proj->set_world_pos(hand_x()+i, start+j);
					proj->set_facing_(direction);
					proj->set_action(ACT_WAIT, 1.5);
					proj->set_rot_speed(.008);
					projs.push_back(proj);
				}
			}
			
		}
	} else if (attack_type_ == ATK_TYPE_STAB) {
		Projectile *proj = new SwordProj(this);
		proj->set_world_pos(hand_x(), hand_y());
		proj->set_facing_(direction);
		proj->set_action(direction, .2);
		projs.push_back(proj);
        
		proj = new SwordProj(this);
		proj->set_world_pos(hand_x()-500, hand_y());
		proj->set_facing_(direction);
		proj->set_action(direction, .2);
		projs.push_back(proj);

		proj = new SwordProj(this);
		proj->set_world_pos(hand_x()-1000, hand_y());
		proj->set_facing_(direction);
		proj->set_action(direction, .2);
		projs.push_back(proj);
        
		proj = new SwordProj(this);
		proj->set_world_pos(hand_x()-1500, hand_y());
		proj->set_facing_(direction);
		proj->set_action(direction, .2);
		projs.push_back(proj);

		set_action(ACT_FIRE_PROJ, .25); // this delay doesnt actually get enforced
		cooldown = .25;
        
	}
    return projs;
}
