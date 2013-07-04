#ifndef WALKABOUT_HPP
#define WALKABOUT_HPP

#include <time.h>
#include <list>

#include "common.hpp"
#include "player.hpp"
#include "world.hpp"
#include "enemy.hpp"
#include "wall.hpp"
#include "projectile.hpp"
#include "square.hpp"
#include "floor.hpp"

using namespace std;

class Drawable;

class Walkabout
{
private:
    glm::mat4 proj_matrix_;
	Player player_;
    Floor floor_;
	World world_;
	list<Wall*> walls_;
	list<Enemy*> enemies_;
	list<Projectile*> projectiles_;
	list<Square> marked_squares_;
    bool typing_mode_;
    
    GLuint vbo_;
    GLuint ebo_;
	GLuint vao_;
    
	GLuint shader_prog_;
    
    GLint position_attr_;
    GLint normal_attr_;
	GLint text_coord_attr_;
    GLint color_attr_;
    
	GLint proj_uni_;
	GLint view_uni_;
	GLint vert_model_uni_;
    GLint norm_model_uni_;
	GLint light_pos_cs_uni_;
    GLint light_count_uni_;
    GLint text_samp_uni_;
    
    GLint player_text_;
    GLint enemy_text_;
    GLint wall_text_;
    GLint floor_text_;
    GLint proj_text_;

    static void handle_key_input(int key, int action);
    static void handle_mouse_button_input(int button, int action);
    static void handle_window_resize(int width, int height);
    glm::mat4 get_view_mat_at_pos(double x_pox, double y_pos);
	void render_frame();
	void create_walls();
	void update_marked_squares(int dir);
	void generate_section();
    GLuint shader_from_file(std::string filename, GLenum type);

public:
    int window_width_;
    int window_height_;
    double input_cooldown_;
    
    Walkabout();
	~Walkabout();
    
    static Walkabout& getInstance() {
        static Walkabout walk;
        return walk;
    }
    static void init_textures();
    
    void init_gl();
    void init_vbo();
	void init_vao();
	void init_shaders();
	void init_attributes();
    
    void render();
    void update_world(double elapsed);
	void render_components();
	void handle_input(double elapsed);
	bool can_move(Drawable *obj, double offX, double offY);
	bool is_visible(int fromX, int fromY, int toX, int toY);
    std::vector<glm::vec3> light_positions_ws_;
    void add_light(float xPos, float yPos, float zPos);
};

#endif
