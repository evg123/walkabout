#include "rect.hpp"

Rect::Rect(void)
{
}

bool Rect::collides_with(Rect other)
{
	if (left < other.right && 
		right > other.left &&
		top < other.bot && 
		bot > other.top)
	{
		return true;
	}

	return false;
}

Rect::~Rect(void)
{
}
