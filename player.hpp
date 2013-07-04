#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "common.hpp"
#include "drawable.hpp"
#include "projectile.hpp"

#define PLAYER_HEIGHT 3000
#define PLAYER_WIDTH 2000


class Player : public Drawable
{
private:

public:
	Player();
	~Player();
	void handle_action(double elapsed);
	list<Projectile*> fire_projectile(int direction);
};

#endif
