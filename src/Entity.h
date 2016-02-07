#ifndef ENTITY_H_
#define ENTITY_H_

#include <SDL2/SDL.h>
#include <string>
#include <sstream>

#include "EntityProperties.h"
#include "UID.h"

namespace flat2d
{
	class Camera;
	class RenderData;
	class GameData;

	class Entity
	{
		private:
			size_t id;
			bool fixedPosition = false;
			bool inputHandler = false;

		protected:
			EntityProperties entityProperties;
			bool dead = false;
			SDL_Texture *texture = nullptr;
			SDL_Rect clip;

		public:
			Entity(int x, int y, int w, int h) :
				entityProperties(x, y, w, h),
				dead(false) {
					id = UID::generate();

					entityProperties.setColliderShape({ 0, 0, w, h });
					clip = { 0, 0, w, h };
				}

			virtual ~Entity() { }

			/* Operators */
			virtual bool operator==(const Entity& o) const {
				return id == o.id;
			}

			virtual bool operator!=(const Entity& o) const {
				return id != o.id;
			}

			virtual Entity& operator=(const Entity& o) {
				id = o.id;
				return *this;
			}

			virtual std::string getStringId() const {
				std::string number;
				std::stringstream ss;
				ss << id;
				ss >> number;
				return number;
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

			/* Implemented override methods */
			virtual bool isDead() const;
			virtual void render(const RenderData*) const;
			virtual const SDL_Texture* getTexture() const;
			virtual void setTexture(SDL_Texture* texture);
			virtual EntityProperties& getEntityProperties();
			virtual const EntityProperties& getEntityProperties() const;
			virtual bool onCollision(Entity *collider, const GameData*);

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
