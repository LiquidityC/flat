#ifndef COLLISIONDETECTOR_H_
#define COLLISIONDETECTOR_H_

#include "EntityShape.h"

namespace flat2d
{
	class EntityContainer;
	class EntityProperties;
	class Entity;
	class DeltatimeMonitor;
	class Square;
	class GameData;

	class CollisionDetector
	{
		private:
			EntityContainer *entityContainer;
			DeltatimeMonitor *dtMonitor;

			bool handlePossibleCollision(Entity*, Entity*, const GameData *data) const;

			void handleHorizontalCollisions(EntityProperties* props1, EntityProperties* props2) const;
			void handleVerticalCollisions(EntityProperties* props1, EntityProperties* props2) const;

			void handleVerticalSolidCollision(EntityProperties* props1, EntityProperties* props2) const;
			void handleHorizontalSolidCollision(EntityProperties* props1, EntityProperties* props2) const;

			void handleVerticalBouncyCollision(EntityProperties* props1, EntityProperties* props2) const;
			void handleHorizontalBouncyCollision(EntityProperties* props1, EntityProperties* props2) const;

			void handleVerticalStickyCollision(EntityProperties* props1, EntityProperties* props2) const;
			void handleHorizontalStickyCollision(EntityProperties* props1, EntityProperties* props2) const;

		public:
			CollisionDetector(EntityContainer *ec, DeltatimeMonitor *dtm) : entityContainer(ec), dtMonitor(dtm) { }

			// Single instance, delete copy operations
			CollisionDetector(const CollisionDetector&) = delete;
			const CollisionDetector& operator=(const CollisionDetector& c) = delete;

			void handlePossibleCollisionsFor(Entity* entity, const GameData *data) const;

			/**
			 * Collision algorithms follow below
			 *
			 * These are public for the sake of testing
			 */
			bool AABB(const EntityShape&, const EntityShape&) const;

			float sweptAABB(EntityProperties* props1, EntityProperties* props2,
					float *normalx, float *normaly) const;
	};
} // namespace flat2d

#endif // COLLISIONDETECTOR_H_
