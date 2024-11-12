#version 450 core

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;

void main() {
    vec4 diffuse = texture(material.diffuse1, TexCoords);
    FragColor = diffuse;
}
