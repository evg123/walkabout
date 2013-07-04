#ifndef WORLD_HPP
#define WORLD_HPP

class World
{
private:
	double width_, height_;
public:
	World(void);
	~World(void);
	double width() { return width_; }
	double height() { return height_; }
};

#endif
