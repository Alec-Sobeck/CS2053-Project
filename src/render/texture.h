#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include <string>

/**
 * Texture is a simple class for managing OpenGL textures. A texture object is created
 * with an integer value (the textureID), and can then be bound using {@link #bind()}. This will cause
 * images drawn to use this texture, until another Texture object's bind() method is called.
 * @author      Alec Sobeck
 * @author      Matthew Robertson
 * @version     1.0
 * @since       1.0
 */
class Texture
{
public:
	const unsigned int textureID;
	const std::string associatedFileName;

	/**
	 * Constructs a new Texture object, storing the specified textureID for binding
	 * @param textureID the Integer representing the texture that can be bound
	 */
	Texture(std::string associatedFileName, unsigned int textureID);
	/**
	 * Binds the texture using GL11. The texture will remain bound until the next bind() call of a different
	 * texture object, or manual call to GL11.glBindTexture(...)
	 */
	void bind();
};


#endif
