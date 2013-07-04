#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <list>
#include "drawable.hpp"

using namespace std;

#define PROJECTILE_HEIGHT 1000;
#define PROJECTILE_WIDTH 1000;

class Projectile : public Drawable
{
private:
	
public:
	Drawable *shooter;

	Projectile(Drawable *shooter);
	~Projectile();
	
};


// specialized projectile classes

class SwordProj : public Projectile
{
protected:
public:
    SwordProj(Drawable *shooter);
};

#endif
