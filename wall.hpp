#ifndef WALL_HPP
#define WALL_HPP

#include <list>
#include "drawable.hpp"

using namespace std;

#define WALL_HEIGHT 1000;
#define WALL_WIDTH 1000;

class Wall : public Drawable
{
private:
	int type_;

public:
	Wall(int type);
	~Wall();
	void set_type(int type);
	int type() { return type_; };
	static list<Wall*> create_line(int startX, int startY, int endX, int endY, int type);
	static list<Wall*> add_wall(int startX, int startY, int endX, int endY);
	static list<Wall*> add_door(int startX, int startY, int endX, int endY);
};


#endif
