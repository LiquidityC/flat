#ifndef BUTTON_H_
#define BUTTON_H_

#include "Entity.h"

namespace flat2d
{
	namespace ui
	{
		class Button : public flat2d::Entity
		{
			private:
				std::function<void ()> onClick;
				bool mouseIsOver = false;

			public:
				Button(int x, int y, unsigned int w, unsigned int h, std::function<void ()> click)
					: Entity(x, y, w, h), onClick(click) {
						entityProperties.setCollidable(false);
					}

				~Button() { }

				void setOnClick(std::function<void ()> click);
				void triggerClick();

				virtual void render(const RenderData* data) const;

				virtual void handle(const SDL_Event&);
		};
	}// namespace ui NOLINT(readability/namespace)
}// namespace flat2d

#endif // BUTTON_H_
