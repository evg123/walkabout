#include "projectile.hpp"

namespace texture {
    GLint projectile = 0;
}

Projectile::Projectile(Drawable *shooter)
{
	this->shooter = shooter;
	height_ = PROJECTILE_HEIGHT;
	width_ = PROJECTILE_WIDTH;
	speed_ = 14000;
    texture_up = texture_down = texture_left = texture_right = texture::projectile;
    
}

Projectile::~Projectile()
{

}

// specialized projectile classes

SwordProj::SwordProj(Drawable *shooter)
    : Projectile(shooter)
{

}
