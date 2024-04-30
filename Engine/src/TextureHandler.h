//#pragma once 
//
//#include <memory>
//
//#include "Core.h"
//#include "Texture.h"
//
///**
// * This class is the main texture handling utlity, it wraps the texture class (aka the texture resource) and is used
// * to manipulate the resource for different uses, for example, a single texture can be used by 2 different TextureHandler
// * classes differently, by one textureHandler the texture can be used as a diffuse map and by the 2nd textureHandler as a specular map,
// * this while still only using 1 texture instance in memory.
// */
//class EngineAPI TextureHandler
//{
//public:
//	/** Constructor */
//	TextureHandler(Texture* texture);
//
//	/** Constructor */
//	TextureHandler(std::shared_ptr<Texture> texture);
//
//	/** Binds the GPU to the assigned texture */
//	void bind() const;
//
//	/** Unbinds the assigned texture from the GPU */
//	void unbind() const;
//
//	/**
//	 * Sets the texture type.
//	 *
//	 * \param type	The texture type to set
//	 */
//	void setType(Texture::Type type);
//
//	/**
//	 * Get the texture type.
//	 *
//	 * \return Type of this texture
//	 */
//	Texture::Type getType() const;
//
//	/**
//	 * Retruns the assigned texture ID.
//	 * 
//	 * \return	Texture ID
//	 */
//	unsigned int getID() const;
//
//	inline void setSlot(int slot) { m_texture->setSlot(slot); }
//
//
//private:
//	std::shared_ptr<Texture> m_texture = nullptr;
//
//	Texture::Type m_type = Texture::Type::None;
//};
