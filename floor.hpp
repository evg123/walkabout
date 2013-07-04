#ifndef FLOOR_HPP
#define FLOOR_HPP

#include <list>
#include "drawable.hpp"

using namespace std;

#define FLOOR_HEIGHT 1000;
#define FLOOR_WIDTH 1000;

class Floor : public Drawable
{
private:
    void update_model_matrix();

public:
	Floor();
	~Floor();
    void set_size(int width, int height);
    void set_world_pos(int posX, int posY);
};


#endif
