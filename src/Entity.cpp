#include "Entity.h"
#include "Animation.h"
#include "Camera.h"
#include "RenderData.h"
#include "Texture.h"
#include <cassert>
#include <iostream>
#include <string>

namespace flat2d {
	void Entity::setDead(bool dead) { this->dead = dead; }

	bool Entity::isDead() const { return dead; }

	void Entity::render(const RenderData* data) const
	{
		if (texture == nullptr || dead || !entityProperties.isVisible()) {
			return;
		}

		SDL_Rect bounding_box = entityProperties.getBoundingBox();
		if (data->getCamera() != nullptr && !fixedPosition) {
			int z = entityProperties.getDepth();
			Camera* camera = data->getCamera();

			if (!camera->isVisibleOnCamera(bounding_box, z)) {
				return;
			}

			bounding_box.x = camera->getScreenXposFor(bounding_box.x, z);
			bounding_box.y = camera->getScreenYposFor(bounding_box.y, z);
		}

		const SDL_Rect* renderClip = nullptr;
		if (!currentAnimation.empty()) {
			renderClip = animations.at(currentAnimation)->run();
		} else {
			renderClip = &clip;
		}

		texture.get()->render(data->getRenderer(), renderClip, &bounding_box);

#ifdef COLLISION_DBG
		// Draw collider box
		if (entityProperties.isCollidable()) {
			SDL_SetRenderDrawColor(data->getRenderer(), 0x00, 0xFF, 0x00, 0xFF);
			EntityShape bounds = entityProperties.getColliderShape();
			if (data->getCamera() != nullptr) {
				bounds.x = data->getCamera()->getScreenXposFor(bounds.x);
				bounds.y = data->getCamera()->getScreenYposFor(bounds.y);
			}
			SDL_Rect cbox = { bounds.x, bounds.y, bounds.w, bounds.h };
			SDL_RenderDrawRect(data->getRenderer(), &cbox);
		}

		// Draw velocity collider box
		if (entityProperties.isMoving()) {
			flat2d::EntityShape vShape =
			  entityProperties.getVelocityColliderShape(0.017);
			SDL_Rect broadphaseShape = {
				vShape.x, vShape.y, vShape.w, vShape.h
			};
			broadphaseShape.x =
			  data->getCamera()->getScreenXposFor(broadphaseShape.x);
			broadphaseShape.y =
			  data->getCamera()->getScreenYposFor(broadphaseShape.y);
			SDL_SetRenderDrawColor(data->getRenderer(), 0x00, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawRect(data->getRenderer(), &broadphaseShape);
		}

		// Draw spatial partitions
		SDL_SetRenderDrawColor(data->getRenderer(), 0xFF, 0x00, 0x00, 0xFF);
		const flat2d::EntityProperties::Areas currentAreas =
		  entityProperties.getCurrentAreas();
		for (auto it = currentAreas.begin(); it != currentAreas.end(); it++) {
			SDL_Rect bounds = (*it).asSDLRect();
			bounds.x = data->getCamera()->getScreenXposFor(bounds.x);
			bounds.y = data->getCamera()->getScreenYposFor(bounds.y);
			SDL_RenderDrawRect(data->getRenderer(), &bounds);
		}
#endif
	}

	void Entity::setClip(const SDL_Rect& clip) { this->clip = clip; }

	const std::weak_ptr<Texture> Entity::getTexture() const { return texture; }

	void Entity::setSharedTexture(const std::shared_ptr<Texture>& texture)
	{
		this->texture = texture;
	}

	void Entity::setTexture(Texture* texture)
	{
		this->texture = std::shared_ptr<Texture>(texture);
	}

	bool Entity::isFixedPosition() { return fixedPosition; }

	void Entity::setFixedPosition(bool fixedPosition)
	{
		this->fixedPosition = fixedPosition;
	}

	bool Entity::isInputHandler() { return inputHandler; }

	void Entity::setInputHandler(bool inputHandler)
	{
		this->inputHandler = inputHandler;
	}

	void Entity::addAnimation(std::string id, Animation* animation)
	{
		assert(animations.find(id) == animations.end());

		animations[id] = animation;
	}

	void Entity::startAnimation(std::string id)
	{
		assert(animations.find(id) != animations.end());

		if (!currentAnimation.empty()) {
			animations[currentAnimation]->stop();
		}
		currentAnimation = id;
		animations[currentAnimation]->start();
	}

	void Entity::stopAnimations()
	{
		animations[currentAnimation]->stop();
		currentAnimation = "";
	}

	EntityProperties& Entity::getEntityProperties() { return entityProperties; }

	bool Entity::onCollision(Entity* collider, const GameData* data)
	{
		return false;
	}

	bool Entity::onVerticalCollision(Entity* collider, const GameData* data)
	{
		return false;
	}

	bool Entity::onHorizontalCollision(Entity* collider, const GameData* data)
	{
		return false;
	}

	const EntityProperties& Entity::getEntityProperties() const
	{
		return entityProperties;
	}
} // namespace flat2d
