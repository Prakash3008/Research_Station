#pragma once
#ifndef STATIONMESH_H
#define STATIONMESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class StationMesh
{
public:
	StationMesh();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~StationMesh();

private:
	GLuint VAO, VBO, EBO;
	GLsizei indexCount;
};

#endif