#version 330

in vec3 v_f_pos_cs;
in vec3 v_f_norm_cs;
in vec3 v_f_color_diffuse;
in vec2 v_f_text;

// maybe convert to uniforms later
const vec4 light_amt = vec4(3.5, 3.5, 3.5, 1.0);
//const vec3 light_dir = normalize(vec3(1.0, 1.0, 0.0));
const vec4 v_f_ambient_amt = vec4(0.3, 0.3, 0.3, 1.0);

uniform int light_count;
uniform vec3 light_pos_cs[16];
uniform sampler2D texture_sampler;

out vec4 out_color;

void main() {
    
    vec4 light_summation = (v_f_ambient_amt * vec4(v_f_color_diffuse, 1.0));
    for (int i = 0; i < light_count; i++) {
        
        vec3 light_diff = light_pos_cs[i] - v_f_pos_cs;
        float light_dist_sqr = dot(light_diff, light_diff);
        vec3 light_dir_to_camera = light_diff * inversesqrt(light_dist_sqr);

        //vec4 light_amt_atten = light_amt * (1 / (1.0 + .0005 * sqrt(light_dist_sqr)));
        vec4 light_amt_atten = light_amt * (1 / (1.0 + .1 * sqrt(light_dist_sqr)));

        float angle = dot(normalize(v_f_norm_cs), light_dir_to_camera);
        angle = clamp(angle, 0, 1);
        
        float shininess = 0.8;
        vec3 reflect_dir = reflect(light_dir_to_camera, normalize(v_f_norm_cs));
        float phong = dot(normalize(v_f_pos_cs), reflect_dir);
        phong = clamp(phong, 0, 1);
        //phong = angle != 0.0 ? phong : 0.0;
        phong = pow(phong, shininess);
        
        light_summation += (light_amt_atten * angle * vec4(v_f_color_diffuse, 1.0)) + (light_amt_atten * phong * vec4(v_f_color_diffuse, 1.0));
    }
    
    out_color = texture(texture_sampler, v_f_text) * light_summation;
    //out_color = (v_f_ambient_amt * vec4(v_f_color_diffuse, 1.0)) + (light_amt_atten * angle * vec4(v_f_color_diffuse, 1.0));
    //out_color = texture(texture_sampler, v_f_text) * ((v_f_ambient_amt * vec4(v_f_color_diffuse, 1.0)) + (light_amt_atten * angle * vec4(v_f_color_diffuse, 1.0)));
    //out_color = texture(texture_sampler, v_f_text);
    //out_color = light_summation;
}
