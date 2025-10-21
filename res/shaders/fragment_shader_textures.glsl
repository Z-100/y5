#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_elmoTexture;
uniform sampler2D u_obamaTexture;

uniform Material u_material;
uniform Light    u_light;

uniform vec3 u_viewPos;

out vec4 FragColor;

void main() {

    // Ambient light
    vec3 ambient_light = u_light.ambient * u_material.ambient;

    // Diffuse light
    vec3 normalized_normal = normalize(Normal);
    vec3 light_direction = normalize(u_light.position - FragPos);
    float diffuse = max(dot(normalized_normal, light_direction), 0.0f);
    vec3 diffused_light = u_light.diffuse * (diffuse * u_material.diffuse);

    // Specular
    vec3 view_direction = normalize(u_viewPos - FragPos);
    vec3 reflect_direction = reflect(-light_direction, normalized_normal);
    float specular = pow(max(dot(view_direction, reflect_direction), 0.0f), u_material.shininess);
    vec3 specular_glow = u_light.specular * (specular * u_material.specular);

    // Combined phong
    vec3 phong = (ambient_light + diffused_light + specular_glow);

    // Elmo x Obama
    vec4 textureMix = mix(
        texture(u_elmoTexture, TexCoord),
        texture(u_obamaTexture, TexCoord),
        0.5f
    );

    FragColor = vec4(phong, 1.0f) * textureMix;
}
