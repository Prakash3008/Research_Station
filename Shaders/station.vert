#version 330 core

layout(location = 0) in vec3 position; // Vertex position
layout(location = 1) in vec2 texCoord; // Texture coordinates

out vec2 fragTexCoord; // Pass texture coordinates to the fragment shader

uniform mat4 model;      // Model matrix
uniform mat4 view;       // View matrix
uniform mat4 projection; // Projection matrix

void main() {
    fragTexCoord = texCoord;
    gl_Position = projection * view * model * vec4(position, 1.0);
}
