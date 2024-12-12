#pragma once
#ifndef STATIONTEXTURE_H
#define STATIONTEXTURE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../commonFns.h"

class StationTexture
{
public:
	StationTexture();
	StationTexture(const char* fileLoc);

	bool LoadStationTexture();
	bool LoadStationTextureA();

	void UseStationTexture();

private:
	GLuint StationTextureID;
	int width, height, bitDepth;

	const char* fileLocation;
};
#endif