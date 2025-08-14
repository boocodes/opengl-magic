#include "Font.h"

Font::Font()
{
	return;
}
void Font::drawText(std::string textDisplay, float xPos, float yPos, glm::vec3 color)
{
	if (textDisplay.size() == 0)
	{
		textDisplay = std::to_string(this->id);
	}
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

}