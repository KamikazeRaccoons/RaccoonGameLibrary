#include "ImageLayer.h"
#include "TextureManager.h"

namespace rgl
{
	void ImageLayer::render()
	{
		TextureManager::get()->draw(m_textureID, m_x, m_y, m_width, m_height);
	}
}