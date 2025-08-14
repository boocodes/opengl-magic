#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

#include <map>
#include <random>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H


struct Character
{
	unsigned int TextureId;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class Font {
private:
	int id;
	int pixel_size_px = 48;
	std::string font_name;
	unsigned int VAO, VBO, texture;
	std::map<GLchar, Character> Characters;
public:
	Font();
	void drawText(std::string textDisplay = "", float xPos = 0, float yPos = 20, glm::vec3 color = glm::vec3(0, 0, 0));
	void init(std::string fontName, int pixelSize = 48);
};