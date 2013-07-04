#include "wall.hpp"

namespace texture {
    GLint wall = 0;
    GLint door = 0;
}

Wall::Wall(int type)
{
	height_ = WALL_HEIGHT;
	width_ = WALL_WIDTH;
	set_type(type);
}

void Wall::set_type(int type) {
    this->type_ = type;
    switch (type_) {
    case TYPE_WALL:
        texture_up = texture_down = texture_left = texture_right = texture::wall;
        break;
    case TYPE_DOOR:
        texture_up = texture_down = texture_left = texture_right = texture::door;
        break;
    default:
        texture_up = texture_down = texture_left = texture_right = texture::wall;
        break;
    }
}

list<Wall*> Wall::create_line(int startX, int startY, int endX, int endY, int type)
{
	int start, end;
	list<Wall*> segments;

	if (startX == endX)
	{
		// vertical wall
		if (startY <= endY)
		{
			start = startY;
			end = endY;
		} 
		else
		{
			start = endY;
			end = startY;
		}

		for (int y = start; y <= end; y += 1000)
		{
			Wall *seg = new Wall(type);
			seg->world_x_ = startX;
			seg->world_y_ = y;
			segments.push_back(seg);
		}
	}
	else if (startY == endY)
	{
		// horizontal wall
		if (startX <= endX)
		{
			start = startX;
			end = endX;
		} 
		else
		{
			start = endX;
			end = startX;
		}

		for (int x = start; x <= end; x += 1000)
		{
			Wall *seg = new Wall(type);
			seg->world_x_ = x;
			seg->world_y_ = startY;
			segments.push_back(seg);
		}
	}
	
	return segments;
}

list<Wall*> Wall::add_door(int startX, int startY, int endX, int endY)
{
	return create_line(startX, startY, endX, endY, TYPE_DOOR);
}

list<Wall*> Wall::add_wall(int startX, int startY, int endX, int endY)
{
	return create_line(startX, startY, endX, endY, TYPE_WALL);
}

Wall::~Wall()
{

}
