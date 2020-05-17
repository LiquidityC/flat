#ifndef BUTTON_H_
#define BUTTON_H_

#include "Entity.h"

namespace flat2d {
	namespace ui {
		/**
		 * This is supposed to be a UI button. The UI hasn't been developed at
		 * all and this button is very untested. It will be done in the future.
		 * But I'm not sure if buttons belong here or not.
		 * @author Linus Probert <linus.probert@gmail.com>
		 */
		class Button : public flat2d::Entity
		{
		  private:
			std::function<void()> onClick;
			bool mouseIsOver = false;

		  public:
			Button(int x,
			       int y,
			       unsigned int w,
			       unsigned int h,
			       const std::function<void()>& click)
			  : Entity(x, y, w, h)
			  , onClick(click)
			{
				entityProperties.setCollidable(false);
			}

			~Button() {}

			void setOnClick(const std::function<void()>& click);
			void triggerClick();

			virtual void render(const RenderData* data) const override;

			virtual void handle(const SDL_Event&) override;
		};
	} // namespace ui
} // namespace flat2d

#endif // BUTTON_H_
