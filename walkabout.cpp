
#include <time.h>
#include <typeinfo>

#include "walkabout.hpp"
#include "rect.hpp"
#include "gli/gli.hpp"
#include "gli/gtx/gl_texture2d.hpp"

Walkabout::Walkabout()
{
    window_width_ = DEFAULT_WINDOW_WIDTH;
    window_height_ = DEFAULT_WINDOW_HEIGHT;
	srand(time(NULL));
	typing_mode_ = false;
	input_cooldown_ = 0;

	create_walls();

	player_.set_world_pos(2000, 3000);
	Drawable::set_player(&player_); // yuck
	Drawable::set_walk(this); // very yuck

}

void Walkabout::init_gl() {
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Walkabout::init_vao() {
    
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
}

void Walkabout::init_vbo() {
    
    /* need this if we ever add new models
    for (int i = 0; i < SURFACE_EBO_COUNT; i++) {
        Surface::elems_[i] += WALL_SEGMENT_VERT_COUNT;
    }
    */
    
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    
    // total vbo size is size of all models added together
    int vbo_size = sizeof(Drawable::verts_);
    
    // allocate space for verticies
    glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, GL_STATIC_DRAW);
    
    // load each model's vertex data into the vbo
    glBufferSubData(GL_ARRAY_BUFFER, SURFACE_VBO_POS, sizeof(Drawable::verts_), Drawable::verts_);
    
    // init element array buffer
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    
    // total ebo size is size of all models added together
    int ebo_size = sizeof(Drawable::elems_);
    
    // allocate space for elements
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_size, NULL, GL_STATIC_DRAW);
    
    // load each model's element data into the ebo
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, SURFACE_EBO_POS, sizeof(Drawable::elems_), Drawable::elems_);
}

void Walkabout::init_shaders() {
    
    GLuint vertex_shader = shader_from_file("shaders/vertex.glsl", GL_VERTEX_SHADER);
    GLuint frag_shader = shader_from_file("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    if (vertex_shader == 0 || frag_shader == 0) {
        // TODO: thow something meaningful
        throw "shader error";
    }
    
    shader_prog_ = glCreateProgram();
    glAttachShader(shader_prog_, vertex_shader);
    glAttachShader(shader_prog_, frag_shader);
    glBindFragDataLocation(shader_prog_, 0, "out_color");
    glLinkProgram(shader_prog_);
    glUseProgram(shader_prog_);
    
}

GLuint Walkabout::shader_from_file(std::string filename, GLenum type) {
    std::ifstream in_stream(filename.c_str());
    std::string source_str((std::istreambuf_iterator<char>(in_stream)),
                           std::istreambuf_iterator<char>());
    const char *source_ca = source_str.c_str();
    
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source_ca, NULL);
    glCompileShader(shader);
    
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint max_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);
        std::vector<GLchar> info_log(max_len);
        glGetShaderInfoLog(shader, max_len, &max_len, &info_log[0]);
        
        std::cout << "Error compiling shader " << filename << std::endl;
        for (GLchar c : info_log) {
            std::cout << c;
        }
        std::cout << std::endl;
        return 0;
    }
    
    return shader;
}

void Walkabout::init_textures() {
    
    texture::player_up = gli::createTexture2D("textures/player_down.dds");
    texture::player_down = gli::createTexture2D("textures/test_texture.dds");
    texture::player_left = gli::createTexture2D("textures/test_texture.dds");
    texture::player_right = gli::createTexture2D("textures/test_texture.dds");
    texture::floor = gli::createTexture2D("textures/rough_stone_wall.dds");
    texture::wall = gli::createTexture2D("textures/stone_wall.dds");
    texture::door = gli::createTexture2D("textures/rough_stone_wall.dds");
    texture::projectile = gli::createTexture2D("textures/ice_wall.dds");
    texture::enemy_1_up = gli::createTexture2D("textures/rough_stone_wall.dds");
    texture::enemy_1_down = gli::createTexture2D("textures/rough_stone_wall.dds");
    texture::enemy_1_left = gli::createTexture2D("textures/rough_stone_wall.dds");
    texture::enemy_1_right = gli::createTexture2D("textures/rough_stone_wall.dds");
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
}

void Walkabout::init_attributes() {
    
    position_attr_ = glGetAttribLocation(shader_prog_, "position_ms");
    glVertexAttribPointer(position_attr_, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void*)(0*sizeof(GLfloat)));
    
    normal_attr_ = glGetAttribLocation(shader_prog_, "normal_ms");
    glVertexAttribPointer(normal_attr_, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    
    text_coord_attr_ = glGetAttribLocation(shader_prog_, "in_text");
    glVertexAttribPointer(text_coord_attr_, 2, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
    
    color_attr_ = glGetAttribLocation(shader_prog_, "in_color");
    glVertexAttribPointer(color_attr_, 3, GL_FLOAT, GL_FALSE, 11*sizeof(GLfloat), (void*)(8*sizeof(GLfloat)));
    
    proj_uni_ = glGetUniformLocation(shader_prog_, "proj");
    view_uni_ = glGetUniformLocation(shader_prog_, "view");
    vert_model_uni_ = glGetUniformLocation(shader_prog_, "vert_model");
    norm_model_uni_ = glGetUniformLocation(shader_prog_, "norm_model");
    light_pos_cs_uni_ = glGetUniformLocation(shader_prog_, "light_pos_cs");
    light_count_uni_ = glGetUniformLocation(shader_prog_, "light_count");
    text_samp_uni_ = glGetUniformLocation(shader_prog_, "texture_sampler");
    
    proj_matrix_ = glm::perspective(35.0f, window_width_ / float(window_height_), 0.1f, 200.0f);
    glUniformMatrix4fv(proj_uni_, 1, GL_FALSE, glm::value_ptr(proj_matrix_));
    
}

glm::mat4 Walkabout::get_view_mat_at_pos(double x_pos, double y_pos) {
	
    glm::vec3 position = glm::vec3(x_pos, y_pos, 80.0f);
    glm::vec3 target = glm::vec3(x_pos, y_pos, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    glm::mat4 view_mat = glm::lookAt(position, target, up);
    //view_mat = glm::inverse(view_mat);
    
    return view_mat;
}

void Walkabout::render() {
    
    glEnableVertexAttribArray(position_attr_);
    glEnableVertexAttribArray(normal_attr_);
    glEnableVertexAttribArray(text_coord_attr_);
    glEnableVertexAttribArray(color_attr_);
    
    glm::mat4 view_mat = get_view_mat_at_pos(player_.gl_x(), player_.gl_y());
    glUniformMatrix4fv(view_uni_, 1, GL_FALSE, glm::value_ptr(view_mat));
    
    int light_count = light_positions_ws_.size() + 1;
    glUniform1i(light_count_uni_, light_count);
    
    // convert each light position to camera space
    glm::vec3 light_pos_cs[16];
    for (int i = 1; i < light_count; i++) {
        glm::vec3 light_pos_ws = light_positions_ws_[i];
        light_pos_cs[i] = (glm::vec3)(view_mat * glm::vec4(light_pos_ws, 1.0));
    }
    // add a light above the player
    light_pos_cs[0] = (glm::vec3)(view_mat * glm::vec4(player_.gl_x(), player_.gl_y(), -8.0, 1.0));
    glUniform3fv(light_pos_cs_uni_, light_count, glm::value_ptr(light_pos_cs[0]));

    // draw floor
    glUniformMatrix4fv(vert_model_uni_, 1, GL_FALSE, glm::value_ptr(floor_.model_matrix_));
    glUniformMatrix4fv(norm_model_uni_, 1, GL_FALSE, glm::value_ptr(floor_.model_matrix_));
    glUniform1i(text_samp_uni_, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floor_.texture());
    glDrawElements(GL_TRIANGLES, floor_.ebo_count_, GL_UNSIGNED_SHORT, floor_.ebo_pos_);

    // draw player
    player_.update_model_matrix();
    glUniformMatrix4fv(vert_model_uni_, 1, GL_FALSE, glm::value_ptr(player_.model_matrix_));
    glUniformMatrix4fv(norm_model_uni_, 1, GL_FALSE, glm::value_ptr(player_.model_matrix_));
    glUniform1i(text_samp_uni_, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, player_.texture());
    glDrawElements(GL_TRIANGLES, player_.ebo_count_, GL_UNSIGNED_SHORT, player_.ebo_pos_);
    
    for (Enemy *en : enemies_) {
        en->update_model_matrix();
        glUniformMatrix4fv(vert_model_uni_, 1, GL_FALSE, glm::value_ptr(en->model_matrix_));
        glUniformMatrix4fv(norm_model_uni_, 1, GL_FALSE, glm::value_ptr(en->model_matrix_));
        glUniform1i(text_samp_uni_, 0);
        
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, en->texture());
        
        glDrawElements(GL_TRIANGLES, en->ebo_count_, GL_UNSIGNED_SHORT, en->ebo_pos_);
    }
    
    for (Wall *ws : walls_) {
        ws->update_model_matrix();
        glUniformMatrix4fv(vert_model_uni_, 1, GL_FALSE, glm::value_ptr(ws->model_matrix_));
        glUniformMatrix4fv(norm_model_uni_, 1, GL_FALSE, glm::value_ptr(ws->model_matrix_));
        glUniform1i(text_samp_uni_, 0);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ws->texture());
        
        glDrawElements(GL_TRIANGLES, ws->ebo_count_, GL_UNSIGNED_SHORT, ws->ebo_pos_);
    }
    
    for (Projectile *proj : projectiles_) {
        proj->update_model_matrix();
        glUniformMatrix4fv(vert_model_uni_, 1, GL_FALSE, glm::value_ptr(proj->model_matrix_));
        glUniformMatrix4fv(norm_model_uni_, 1, GL_FALSE, glm::value_ptr(proj->model_matrix_));
        glUniform1i(text_samp_uni_, 0);
        
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, proj->texture());
        
        glDrawElements(GL_TRIANGLES, proj->ebo_count_, GL_UNSIGNED_SHORT, proj->ebo_pos_);
    }
    
    glDisableVertexAttribArray(position_attr_);
    glDisableVertexAttribArray(normal_attr_);
    glDisableVertexAttribArray(text_coord_attr_);
    glDisableVertexAttribArray(color_attr_);
}

void Walkabout::add_light(float xPos, float yPos, float zPos) {
    
    // convert in game dimensions to gl dimensions
    glm::vec3 light = glm::vec3(xPos, yPos, zPos);
    light_positions_ws_.push_back(light);
}

void Walkabout::create_walls()
{
	generate_section();
}

inline int randRoomSide() { return 20000 + rand()%25000; }

void Walkabout::generate_section() 
{
	int sectionMaxX = rand()%250000+100000;
	int sectionMaxY = rand()%250000+100000;
	
    floor_.set_size(sectionMaxX, sectionMaxY);
    floor_.set_world_pos(sectionMaxX/2, sectionMaxY/2);
    //floor_.set_world_pos(sectionMaxX, sectionMaxY);
    //floor_.set_world_pos(0, 0);
    
	// TODO mark these as indesctructable
	walls_.splice(walls_.end(), Wall::add_wall(          0, 0,           sectionMaxX, 0          ));
	walls_.splice(walls_.end(), Wall::add_wall(          0, 0,           0,           sectionMaxY));
	walls_.splice(walls_.end(), Wall::add_wall(sectionMaxX, 0,           sectionMaxX, sectionMaxY));
	walls_.splice(walls_.end(), Wall::add_wall(          0, sectionMaxY, sectionMaxX, sectionMaxY));

	bool skipFirst = true;
	int lastHorizDiv = 0;
	bool doneDividing = false;
	while (!doneDividing) {
		// pick a new horizontal divider position
		int horizDivLen = randRoomSide();
		int horizDiv = lastHorizDiv + horizDivLen;
		if (horizDiv > sectionMaxY) {
			horizDiv = sectionMaxY;
			doneDividing = true;
		}
		
		//walk down the divider and place walls up to the last divider
		int vertDiv = 0;
		while (true) {
			int lastVertDiv = vertDiv;
			int vertDivLen = randRoomSide();
			vertDiv += vertDivLen;
			if (vertDiv > sectionMaxX) {
				break;
			}
			int doorPosHoriz = lastHorizDiv + horizDivLen/2 - 3000;
			walls_.splice(walls_.end(), Wall::add_wall(vertDiv,lastHorizDiv, vertDiv,doorPosHoriz));
			walls_.splice(walls_.end(), Wall::add_door(vertDiv,doorPosHoriz+1000, vertDiv,doorPosHoriz+6000));
			walls_.splice(walls_.end(), Wall::add_wall(vertDiv,doorPosHoriz+7000, vertDiv,horizDiv));

			// place this section of the horizontal divider
			int doorPosVert = lastVertDiv + horizDivLen/2 - 3000;
			walls_.splice(walls_.end(), Wall::add_wall(lastVertDiv,horizDiv, doorPosVert,horizDiv));
			walls_.splice(walls_.end(), Wall::add_door(doorPosVert+1000,horizDiv, doorPosVert+6000,horizDiv));
			walls_.splice(walls_.end(), Wall::add_wall(doorPosVert+7000,horizDiv, vertDiv,horizDiv));

			// place enemies within this room
			if (!skipFirst) {
				for (int numEnemies = rand()%3; numEnemies > 0; numEnemies--) {
					Enemy *obj = new Enemy();
					obj->set_world_pos(lastVertDiv+numEnemies*2000, lastHorizDiv+numEnemies*2000);
					enemies_.push_front(obj);
				}
			}
			skipFirst = false;
		}
		lastHorizDiv = horizDiv;
	}
}

Walkabout::~Walkabout()
{
	// TODO delete drawables in environment
}


int main() {
    if (!glfwInit()) {
        std::cout << "Error initializing GLFW\n";
        return -1;
    }
    
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    if (!glfwOpenWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 
                        8, 8, 8, 0, 24, 0, GLFW_WINDOW)) {
        std::cout << "Error opening GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwSetWindowTitle("Walkabout");
    //glfwDisable(GLFW_MOUSE_CURSOR);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Error initing GLEW\n";
        glfwTerminate();
        return -1;
    }
    Walkabout::init_textures();
    Walkabout &walk = Walkabout::getInstance();
    
    walk.init_gl();
    walk.init_vao();
    walk.init_vbo();
    walk.init_shaders();
    walk.init_attributes();
    
    
    //walk.add_light(5.0, 5.0, 0.0);
    
    double fps_timer = 0;
    //glfwSetKeyCallback(Walkabout::handleKeyInput);
    //glfwSetMouseButtonCallback(Walkabout::handleMouseButtonInput);
    //glfwSetWindowSizeCallback(Walkabout::handleWindowResize);
    double prev_time = glfwGetTime();
    while (glfwGetWindowParam(GLFW_OPENED)) {
        double cur_time = glfwGetTime();
        double time_delta = cur_time - prev_time;
        prev_time = cur_time;
        walk.input_cooldown_ -= time_delta;
        
        // print framerate
        fps_timer += time_delta;
        if (fps_timer > 1) {
            int fps = 1 / time_delta;
            std::cout << fps << "\r" << std::flush;
            fps_timer = 0;
        }
        // logic
        walk.handle_input(time_delta);
        walk.update_world(time_delta);
        
        //walk.cleanupObjects();
        
        // drawing
        glClearColor(0.8f, 0.8f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        walk.render();
        
        glfwSwapBuffers();
        
        int error = glGetError();
        if (error != 0) {
            std::cout << "GL Error " << error << std::endl;
            //return -1;
        }
    }
    
    glfwTerminate();
    return 0;
}

// not used
void Walkabout::handle_mouse_button_input(int button, int action) {
    
    //Walkabout &walk = Walkabout::getInstance();
    
    switch (button) {
        default:
            break;
    }
}

// not used
void Walkabout::handle_key_input(int key, int action) {
    
    //Walkabout &walk = Walkabout::getInstance();
    
    switch (key) {
        default:
            break;
    }
}

void Walkabout::handle_window_resize(int width, int height) {
    
    Walkabout &walk = Walkabout::getInstance();
    
    walk.window_width_ = width;
    walk.window_height_ = height;
}

bool Walkabout::can_move(Drawable *obj, double offX, double offY)
{
	Rect r = obj->bounds();
	r.top += offY;
	r.bot += offY;
	r.left += offX;
	r.right += offX;

	if (typeid(*obj) == typeid(Projectile)) {
		// projectiles can always move
		return true;
	}
	
	for (Wall *w : walls_)	{
		if (r.collides_with(w->bounds()) && w->type() == TYPE_WALL) {
			return false;
		}
	}
	for (Drawable *d : enemies_) {
		if (r.collides_with(d->bounds()) && d != obj)	{
			return false;
		}
	}

	return true;
}

void Walkabout::handle_input(double elapsed)
{
	bool inputTaken = false;

	// update based on mouse position
    int x_pos, y_pos;
	glfwGetMousePos(&x_pos, &y_pos);
	int mouseDir = player_.calc_direction(x_pos, y_pos);

	player_.set_facing_(mouseDir);
	update_marked_squares(mouseDir);

	if (typing_mode_)
	{
		// capture text and handle appropriately
	}
	
	// handle player actions
	if (player_.can_act())
	{
        // w or up
		if (glfwGetKey(87) || glfwGetKey(265)) {
            player_.move(0, -(player_.speed()*elapsed));
		}
        // s or down
		if (glfwGetKey(83) || glfwGetKey(264)) {
			player_.move(0, (player_.speed()*elapsed));
		}
        // a or left
		if (glfwGetKey(65) || glfwGetKey(263)) {
			player_.move(-(player_.speed()*elapsed), 0);
		}
        // d or right
		if (glfwGetKey(68) || glfwGetKey(262)) {
			player_.move((player_.speed()*elapsed), 0);
		}

        // space
		if (glfwGetKey(32)) {
			player_.set_action(ACT_WAIT, 100);
		}
        // e
		if (glfwGetKey(69)) {
			if (input_cooldown_ <= 0) {
				player_.cycle_attack();
				inputTaken = true;
			}
		}

        // left click
		if (glfwGetMouseButton(0)) {
			if (player_.cooldown <= 0 && input_cooldown_ <= 0) {
				if(mouseDir == DIR_UP) {
					projectiles_.splice(projectiles_.end(), player_.fire_projectile(ACT_MOVE_UP));
					inputTaken = true;
				}
				else if (mouseDir == DIR_DOWN) {
					projectiles_.splice(projectiles_.end(), player_.fire_projectile(ACT_MOVE_DOWN));
					inputTaken = true;
				}
				else if (mouseDir == DIR_LEFT) {
					projectiles_.splice(projectiles_.end(), player_.fire_projectile(ACT_MOVE_LEFT));
					inputTaken = true;
				}
				else if (mouseDir == DIR_RIGHT)	{
					projectiles_.splice(projectiles_.end(), player_.fire_projectile(ACT_MOVE_RIGHT));
					inputTaken = true;
				}
			}
		}
	}
	
	if (inputTaken) {
		input_cooldown_ = INPUT_COOLDOWN;
	}
}

void Walkabout::update_world(double elapsed)
{
	player_.handle_action(elapsed);
	
	// handle collisions
	std::list<Projectile*>::iterator piter = projectiles_.begin();
	while (piter != projectiles_.end())
	{
		Projectile *proj = *piter;

		proj->handle_action(elapsed);
		
		if (!proj->has_action()) {
			// projectile has expired
			piter = projectiles_.erase(piter);
			delete proj;
			continue;
		}

		if (proj->shooter != &player_ && player_.bounds().collides_with(proj->bounds())) {
			// player was hit
			player_.hit_by(*proj);
			piter = projectiles_.erase(piter);
			delete proj;
			continue;
			
		}

		bool contFlag = false;
		for (Enemy *enemy : enemies_) {
			if (proj->shooter != enemy && enemy->bounds().collides_with(proj->bounds())) {
				// enemy was hit
				enemy->hit_by(*proj);
				piter = projectiles_.erase(piter);
				delete proj;
				contFlag = true;
				break;
			}
		}
		if (contFlag) continue;

		for (Wall *wall : walls_) {
			if (wall->bounds().collides_with(proj->bounds())) {
				// wall was hit
				wall->hit_by(*proj);
				piter = projectiles_.erase(piter);
				delete proj;
				contFlag = true;
				break;
			}
		}
		if (contFlag) continue;

		piter++;
	}

	std::list<Enemy*>::iterator eiter = enemies_.begin();
	while (eiter != enemies_.end()) {
		Drawable *enemy = *eiter;

		enemy->handle_action(elapsed);

		if (enemy->getHp() <= 0) {
			// enemy is dead
			eiter = enemies_.erase(eiter);
			delete enemy;
			continue;
		} else {
			eiter++;
		}
	}
	
	if (player_.getHp() <= 0) {
		// player is dead
	}

}

void Walkabout::update_marked_squares(int dir)
{
	int incX = 0, incY = 0;
	switch (dir)
	{
	case DIR_UP:
		incY = -1000;
		break;
	case DIR_DOWN:
		incY = 1000;
		break;
	case DIR_LEFT:
		incX = -1000;
		break;
	case DIR_RIGHT:
		incX = 1000;
		break;
	}

	marked_squares_.clear();

	int aType = player_.attack_type();
	int aRange = player_.attack_range();
	if (aType == ATK_TYPE_SHOT) {
		for (int i = 0; i < aRange; i += 1000) {
			marked_squares_.push_front(Square(player_.hand_x()+(i*incX), player_.hand_y()+(i*incY)));
		}
	} else if (aType == ATK_TYPE_LINE) {
		for (int i = 0; i < aRange; i += 1000) {
			marked_squares_.push_front(Square(player_.hand_x()+(i*incX), player_.hand_y()+(i*incY)));
		}
	} else if (aType == ATK_TYPE_CONE) {
		// i dont feel like figuring out how to do this with one set of loops...
		int modRange = aRange / 8;
		switch (dir)
		{
		case DIR_UP:
			for (int i = 0; i < modRange; i += 1000) {
				int start = player_.hand_x() - i + 1000;
				for (int j = 0; j < i*2000-1000; j += 1000)
				{
					marked_squares_.push_front(Square(start+j, player_.hand_y()-i));
				}
			}
			break;
		case DIR_DOWN:
			for (int i = 0; i < modRange; i += 1000) {
				int start = player_.hand_x() - i + 1000;
				for (int j = 0; j < i*2000-1000; j += 1000)
				{
					marked_squares_.push_front(Square(start+j, player_.hand_y()+i));
				}
			}
			break;
		case DIR_LEFT:
			for (int i = 0; i < modRange; i += 1000) {
				int start = player_.hand_y() - i + 1000;
				for (int j = 0; j < i*2000-1000; j += 1000)
				{
					marked_squares_.push_front(Square(player_.hand_x()-i, start+j));
				}
			}
			break;
		case DIR_RIGHT:
			for (int i = 0; i < modRange; i += 1000) {
				int start = player_.hand_y() - i + 1000;
				for (int j = 0; j < i*2000-1000; j += 1000)
				{
					marked_squares_.push_front(Square(player_.hand_x()+i, start+j));
				}
			}
			break;
		}
		
	}
}

bool Walkabout::is_visible(int fromX, int fromY, int toX, int toY)
{
	for (Wall *w : walls_)
	{
		if ((w->world_x() > fromX && w->world_x() > toX) || 
			(w->world_x() < fromX && w->world_x() < toX) || 
			(w->world_y() > fromY && w->world_y() > toY) || 
			(w->world_y() < fromY && w->world_y() < toY))
		{
			continue;
		}

		return false;
	}

	return true;
}

