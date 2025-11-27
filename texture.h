#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

using std::string;

class Uniform; // Forward declaration for friend

class Texture {
	public:
		/// @brief Creates and returns a Texture Object given an image
		/// @param filename The filepath of the image to load
		/// Usage:
		/// Texture image = Texture::from_image("man.jpg");
		/// uniform.send(image);
		/// @return The Texture object
		static Texture from_image(string filename);
	private:
		friend class Uniform; // So that uniform may send image to shader
		Texture();
		unsigned int texture_id;
};

#endif // TEXTURE_H