#ifndef ENTITYPROPERTIES_H_
#define ENTITYPROPERTIES_H_

#include <SDL.h>
#include <functional>
#include <vector>

#include "CollisionProperty.h"
#include "EntityShape.h"
#include "MapArea.h"
#include "Square.h"

namespace flat2d {
	class EntityContainer;

	/**
	 * Holds properties of the Entity to avoid code bloating and improve
	 * code structure. The EntityProperties holds data such as position,
	 * velocity etc. You should never have to create this object in game code.
	 * It's automatically created and attached to your derived Entity objects
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class EntityProperties : public Square
	{
	  public:
		typedef std::vector<MapArea> Areas;

	  private:
		int z = 0;

		float xvel = 0.0f;
		float yvel = 0.0f;

		bool collidable = false;
		bool locationChanged = false;
		bool visible = true;

		CollisionProperty collisionProperty = CollisionProperty::SOLID;

		EntityShape colliderShape = { 0, 0, 0, 0 };
		Areas currentAreas;

		EntityShape getCustomVelocityColliderShape(float dx, float dy) const;

	  public:
		/**
		 * Create an EntityProperties object
		 * @param x The x position
		 * @param y The y position
		 * @param w The width
		 * @param h The height
		 */
		EntityProperties(int x, int y, int w, int h)
		  : Square(x, y, w, h)
		{
			setColliderShape({ 0, 0, w, h });
		}

		/**
		 * Create an EntityProperties object
		 * @param x The x position
		 * @param y The y position
		 * @param dim The dimension (width, height)
		 */
		EntityProperties(int x, int y, int dim)
		  : EntityProperties(x, y, dim, dim)
		{}

		/**
		 * Get the EntityProperties bounding box
		 * @return the bounding box as an SDL_Rect
		 */
		SDL_Rect getBoundingBox() const;

		/**
		 * Get the collider shape for the Entity. This is offset within
		 * EntityProperties bounds meaning that if it starts at the top left
		 * corner the index for x,y will be 0,0
		 * @return The collider shape
		 */
		EntityShape getColliderShape() const;

		/**
		 * Set the collider shape for the Entity
		 * @param shape The EntityShape representing the collider shape
		 */
		void setColliderShape(EntityShape shape);

		/**
		 * Get a collider shape for the Entity with velocity included.
		 * Colliders are bigger when an object is moving.
		 * @return The collider shape with velocity included
		 */
		EntityShape getVelocityColliderShape(float deltatime) const;

		/**
		 * Get the collider shape with only X velocity included
		 * @return a collider shape with only x velocity included
		 */
		EntityShape getXVelocityColliderShape(float deltatime) const;

		/**
		 * Get the collider shape with only Y velocity included
		 * @return a collider shape with only Y velocity included
		 */
		EntityShape getYVelocityColliderShape(float deltatime) const;

		/**
		 * Mark this Entity as collidable
		 * @param collidable true or false
		 */
		void setCollidable(bool collidable);

		/**
		 * Check if the Entity is collidable
		 * @return true or false
		 */
		bool isCollidable() const;

		/**
		 * Mark this Entity as visible (will render)
		 * @param visible true or false
		 */
		void setVisible(bool visible);

		/**
		 * Check if Entity is visible (rendering or not)
		 * @return true or false
		 */
		bool isVisible() const;

		/**
		 * Increment the Entity X postion
		 * @param x The x increment, both negative and possitive values allowed
		 */
		void incrementXpos(int x);

		/**
		 * Set the x position of the Entity
		 * @param pos The new x position
		 */
		void setXpos(int pos) override;

		/**
		 * Set X velocity of the Entity
		 * @param xvel the new x velocity to use
		 */
		void setXvel(float xvel);

		/**
		 * Get the x velocity of the Entity
		 * @return a float value
		 */
		float getXvel() const;

		/**
		 * Increment the y position of the Entity
		 * @param y The y position increment
		 */
		void incrementYpos(int y);

		/**
		 * Set Y position of the Entity
		 * @param pos The new Y position
		 */
		void setYpos(int pos) override;

		/**
		 * Set the Y velocity of the Entity
		 * @param yvel The new velocity
		 */
		void setYvel(float yvel);

		/**
		 * Get the Y velocity of the Entity
		 * @return a float value
		 */
		float getYvel() const;

		/**
		 * Set the Entity depth. This gives a paralax effect
		 * @param depth The depth the entity should render at
		 */
		void setDepth(int depth);

		/**
		 * Get Entity depth
		 * @return an int value
		 */
		int getDepth() const;

		/**
		 * Set the collision property of the Entity. This affects
		 * how Entities behave when default collision handling is used
		 * @param prop The CollisionProperty
		 */
		void setCollisionProperty(CollisionProperty);

		/**
		 * Get the CollisionProperty for this Entity
		 * @return a CollisionProperty
		 */
		CollisionProperty getCollisionProperty() const;

		/**
		 * Get the distance between Entity bounds and Colldier
		 * @return an int value
		 */
		int getColliderLeftOffset() const;

		/**
		 * Get the distance between Entity bounds and Colldier
		 * @return an int value
		 */
		int getColliderRightOffset() const;

		/**
		 * Get the distance between Entity bounds and Colldier
		 * @return an int value
		 */
		int getColliderTopOffset() const;

		/**
		 * Get the distance between Entity bounds and Collider
		 * @return an int value
		 */
		int getColliderBottomOffset() const;

		/**
		 * Check if the Entity is moving
		 * @return true or false
		 */
		bool isMoving() const;

		/**
		 * Move the Entity based on velocity
		 * @param deltatime
		 */
		void move(float deltatime);

		/**
		 * Check if a point is contained between Entity bounds
		 * @return true or false
		 */
		bool containsPoint(int, int) const;

		/**
		 * Get the spatial partitions the Entity currently inhabits
		 * @return a list of Areas
		 */
		Areas& getCurrentAreas();

		/**
		 * Get the spatial partitions the Entity currently inhabits
		 * @return a const list of Areas
		 */
		const Areas& getCurrentAreas() const;

		/**
		 * Mark that the Entity has moved. Used by engine.
		 */
		void setLocationChanged(bool changed);

		/**
		 * Check if Entity has moved
		 * @return true or false
		 */
		bool hasLocationChanged() const;
	};
} // namespace flat2d

#endif // ENTITYPROPERTIES_H_
