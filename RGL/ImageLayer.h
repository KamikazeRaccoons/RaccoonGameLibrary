#pragma once

#include "Layer.h"

namespace rgl
{
	class ImageLayer : public Layer
	{
	private:

		int m_x;
		int m_y;

		int m_width;
		int m_height;

		std::string m_textureID;

	public:

		ImageLayer(int x, int y, int width, int height, std::string textureID, std::shared_ptr<Level> pLevel)
			: Layer(pLevel), m_x(x), m_y(y), m_width(width), m_height(height), m_textureID(textureID) { }
		
		virtual void update() { }
		virtual void render();
		virtual void clean() { }

	};
}
