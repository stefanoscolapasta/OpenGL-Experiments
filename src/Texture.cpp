#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); //flips our texture vertically upside down, because opengl expects tex coords to start at bottom left, not top left (for PNGs)
	//we pass widthm height, mpbb and others so that stbi can write to them
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); //last 4 is num of channels
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//Texture settings/params (next 4 are mandatory)
	//GL_TEXTURE_MIN_FILTER how to resample/scale down/up if it needs to
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); //mag stands for magnification (scale up)

	//S and T is like X,Y for textures --> Wrap tells how to wrap Horiz and Vertic, in our case we clamp, we could tile if wanted
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//second par, when to 0 says tex is not multilevel
	//first GL_RGBA8 (8 == bits per channel) is an internal format to tell it how to store the data, not to specify how the data in the passed image is stored
	//as last argument can pass nullptr as all info to allocate correct space is specified and can supply image later, we will supply the data now
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	//GL_TEXTURE0, GL_TEXTURE1, ... , GL_TEXTUREn (n max 31, but depends on platform/GPU) represents the slot we want to bind the texture to
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); //to understand look at OpenGL texture slot enum definition :3
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

