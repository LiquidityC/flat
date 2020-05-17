#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Dimension.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

namespace flat2d {
	/**
	 * A Texture object
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Texture : public Dimension
	{
	  private:
		SDL_Texture* texture = nullptr;
		TTF_Font* font = nullptr;
		double rotation = 0.0f;
		SDL_Point rotationPoint{ 0, 0 };
		SDL_RendererFlip flip = SDL_FLIP_NONE;

		void freeTexture();
		void freeFont();

	  public:
		Texture()
		  : Dimension(0, 0)
		{}
		explicit Texture(SDL_Texture* t)
		  : Dimension(0, 0)
		  , texture(t)
		{}
		virtual ~Texture();

		/**
		 * Render the texture
		 * @param renderer The SDL_Renderer
		 * @param clip The texture clip to render
		 * @param pos Position and dimension to render at
		 */
		void render(SDL_Renderer* renderer,
		            const SDL_Rect* clip,
		            const SDL_Rect* pos) const;

		/**
		 * Load a texture resource from file
		 * @param path The resource path
		 * @param renderer The SDL_Renderer
		 * @return success or fail
		 */
		bool loadFromFile(std::string path, SDL_Renderer* renderer);

		/**
		 * Load a font resource into the Texture
		 * @param path The resource path
		 * @param size The font size
		 * @return success or fail
		 */
		bool loadFont(std::string path, size_t size);

		/**
		 * Create a texture from the loaded font
		 * @param text The message to render
		 * @param color The color to render font with
		 * @param renderer The SDL_Renderer
		 */
		bool loadFromRenderedText(std::string text,
		                          SDL_Color color,
		                          SDL_Renderer* renderer);

		/**
		 * Get current rotation of the texture.
		 * @return the rotation angle
		 */
		double getRotation() const;

		/**
		 * Set the texture rotation.
		 * @param angle The rotation angle.
		 * @param point The point to rotate around
		 */
		void setRotation(double rotation, SDL_Point point);

		/**
		 * Get the texture flip
		 * @return the texture flip type
		 */
		SDL_RendererFlip getFlip() const;

		/**
		 * Set the rendering flip.
		 * @param flip The flip type
		 */
		void setFlip(SDL_RendererFlip flip);
	};
} // namespace flat2d

#endif // TEXTURE_H_
