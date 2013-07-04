#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP
#include <list>
#include <time.h>
#include <cmath>

#include "common.hpp"

#define REPULSE_SPEED 12000

class Rect;
class Walkabout;

class Drawable
{
public:
    void* vbo_pos_;
	int vbo_count_;
    void* ebo_pos_;
	int ebo_count_;
    double cooldown;
    static GLfloat verts_[SURFACE_VBO_COUNT];
    static GLushort elems_[SURFACE_EBO_COUNT];
    glm::mat4 model_matrix_;
    
protected:
	static Drawable *player;
	static Walkabout *walk;
	int height_;
	int width_;
	int world_x_, world_y_;
	double hand_x_, hand_y_;
	int action;
	double act_duration_;
	double speed_;
	double hp;
	double damage_;
    int exp;
	int facing_;
	int attack_type_;
	double attack_range_;
	double rotation;
	double rot_speed_;
	double repulse_power_;
    float scale_;
    GLint texture_up;
    GLint texture_down;
    GLint texture_left;
    GLint texture_right;

public:
	Drawable();
	~Drawable();
    inline int world_x() { return world_x_; };
	inline int world_y() { return world_y_;};
    inline float gl_x() { return world_x_ / 1000.0; };
	inline float gl_y() { return -world_y_ / 1000.0; };
    inline int width() { return width_; };
	inline int height() { return height_; };
    inline int gl_width() { return width_ / 1000.0; };
	inline int gl_height() { return height_ / 1000.0; };
	inline double hand_x() {return hand_x_; }
	inline double hand_y() { return hand_y_; }
	virtual void set_world_pos(int posX, int posY);
	void move(int x, int y);
	Rect bounds();
	double damage() { return damage_; };
	double getHp() { return hp; };
	void set_rot_speed(double spd) { rot_speed_ = spd; }
    virtual inline void set_size(int width, int height) { width_ = width; height_ = height; };
	inline double speed() { return speed_; };
	inline bool has_action() { return action != ACT_NONE; };
	virtual void handle_action(double elapsed);
	void set_action(int action, double duration);
	void hit_by(Drawable &drawable);
	void set_facing_(int direction);
	int facing() { return facing_; };
	int calc_direction(double x, double y);
	static void set_player(Drawable *player) { Drawable::player = player; }
	static void set_walk(Walkabout *walkabout) { Drawable::walk = walkabout; }
	void cycle_attack();
	int attack_type() { return attack_type_; };
	int attack_range() { return attack_range_; };
	double repulse_power() { return repulse_power_; }
    void update_model_matrix();
    bool can_act();
    GLint texture();

protected:
    double reduce_duration(double elapsed);
	void update_hand();
};

#endif
