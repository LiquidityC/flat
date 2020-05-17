#ifndef ENTITY_H_
#define ENTITY_H_

#include <SDL.h>
#include <map>
#include <memory>
#include <sstream>
#include <string>

#include "Animation.h"
#include "EntityProperties.h"
#include "UID.h"

namespace flat2d {
	class Camera;
	class RenderData;
	class GameData;
	class Texture;

	/**
	 * The Entity class. You can extend this to create your game objects.
	 * Extend it publicly. To integrate your custom functionality use the
	 * pre/postHandle, pre/postMove, pre/postRender functions.
	 * You may override the handle, move and render functions but shouldn't have
	 * to do this in most cases. The working logic is solid enough most of the
	 * time.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class Entity
	{
	  private:
		size_t id;
		bool fixedPosition = false;
		bool inputHandler = false;
		SDL_Rect clip;
		std::shared_ptr<Texture> texture = nullptr;

	  protected:
		EntityProperties entityProperties;

		std::string currentAnimation = "";
		std::map<std::string, Animation*> animations;

		bool dead = false;

	  public:
		Entity(int x, int y, int w, int h)
		  : entityProperties(x, y, w, h)
		  , dead(false)
		{
			id = UID::generate();
			clip = { 0, 0, w, h };
		}

		Entity(const Entity& e)
		  : entityProperties(e.entityProperties)
		  , dead(e.dead)
		  , clip(e.clip)
		  , id(e.id)
		{}

		virtual ~Entity()
		{
			for (auto it = animations.begin(); it != animations.end(); it++) {
				delete it->second;
			}
		}

		/* srcosrc/Operators */
		virtual bool operator==(const Entity& o) const { return id == o.id; }

		virtual bool operator!=(const Entity& o) const { return id != o.id; }

		virtual Entity& operator=(const Entity& o)
		{
			// TODO(Linus): This doesn't look right...
			id = o.id;
			return *this;
		}

		/**
		 * Get the Entity id
		 * @return the Entity id
		 */
		virtual int getId() const { return static_cast<int>(id); }

		/**
		 * Get the id as a string
		 * @return id as a string
		 */
		virtual std::string getStringId() const
		{
			std::string number;
			std::stringstream ss;
			ss << id;
			ss >> number;
			return number;
		}

		virtual bool operator<(const Entity& o) const { return id < o.id; }

		/**
		 * Get the Entity type. You SHOULD override this in your
		 * derived classes if you want to be able to distinguish between Entity
		 * objects and static_cast Entity objects to your own derived Entity
		 * objects.
		 * @return The entity type
		 */
		virtual int getType() const { return -1; }

		/**
		 * Set the clip for the Entity texture
		 * @param clip The clip to use
		 */
		void setClip(const SDL_Rect&);

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
		 * Add an Animation to this Entity. Animations will
		 * override clip set through setClip. Animations that are added
		 * will be destroyed when the entity is so you don't need to save the
		 * pointer. This function isn't super well tested. Should work though.
		 * @param id A string identifier for this animation
		 * @param animation A shared pointer to the animation
		 */
		void addAnimation(std::string id, Animation* animation);

		/**
		 * Start an animation that was previously added
		 * This will override the clip setting as long as the animation
		 * is running.
		 * @param id The animation identifier
		 */
		void startAnimation(std::string id);

		/**
		 * Stop an animation and return to rendering the clip if one is set
		 */
		void stopAnimations();

		/**
		 * Return the EntityProperties object.
		 * @return A reference to the EntityProperties
		 */
		EntityProperties& getEntityProperties();

		/**
		 * Return a const reference to the EntityProperties
		 * @return a const ref to EntityProperties
		 */
		const EntityProperties& getEntityProperties() const;

		/**
		 * Get the Entity texture
		 * @return a shared_ptr to the Texture object
		 */
		const std::weak_ptr<Texture> getTexture() const;

		/**
		 * Set a shared Texture for this Entity
		 */
		void setSharedTexture(const std::shared_ptr<Texture>& texture);

		/**
		 * Set a Texture for this Entity to use
		 * @param texture The texture to store.
		 */
		void setTexture(Texture* texture);

		/**
		 * Check if the Entity is dead. When this returns true the
		 * EntityContainer will destroy the object next cycle.
		 * @return true or false
		 */
		virtual bool isDead() const;

		/**
		 * Render the Entity
		 * @param data The RenderData object pointer
		 */
		virtual void render(const RenderData*) const;

		/**
		 * Callback that is triggered when the Entity collides.
		 * This will be called for all collisions.
		 * @param collider The object that Entity collided with
		 * @param data The GameData object pointer
		 * @return If the collision is handled or should be handled by the
		 * collision detector
		 */
		virtual bool onCollision(Entity* collider, const GameData*);

		/**
		 * Callback that is called on a vertical collision.
		 * @param collider The object that Entity collided with
		 * @param data The GameData object pointer
		 * @return If the collision is handled or should be handled by the
		 * collision detector
		 */
		virtual bool onVerticalCollision(Entity* collider, const GameData*);

		/**
		 * Callback that is called on a horizontal collision.
		 * @param collider The object that Entity collided with
		 * @param data The GameData object pointer
		 * @return If the collision is handled or should be handled by the
		 * collision detector
		 */
		virtual bool onHorizontalCollision(Entity* collider, const GameData*);

		/**
		 * An init method. Can be used to load resources into Entity
		 * @param gameData The GameData
		 */
		virtual void init(const GameData* gameData) {}

		/**
		 * Callback that is triggered before handle is triggered
		 * @param gameData The GameData
		 */
		virtual void preHandle(const GameData* gameData) {}

		/**
		 * Callback for input events
		 * @param event The SDL_Event
		 */
		virtual void handle(const SDL_Event& event) {}

		/**
		 * Callback that is triggered after handle
		 * @param gameData The GameData
		 */
		virtual void postHandle(const GameData* gameData) {}

		/**
		 * Callback that is triggered before render
		 * @param gameData The GameData
		 */
		virtual void preRender(const GameData* gameData) {}

		/**
		 * Callback that is triggered after render
		 * @param gameData The GameData
		 */
		virtual void postRender(const GameData* gameData) {}

		/**
		 * Callback that is triggered before move
		 * @param gameData The GameData
		 */
		virtual void preMove(const GameData* gameData) {}

		/**
		 * Callback that is triggered after move
		 * @param gameData The GameData
		 */
		virtual void postMove(const GameData* gameData) {}
	};
} // namespace flat2d

#endif // ENTITY_H_
