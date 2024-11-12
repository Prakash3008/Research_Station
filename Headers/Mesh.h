#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

// Texture structure
struct Texture {
    unsigned int id;
    std::string type;  // e.g., "texture_diffuse"
    std::string path;  // Used to check for duplicates
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    // Constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    // Render the mesh
    void Draw(unsigned int shaderID);

private:
    unsigned int VBO, EBO;

    void setupMesh();
};

#endif
