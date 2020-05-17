#ifndef COLLISIONDETECTOR_H_
#define COLLISIONDETECTOR_H_

#include "EntityShape.h"

namespace flat2d {
	class EntityContainer;
	class EntityProperties;
	class Entity;
	class DeltatimeMonitor;
	class Square;
	class GameData;

	/**
	 * Checks collisions between objects.
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class CollisionDetector
	{
	  private:
		EntityContainer* entityContainer;
		DeltatimeMonitor* dtMonitor;

		bool handlePossibleCollision(Entity*, Entity*, const GameData* data);

		void handleHorizontalCollisions(EntityProperties* props1,
		                                EntityProperties* props2) const;
		void handleVerticalCollisions(EntityProperties* props1,
		                              EntityProperties* props2) const;

		void handleVerticalSolidCollision(EntityProperties* props1,
		                                  EntityProperties* props2) const;
		void handleHorizontalSolidCollision(EntityProperties* props1,
		                                    EntityProperties* props2) const;

		void handleVerticalBouncyCollision(EntityProperties* props1,
		                                   EntityProperties* props2) const;
		void handleHorizontalBouncyCollision(EntityProperties* props1,
		                                     EntityProperties* props2) const;

		void handleVerticalStickyCollision(EntityProperties* props1,
		                                   EntityProperties* props2) const;
		void handleHorizontalStickyCollision(EntityProperties* props1,
		                                     EntityProperties* props2) const;

	  public:
		CollisionDetector(EntityContainer* ec, DeltatimeMonitor* dtm)
		  : entityContainer(ec)
		  , dtMonitor(dtm)
		{}

		// Single instance, delete copy operations
		CollisionDetector(const CollisionDetector&) = delete;
		const CollisionDetector& operator=(const CollisionDetector& c) = delete;

		/**
		 * Check for a possible collision for an Entity. Used by EntityContainer
		 * avoid using in game code.
		 * @param entity The entity to work on
		 * @param data The GameData object
		 */
		void handlePossibleCollisionsFor(Entity* entity, const GameData* data);

		/**
		 * AABB Collision detection between two EntityShape objects.
		 * This is public due to testing. Let the EntityContainer use it alone
		 * @param b1 The first EntityShape
		 * @param b2 The second EntityShape
		 */
		bool AABB(const EntityShape&, const EntityShape&) const;

		/**
		 * AABB sweeping collision detection between two EntityShape objects.
		 * This is public due to testing. Let the EntityContainer use it alone.
		 * This actually isn't used at the moment because it's a bit broken.
		 */
		float sweptAABB(EntityProperties* props1,
		                EntityProperties* props2,
		                float* normalx,
		                float* normaly) const;
	};
} // namespace flat2d

#endif // COLLISIONDETECTOR_H_
