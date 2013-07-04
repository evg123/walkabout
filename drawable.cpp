#include "drawable.hpp"
#include "rect.hpp"
#include "walkabout.hpp"

Drawable* Drawable::player = NULL;
Walkabout* Drawable::walk = NULL;

Drawable::Drawable()
{
	set_world_pos(0, 0);
	action = ACT_NONE;
	act_duration_ = 0;
	speed_ = 6000;
    exp = 0;
	hp = 100;
	damage_ = 35;
	facing_ = DIR_DOWN;
	attack_type_ = ATK_TYPE_MIN;
	attack_range_ = 60000;
	rotation = 0;
	rot_speed_ = 0;
	repulse_power_ = 12000;
	cooldown = 0;
    
    vbo_pos_ = (void*)(SURFACE_VBO_POS*sizeof(GLfloat));
	vbo_count_ = SURFACE_VBO_COUNT;
    ebo_pos_ = (void*)(SURFACE_EBO_POS*sizeof(GLushort));
	ebo_count_ = SURFACE_EBO_COUNT;
}

GLfloat Drawable::verts_[] = {
	// position(3) normal(3), texture(2) color(3)
	-0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,    0.0f,   0.0f,  0.5f, 0.5f, 0.5f, 
	-0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,    0.0f, 200.0f,  0.5f, 0.5f, 0.5f, 
	 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  200.0f,   0.0f,  0.5f, 0.5f, 0.5f, 
	 0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  200.0f, 200.0f,  0.5f, 0.5f, 0.5f, 
};

GLushort Drawable::elems_[] = {
	 0,  1,  2, 
     3,  2,  1, 
};

void Drawable::update_model_matrix() {
    
    model_matrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(gl_x()+gl_width()/2, gl_y()-gl_height()/2, 0.0f));
    //model_matrix_ = glm::rotate(model_matrix_, radsToDegrees(-xFacing_+(3.0f/2.0f)*M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
    model_matrix_ = glm::scale(model_matrix_, glm::vec3(gl_width(), gl_height(), 0.0f));
}

Rect Drawable::bounds()
{
	Rect r;

	r.top = world_y_;
	r.bot = world_y() + height();
	r.left = world_x();
	r.right = world_x() + width();

	return r;
}

Drawable::~Drawable()
{
}

GLint Drawable::texture() 
{
	switch (facing_)
	{
	case DIR_UP:
		return texture_up;
	case DIR_DOWN:
		return texture_down;
	case DIR_LEFT:
		return texture_left;
	case DIR_RIGHT:
		return texture_right;
    default:
        // shouldnt happen
        return 0;
	}
}

void Drawable::move(int x, int y)
{
    if (walk->can_move(this, x, y)) {
        set_world_pos(world_x()+x, world_y()+y);
    }
}

void Drawable::set_world_pos(int posX, int posY)
{
	world_x_ = posX;
	world_y_ = posY;
    update_hand();
}

bool Drawable::can_act()
{
	return cooldown <= 0;
}

void Drawable::handle_action(double elapsed)
{
	cooldown -= elapsed;
	double modElapsed = reduce_duration(elapsed);

	rotation += rot_speed_*elapsed;
	//double dist = speed*modElapsed;

	switch (action)
	{
	case ACT_MOVE_UP:
        move(0, -(speed()*modElapsed));
		break;
	case ACT_MOVE_DOWN:
        move(0, (speed()*modElapsed));
		break;
	case ACT_MOVE_LEFT:
        move(-(speed()*modElapsed), 0);
		break;
	case ACT_MOVE_RIGHT:
        move((speed()*modElapsed), 0);
		break;
    case ACT_REPULSE_UP:
        move(0, -(REPULSE_SPEED*modElapsed));
        break;
    case ACT_REPULSE_DOWN:
        move(0, (REPULSE_SPEED*modElapsed));
        break;
    case ACT_REPULSE_LEFT:
        move(-(REPULSE_SPEED*modElapsed), 0);
        break;
    case ACT_REPULSE_RIGHT:
        move((REPULSE_SPEED*modElapsed), 0);
        break;
	};

	act_duration_ -= elapsed;
}

void Drawable::set_action(int action, double duration)
{
	this->action = action;
	act_duration_ = duration;
}

double Drawable::reduce_duration(double elapsed)
{
	double modElapsed;
	if (act_duration_ - elapsed > 0)	{
		act_duration_ -= elapsed;
		modElapsed = elapsed;
	}
	else {
		modElapsed = act_duration_;
		act_duration_ = 0;
		action = ACT_NONE;
	}
	return modElapsed;
}

void Drawable::hit_by(Drawable &drawable)
{
	hp -= drawable.damage();
	set_action(drawable.facing_+REPULSE_DIR_MOD, REPULSE_SPEED*repulse_power_);
}

int Drawable::calc_direction(double x, double y)
{
	double xOff, yOff;
	
	xOff = x / walk->window_width_ - .5;
	yOff = y / walk->window_height_ - .5;

	if (abs(xOff) > abs(yOff)) {
		// left or right
		if (xOff > 0) {
			return DIR_RIGHT;
		}
		return DIR_LEFT;
	} else {
		// up or down
		if (yOff < 0) {
			return DIR_UP;
		}
		return DIR_DOWN;
	}
}

void Drawable::cycle_attack()
{
	if (attack_type_ == ATK_TYPE_MAX) {
		attack_type_ = ATK_TYPE_MIN;
	} else {
		attack_type_++;
	}
}

void Drawable::set_facing_(int direction) {
	facing_ = direction;
	update_hand();
}

void Drawable::update_hand()
{
	switch (facing_)
	{
	case DIR_UP:
		hand_x_ = world_x() + 1000;
		hand_y_ = world_y();
		break;
	case DIR_DOWN:
		hand_x_ = world_x();
		hand_y_ = world_y() + height() - 1000;
		break;
	case DIR_LEFT:
		hand_x_ = world_x();
		hand_y_ = world_y() + 1000;
		break;
	case DIR_RIGHT:
		hand_x_ = world_x() + width() - 1000;
		hand_y_ = world_y() + 1000;
		break;
	};
}

