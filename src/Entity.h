#ifndef ENTITY_H_
#define ENTITY_H_

#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <memory>
#include <map>

#include "EntityProperties.h"
#include "UID.h"

namespace flat2d
{
	class Camera;
	class RenderData;
	class GameData;
	class Texture;
	class Animation;

	class Entity
	{
		private:
			size_t id;
			bool fixedPosition = false;
			bool inputHandler = false;
			SDL_Rect clip;

		protected:
			EntityProperties entityProperties;
			std::shared_ptr<Texture> texture = nullptr;

			std::weak_ptr<Animation> animation;

			bool dead = false;

		public:
			Entity(int x, int y, int w, int h) :
				entityProperties(x, y, w, h),
				dead(false) {
					id = UID::generate();
					clip = { 0, 0, w, h };
				}

			virtual ~Entity() {
				if (texture != nullptr) {
					texture.reset();
				}
			}

			/* Operators */
			virtual bool operator==(const Entity& o) const {
				return id == o.id;
			}

			virtual bool operator!=(const Entity& o) const {
				return id != o.id;
			}

			virtual Entity& operator=(const Entity& o) {
				// TODO(Linus): This doesn't look right...
				id = o.id;
				return *this;
			}

			virtual int getId() const {
				return static_cast<int>(id);
			}

			virtual std::string getStringId() const {
				std::string number;
				std::stringstream ss;
				ss << id;
				ss >> number;
				return number;
			}

			virtual bool operator<(const Entity& o) const {
				return id < o.id;
			}

			virtual int getType() const {
				return -1;
			}

			/* Own methods */
			void setClip(SDL_Rect&);
			void setDead(bool isDead);
			bool isFixedPosition();
			void setFixedPosition(bool isFixed);
			bool isInputHandler();
			void setInputHandler(bool inputHandler);
			void setAnimation(std::shared_ptr<Animation>);

			/* Implemented override methods */
			virtual bool isDead() const;
			virtual void render(const RenderData*) const;
			virtual const Texture* getTexture() const;
			virtual void setSharedTexture(std::shared_ptr<Texture> texture);
			virtual void setTexture(Texture *texture);
			virtual EntityProperties& getEntityProperties();
			virtual const EntityProperties& getEntityProperties() const;
			virtual bool onCollision(Entity *collider, const GameData*);
			virtual bool onVerticalCollision(Entity *collider, const GameData*);
			virtual bool onHorizontalCollision(Entity *collider, const GameData*);

			/* Empty methods */
			virtual void init(const GameData*) { }
			virtual void preHandle(const GameData*) { }
			virtual void handle(const SDL_Event& event) { }
			virtual void postHandle(const GameData*) { }
			virtual void preRender(const GameData*) { }
			virtual void postRender(const GameData*) { }
			virtual void preMove(const GameData*) { }
			virtual void postMove(const GameData*) { }
	};
} // namespace flat2d

#endif // ENTITY_H_
