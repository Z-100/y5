#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_elmoTexture;
uniform sampler2D u_obamaTexture;

uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform vec3 u_viewPos;
uniform vec3 u_objectColor;

out vec4 FragColor;

void main() {

    // Ambient light
    float ambient_strength = 0.1f;
    vec3 ambient_light = ambient_strength * u_lightColor;

    // Diffuse light
    vec3 normalized_normal = normalize(Normal);
    vec3 light_direction = normalize(u_lightPos - FragPos);

    float diffuse = max(dot(normalized_normal, light_direction), 0.0f);
    vec3 diffused_light = diffuse * u_lightColor;

    // Specular
    float specular_strength = 0.5f;
    vec3 view_direction = normalize(u_viewPos - FragPos);
    vec3 reflect_direction = reflect(-light_direction, normalized_normal);
    float specular = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular_glow = specular_strength * specular * u_lightColor;

    // Combined phong
    vec3 phong = (ambient_light + diffused_light + specular_glow) * u_objectColor;

    // Elmo x Obama
    vec4 textureMix = mix(
        texture(u_elmoTexture, TexCoord),
        texture(u_obamaTexture, TexCoord),
        0.5f
    );

    FragColor = vec4(phong * u_objectColor, 1.0f) * textureMix;
}
