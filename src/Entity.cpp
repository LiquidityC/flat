#include <iostream>
#include "Entity.h"
#include "Camera.h"
#include "RenderData.h"

namespace flat2d
{
	void Entity::setDead(bool dead)
	{
		this->dead = dead;
	}

	void Entity::setClip(SDL_Rect& rect)
	{
		clip = rect;
	}

	bool Entity::isDead() const
	{
		return dead;
	}

	void Entity::render(const RenderData *data) const
	{
		if (texture == nullptr || dead) {
			return;
		}

		SDL_Rect box = entityProperties.getBoundingBox();
		if (data->getCamera() != nullptr && !fixedPosition) {
			int z = entityProperties.getDepth();
			Camera* camera = data->getCamera();

			if (!camera->isVisibleOnCamera(box, z)) {
				return;
			}

			box.x = camera->getScreenXposFor(box.x, z);
			box.y = camera->getScreenYposFor(box.y, z);
		}

		SDL_RenderCopy(data->getRenderer(), texture, &clip, &box);

#ifdef COLLISION_DBG
		// Draw collider box
		if (entityProperties.isCollidable()) {
			SDL_SetRenderDrawColor(data->getRenderer(), 0x00, 0xFF, 0x00, 0xFF );
			EntityShape bounds = entityProperties.getColliderShape();
			if (data->getCamera() != nullptr) {
				bounds.x = data->getCamera()->getScreenXposFor(bounds.x);
				bounds.y = data->getCamera()->getScreenYposFor(bounds.y);
			}
			SDL_Rect box = { bounds.x, bounds.y, bounds.w, bounds.h };
			SDL_RenderDrawRect( data->getRenderer(), &box );
		}

		// Draw velocity collider box
		if (entityProperties.isMoving()) {
			flat2d::EntityShape vShape = entityProperties.getVelocityColliderShape(0.017);
			SDL_Rect broadphaseShape = { vShape.x, vShape.y, vShape.w, vShape.h };
			broadphaseShape.x = data->getCamera()->getScreenXposFor(broadphaseShape.x);
			broadphaseShape.y = data->getCamera()->getScreenYposFor(broadphaseShape.y);
			SDL_SetRenderDrawColor(data->getRenderer(), 0x00, 0xFF, 0xFF, 0xFF );
			SDL_RenderDrawRect( data->getRenderer(), &broadphaseShape );
		}

		// Draw spatial partitions
		SDL_SetRenderDrawColor(data->getRenderer(), 0xFF, 0x00, 0x00, 0xFF );
		const flat2d::EntityProperties::Areas currentAreas = entityProperties.getCurrentAreas();
		for(auto it = currentAreas.begin(); it != currentAreas.end(); it++) {
			SDL_Rect bounds = (*it).asSDLRect();
			bounds.x = data->getCamera()->getScreenXposFor(bounds.x);
			bounds.y = data->getCamera()->getScreenYposFor(bounds.y);
			SDL_RenderDrawRect( data->getRenderer(), &bounds );
		}
#endif
	}

	const SDL_Texture* Entity::getTexture() const
	{
		return texture;
	}

	void Entity::setTexture(SDL_Texture* texture)
	{
		this->texture = texture;
	}

	bool Entity::isFixedPosition()
	{
		return fixedPosition;
	}

	void Entity::setFixedPosition(bool fixedPosition)
	{
		this->fixedPosition = fixedPosition;
	}

	bool Entity::isInputHandler()
	{
		return inputHandler;
	}

	void Entity::setInputHandler(bool inputHandler)
	{
		this->inputHandler = inputHandler;
	}

	EntityProperties& Entity::getEntityProperties()
	{
		return entityProperties;
	}

	bool Entity::onCollision(Entity* collider, const GameData *data)
	{
		return false;
	}

	bool Entity::onVerticalCollision(Entity* collider, const GameData *data)
	{
		return false;
	}

	bool Entity::onHorizontalCollision(Entity* collider, const GameData *data)
	{
		return false;
	}

	const EntityProperties& Entity::getEntityProperties() const
	{
		return entityProperties;
	}
} // namespace flat2d
