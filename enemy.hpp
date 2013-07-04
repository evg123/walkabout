#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "common.hpp"
#include "drawable.hpp"

#define ENEMY_HEIGHT 2000;
#define ENEMY_WIDTH 2000;

class Enemy : public Drawable
{
private:
	
public:
	Enemy(void);
	~Enemy(void);
	void move_towards(int posX, int posY);
	void handle_action(double elapsed);
};

#endif
