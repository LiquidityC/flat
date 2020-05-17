#include <algorithm>
#include <cassert>
#include <limits>
#include <stdlib.h>
#include <vector>

#include "Algorithm.h"
#include "CollisionDetector.h"
#include "DeltatimeMonitor.h"
#include "Entity.h"
#include "EntityContainer.h"
#include "EntityShape.h"
#include "GameData.h"

namespace flat2d {
	void CollisionDetector::handlePossibleCollisionsFor(Entity* e,
	                                                    const GameData* data)
	{
		entityContainer->iterateCollidablesFor(e, [this, e, data](Entity* o) {
			EntityShape broadphaseShape =
			  e->getEntityProperties().getVelocityColliderShape(
			    dtMonitor->getDeltaTime());
			if (this->AABB(broadphaseShape,
			               o->getEntityProperties().getColliderShape())) {
				this->handlePossibleCollision(e, o, data);
			}
			return false;
		});
	}

	bool CollisionDetector::handlePossibleCollision(Entity* o1,
	                                                Entity* o2,
	                                                const GameData* data)
	{
		if (*o1 == *o2) {
			return false;
		}

		EntityProperties& props1 = o1->getEntityProperties();
		EntityProperties& props2 = o2->getEntityProperties();

		bool collided = false;
		float deltatime = dtMonitor->getDeltaTime();
		EntityShape colliderShape = props2.getColliderShape();

		float xvel = props1.getXvel() * deltatime;
		if (xvel != 0 &&
		    AABB(props1.getXVelocityColliderShape(deltatime), colliderShape)) {
			if (!o1->onCollision(o2, data) &&
			    !o1->onHorizontalCollision(o2, data)) {
				handleHorizontalCollisions(&props1, &props2);
			}
			o2->onCollision(o1, data);
			o2->onHorizontalCollision(o1, data);
			collided = true;
		}

		float yvel = props1.getYvel() * deltatime;
		if (yvel != 0 &&
		    AABB(props1.getYVelocityColliderShape(deltatime), colliderShape)) {
			if (!o1->onCollision(o2, data) &&
			    !o1->onVerticalCollision(o2, data)) {
				handleVerticalCollisions(&props1, &props2);
			}
			o2->onCollision(o1, data);
			o2->onVerticalCollision(o1, data);
			collided = true;
		}

		return collided;
	}

	bool CollisionDetector::AABB(const EntityShape& b1,
	                             const EntityShape& b2) const
	{
		return !(b1.x > b2.x + b2.w) && !(b1.x + b1.w < b2.x) &&
		       !(b1.y > b2.y + b2.h) && !(b1.y + b1.h < b2.y);
	}

	float CollisionDetector::sweptAABB(EntityProperties* p1,
	                                   EntityProperties* p2,
	                                   float* normalx,
	                                   float* normaly) const
	{
		EntityShape b1 = p1->getColliderShape();
		EntityShape b2 = p2->getColliderShape();

		float xvel = p1->getXvel() * dtMonitor->getDeltaTime();
		float yvel = p1->getYvel() * dtMonitor->getDeltaTime();

		// Find the distances to the blocking object
		float xInvEntry, xInvExit;
		float yInvEntry, yInvExit;

		if (xvel > 0.0f) {
			xInvEntry = static_cast<float>(b2.x - (b1.x + b1.w + 2));
			xInvExit = static_cast<float>((b2.x + b2.w) - (b1.x - 2));
		} else {
			xInvEntry = static_cast<float>((b2.x + b2.w) - (b1.x - 2));
			xInvExit = static_cast<float>(b2.x - (b1.x + b1.w + 2));
		}

		if (yvel > 0.0f) {
			yInvEntry = static_cast<float>(b2.y - (b1.y + b1.w + 2));
			yInvExit = static_cast<float>((b2.y + b2.w) - (b1.y - 2));
		} else {
			yInvEntry = static_cast<float>((b2.y + b2.w) - (b1.y - 2));
			yInvExit = static_cast<float>(b2.y - (b1.y + b1.w + 2));
		}

		// Find the impact time
		float xEntry, xExit;
		float yEntry, yExit;

		if (xvel == 0.0f) {
			xEntry = -std::numeric_limits<float>::infinity();
			xExit = std::numeric_limits<float>::infinity();
		} else {
			xEntry = xInvEntry / xvel;
			xExit = xInvExit / xvel;
		}

		if (yvel == 0.0f) {
			yEntry = -std::numeric_limits<float>::infinity();
			yExit = std::numeric_limits<float>::infinity();
		} else {
			yEntry = yInvEntry / yvel;
			yExit = yInvExit / yvel;
		}

		// Find earliest/latest times of collision
		float entryTime = std::max(xEntry, yEntry);
		float exitTime = std::min(xExit, yExit);

		if (entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f) ||
		    xEntry > 1.0f || yEntry > 1.0f) {
			// No collision
			*normalx = 0.0f;
			*normaly = 0.0f;
			return 1.0f;

		} else {
			if (xEntry > yEntry) {
				if (xInvEntry < 0.0f) {
					*normalx = 1.0f;
					*normaly = 0.0f;
				} else {
					*normalx = -1.0f;
					*normaly = 0.0f;
				}
			} else {
				if (yInvEntry < 0.0f) {
					*normalx = 0.0f;
					*normaly = 1.0f;
				} else {
					*normalx = 0.0f;
					*normaly = -1.0f;
				}
			}
		}
		return entryTime;
	}

	void CollisionDetector::handleHorizontalCollisions(
	  EntityProperties* props1,
	  EntityProperties* props2) const
	{
		assert(props1->getXvel() != 0);

		switch (props1->getCollisionProperty()) {
			case CollisionProperty::BOUNCY:
				handleHorizontalBouncyCollision(props1, props2);
				break;
			case CollisionProperty::STICKY:
				handleHorizontalStickyCollision(props1, props2);
				break;
			case CollisionProperty::ETHERAL:
				break;
			case CollisionProperty::SOLID:
			default:
				handleHorizontalSolidCollision(props1, props2);
		}
	}

	void CollisionDetector::handleVerticalCollisions(
	  EntityProperties* props1,
	  EntityProperties* props2) const
	{
		assert(props1->getYvel() != 0);

		switch (props1->getCollisionProperty()) {
			case CollisionProperty::BOUNCY:
				handleVerticalBouncyCollision(props1, props2);
				break;
			case CollisionProperty::STICKY:
				handleVerticalStickyCollision(props1, props2);
				break;
			case CollisionProperty::ETHERAL:
				break;
			case CollisionProperty::SOLID:
			default:
				handleVerticalSolidCollision(props1, props2);
		}
	}

	void CollisionDetector::handleVerticalSolidCollision(
	  EntityProperties* props1,
	  EntityProperties* props2) const
	{
		EntityShape colliderShape = props2->getColliderShape();

		if (props1->getYvel() > 0) {
			props1->setYpos(colliderShape.y - props1->getHeight() - 1 +
			                props1->getColliderBottomOffset());
		} else {
			props1->setYpos(colliderShape.y + colliderShape.h + 1 -
			                props1->getColliderTopOffset());
		}
		props1->setYvel(0);
	}

	void CollisionDetector::handleHorizontalSolidCollision(
	  EntityProperties* props1,
	  EntityProperties* props2) const
	{
		EntityShape colliderShape = props2->getColliderShape();

		if (props1->getXvel() > 0) {
			props1->setXpos(colliderShape.x - props1->getWidth() - 1 +
			                props1->getColliderRightOffset());
		} else {
			props1->setXpos(colliderShape.x + colliderShape.w + 1 -
			                props1->getColliderLeftOffset());
		}
		props1->setXvel(0);
	}

	void CollisionDetector::handleVerticalBouncyCollision(
	  EntityProperties* props1,
	  EntityProperties* props2) const
	{
		float yvel = props1->getYvel();
		handleVerticalSolidCollision(props1, props2);

		if (yvel > 0) {
			yvel -= 50;
		}
		if (abs(yvel) <= 300) {
			yvel = 0;
		}

		props1->setYvel(-1 * yvel);
	}

	void CollisionDetector::handleHorizontalBouncyCollision(
	  EntityProperties* props1,
	  EntityProperties* props2) const
	{
		float xvel = props1->getXvel();
		handleHorizontalSolidCollision(props1, props2);

		if (xvel > 0) {
			xvel -= 50;
		}
		if (abs(xvel) <= 300) {
			xvel = 0;
		}

		props1->setYvel(-1 * xvel);
	}

	void CollisionDetector::handleVerticalStickyCollision(
	  EntityProperties* props1,
	  EntityProperties* props2) const
	{
		EntityShape colliderShape = props2->getColliderShape();

		if (props1->getYvel() > 0) {
			props1->setYpos(colliderShape.y);
		} else {
			props1->setYpos(colliderShape.y + colliderShape.h -
			                props1->getHeight());
		}
		props1->setXvel(0);
		props1->setYvel(0);
	}

	void CollisionDetector::handleHorizontalStickyCollision(
	  EntityProperties* props1,
	  EntityProperties* props2) const
	{
		EntityShape colliderShape = props2->getColliderShape();

		if (props1->getXvel() > 0) {
			props1->setXpos(colliderShape.x);
		} else {
			props1->setXpos(colliderShape.x + colliderShape.w -
			                props1->getWidth());
		}
		props1->setXvel(0);
		props1->setYvel(0);
	}

} // namespace flat2d
