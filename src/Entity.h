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

	/**
	 * The Entity class. You can extend this to create your game objects.
	 * Extend it publicly. To integrate your custom functionality use the
	 * pre/postHandle, pre/postMove, pre/postRender functions.
	 * You may override the handle, move and render functions but shouldn't have
	 * to do this in most cases. The working logic is solid enough most of the time.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
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

			/**
			 * Get the Entity id
			 * @return the Entity id
			 */
			virtual int getId() const {
				return static_cast<int>(id);
			}

			/**
			 * Get the id as a string
			 * @return id as a string
			 */
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

			/**
			 * Get the Entity type. You SHOULD override this in your
			 * derived classes if you want to be able to distinguish between Entity
			 * objects and static_cast Entity objects to your own derived Entity objects.
			 * @return The entity type
			 */
			virtual int getType() const {
				return -1;
			}

			/**
			 * Set the clip for the Entity texture
			 * @param clip The clip to use
			 */
			void setClip(SDL_Rect&);

			/**
			 * Mark the Entity as dead.
			 */
			void setDead(bool isDead);

			/**
			 * Check if this is a fixed position Entity
			 * @return true or false
			 */
			bool isFixedPosition();

			/**
			 * Mark this Entity as fixed. It will then always render at the same
			 * position on screen. Useful for 'HUD' graphics.
			 * @param isFixed boolean value
			 */
			void setFixedPosition(bool isFixed);

			/**
			 * Check if this Entity handles input
			 * @return true or false
			 */
			bool isInputHandler();

			/**
			 * Mark this Entity as an input handler. If set to false
			 * the handle method won't be called on this Entity. Mostly
			 * good for performance to set this.
			 * @param inputHandler true or false
			 */
			void setInputHandler(bool inputHandler);

			/**
			 * Set an Animation for this Entity. Animations will 
			 * override clip set through setClip
			 * This function isn't super well tested. Should work though.
			 * @param animation A shared pointer to the animation
			 */
			void setAnimation(std::shared_ptr<Animation>);

			/**
			 * Get the Entity texture
			 * @return a shared_ptr to the Texture object
			 */
			const std::weak_ptr<Texture> getTexture() const;

			/**
			 * Set a shared Texture for this Entity
			 */
			void setSharedTexture(std::shared_ptr<Texture> texture);

			/**
			 * Set a Texture for this Entity to use
			 * @param texture The texture to store.
			 */
			void setTexture(Texture *texture);

			/**
			 * Check if the Entity is dead. When this returns true the EntityContainer
			 * will destroy the object next cycle.
			 * @return true or false
			 */
			virtual bool isDead() const;

			/**
			 * Render the Entity
			 * @param data The RenderData object pointer
			 */
			virtual void render(const RenderData*) const;
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
