#version 330 core

in vec2 fragTexCoord; // Interpolated texture coordinates from the vertex shader
out vec4 fragColor;   // Final color output

uniform sampler2D matTexture;  // Texture sampler
uniform float transparency; // Transparency level (0.0 = fully transparent, 1.0 = fully opaque)

void main() {
    vec4 texColor = texture(matTexture, fragTexCoord); // Sample texture color

    texColor.a *= transparency; // Adjust alpha based on transparency uniform
    if (texColor.a < 0.1)
        discard; // Discard fragments that are almost fully transparent
    fragColor = vec4(texColor.rgb, texColor.a * transparency); // Apply transparency

}
