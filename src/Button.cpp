#include "Button.h"
#include "RenderData.h"

namespace flat2d {
	/**
	 * The UI is more or less untested and far from completed. You should most
	 * likely avoid the contained objects until it's completed.
	 */
	namespace ui {
		void Button::handle(const SDL_Event& e)
		{
			if (e.type == SDL_MOUSEMOTION) {
				int x = e.motion.x;
				int y = e.motion.y;

				if (x < entityProperties.getXpos() ||
				    x > entityProperties.getXpos() +
				          entityProperties.getWidth()) {
					mouseIsOver = false;
					return;
				}
				if (y < entityProperties.getYpos() ||
				    y > entityProperties.getYpos() +
				          entityProperties.getHeight()) {
					mouseIsOver = false;
					return;
				}
				mouseIsOver = true;
			}

			if (e.type == SDL_MOUSEBUTTONDOWN &&
			    e.button.button == SDL_BUTTON_LEFT && mouseIsOver) {
				triggerClick();
			}
		}

		void Button::render(const RenderData* data) const
		{
			// Draw the box
			SDL_Renderer* renderer = data->getRenderer();
			SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
			SDL_Rect rect = { entityProperties.getXpos(),
				              entityProperties.getYpos(),
				              entityProperties.getWidth(),
				              entityProperties.getHeight() };
			SDL_RenderFillRect(renderer, &rect);

			// Draw the border
			if (mouseIsOver) {
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			} else {
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
			SDL_RenderDrawRect(renderer, &rect);
		}

		void Button::setOnClick(const std::function<void()>& click)
		{
			onClick = click;
		}

		void Button::triggerClick() { onClick(); }
	} // namespace ui
} // namespace flat2d
