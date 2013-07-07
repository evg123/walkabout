#ifndef COMMON_HPP
#define COMMON_HPP

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>

#define INPUT_DELAY ((int)(CLOCKS_PER_SEC * .10))

#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080

#define INPUT_COOLDOWN .2

#define ACT_NONE 0
#define ACT_MOVE_UP 1
#define ACT_MOVE_DOWN 2
#define ACT_MOVE_LEFT 3
#define ACT_MOVE_RIGHT 4
#define ACT_SWING 5
#define ACT_FIRE_PROJ 6
#define ACT_WAIT 7
#define ACT_REPULSE_UP 8
#define ACT_REPULSE_DOWN 9
#define ACT_REPULSE_LEFT 10
#define ACT_REPULSE_RIGHT 11

#define REPULSE_DIR_MOD 7 // add this to a direction to get its repulse equivalent

// interchangeable with movement actions
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4

#define ATK_TYPE_MIN 1
#define ATK_TYPE_SHOT 1
#define ATK_TYPE_LINE 2
#define ATK_TYPE_CONE 3
#define ATK_TYPE_STAB 4
#define ATK_TYPE_MAX 4

struct VERT
{
    GLfloat x, y, z; // vertex
    GLfloat tu, tv; // texture coords
};

#define SURFACE_VERT_COUNT 4
#define SURFACE_VBO_COUNT 44
#define SURFACE_EBO_COUNT 6
#define SURFACE_VBO_POS         0
#define SURFACE_EBO_POS         0

#define TYPE_WALL 0
#define TYPE_MARKED_SQUARE 1
#define TYPE_DOOR 2

namespace texture {
    extern GLint player_up;
    extern GLint player_down;
    extern GLint player_left;
    extern GLint player_right;
    extern GLint floor;
    extern GLint wall;
    extern GLint door;
    extern GLint projectile;
    extern GLint enemy_1_up;
    extern GLint enemy_1_down;
    extern GLint enemy_1_left;
    extern GLint enemy_1_right;
}

#endif

