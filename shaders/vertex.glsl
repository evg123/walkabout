#version 330

in vec3 position_ms;
in vec3 normal_ms;
in vec3 in_color;
in vec2 in_text;

out vec3 v_f_pos_cs;
out vec3 v_f_norm_cs;
out vec3 v_f_color_diffuse;
out vec2 v_f_text;

uniform mat4 proj;
uniform mat4 view;
// TODO dont really need normal model matrix?
uniform mat4 vert_model;
uniform mat4 norm_model;

void main() {
	mat4 ms_to_cs = view * vert_model;
	
	v_f_pos_cs = (ms_to_cs * vec4(position_ms, 1.0)).xyz;
    v_f_norm_cs = (ms_to_cs * vec4(normal_ms, 0.0)).xyz;
    
    
    v_f_color_diffuse = in_color;
    //v_f_color_diffuse = vec3(1.0, 1.0, 1.0);
    
    gl_Position = proj * vec4(v_f_pos_cs, 1.0);
    
    v_f_text = in_text;
}
