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
			Camera* camera = data->getCamera();
			if (!camera->isVisibleOnCamera(box)) {
				return;
			}
			box.x = camera->getScreenXposFor(box.x);
			box.y = camera->getScreenYposFor(box.y);
		}

		SDL_RenderCopy(data->getRenderer(), texture, &clip, &box);

#ifdef DEBUG
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

		if (entityProperties.isMoving()) {
			flat2d::EntityShape vShape = entityProperties.getVelocityColliderShape(0.017);
			SDL_Rect broadphaseShape = { vShape.x, vShape.y, vShape.w, vShape.h };
			broadphaseShape.x = data->getCamera()->getScreenXposFor(broadphaseShape.x);
			broadphaseShape.y = data->getCamera()->getScreenYposFor(broadphaseShape.y);
			SDL_SetRenderDrawColor(data->getRenderer(), 0x00, 0xFF, 0xFF, 0xFF );
			SDL_RenderDrawRect( data->getRenderer(), &broadphaseShape );
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

	const EntityProperties& Entity::getEntityProperties() const
	{
		return entityProperties;
	}
} // namespace flat2d
