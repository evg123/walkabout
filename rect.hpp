#ifndef RECT_HPP
#define RECT_HPP

#include "common.hpp"

class Rect
{

public:
	Rect(void);
	~Rect(void);
	double top, bot, left, right;
	bool collides_with(Rect other);
};

#endif
