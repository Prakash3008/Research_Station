#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D matTexture;

void main() {
    vec4 diffuse = texture(matTexture, TexCoords);
    FragColor = diffuse;
}
