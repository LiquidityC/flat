#include <functional>
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <cmath>

#include "Entity.h"
#include "EntityContainer.h"
#include "GameData.h"
#include "RenderData.h"
#include "EntityProperties.h"
#include "DeltatimeMonitor.h"
#include "CollisionDetector.h"
#include "RuntimeAnalyzer.h"
#include "Camera.h"

namespace flat2d
{
	EntityContainer::~EntityContainer()
	{
		if (quadTree) {
			delete quadTree;
		}
		unregisterAllObjects();
	}

	void EntityContainer::addLayer(unsigned int layer)
	{
		Layer newLayer = static_cast<Layer>(layer);
		if (layeredObjects.find(newLayer) != layeredObjects.end()) {
			return;
		}
		ObjectList list;
		layeredObjects[newLayer] = list;
	}

	std::vector<int> EntityContainer::getLayerKeys() const
	{
		std::vector<int> keys;
		for (auto& it : layeredObjects) {
			keys.push_back(it.first);
		}
		return keys;
	}

	void EntityContainer::registerObject(Entity* object, Layer layer)
	{
		std::string objId = object->getStringId();
		if (objects.find(objId) != objects.end()) {
			return;
		}

		// Make sure this layer exists
		if (layeredObjects.find(layer) == layeredObjects.end()) {
			return;
		}

		objects[objId] = object;
		uninitiatedEntities[objId] = object;
		layeredObjects[layer][objId] = object;
		if (object->isInputHandler()) {
			inputHandlers[objId] = object;
		}
		if (object->getEntityProperties().isCollidable()) {
			collidableObjects[objId] = object;
		}
	}

	void EntityContainer::repopulateCollidables()
	{
		collidableObjects.clear();
		for (auto& it : objects) {
			if (it.second->getEntityProperties().isCollidable()) {
				collidableObjects[it.first] = it.second;
			}
		}
	}

	void EntityContainer::unregisterObject(Entity* object)
	{
		std::string objId = object->getStringId();
		objects.erase(objId);
		inputHandlers.erase(objId);
		uninitiatedEntities.erase(objId);
		if (object->getEntityProperties().isCollidable()) {
			collidableObjects.erase(objId);
		}

		for (auto it = layeredObjects.begin(); it != layeredObjects.end(); it++) {
			it->second.erase(objId);
		}
	}

	void EntityContainer::reinitLayerMap()
	{
		layeredObjects.clear();
		ObjectList list;
		layeredObjects[-1] = list;
	}

	bool EntityContainer::isUninitiated(const std::string& id) const
	{
		return uninitiatedEntities.find(id) != uninitiatedEntities.end();
	}

	void EntityContainer::unregisterAllObjects()
	{
		for(auto it = objects.begin(); it != objects.end(); it++) {
			delete it->second;
		}
		uninitiatedEntities.clear();
		objects.clear();
		collidableObjects.clear();
		inputHandlers.clear();
		reinitLayerMap();
		for (auto it = renderAreas.begin(); it != renderAreas.end(); it++) {
			delete it->second;
		}
		renderAreas.clear();
	}

	void EntityContainer::unregisterAllObjectsFor(Layer layer)
	{
		if (layeredObjects.find(layer) == layeredObjects.end()) {
			return;
		}

		for (auto it = layeredObjects[layer].begin(); it != layeredObjects[layer].end(); it++) {
			std::string objId = it->second->getStringId();
			objects.erase(objId);
			inputHandlers.erase(objId);
			uninitiatedEntities.erase(objId);
			if (it->second->getEntityProperties().isCollidable()) {
				collidableObjects.erase(objId);
			}
			delete it->second;
		}

		layeredObjects[layer].clear();
	}

	void EntityContainer::initiateEntities(const GameData *gameData)
	{
#ifdef FPS_DBG
		TIME_FUNCTION;
#endif
		for (auto it = uninitiatedEntities.begin(); it != uninitiatedEntities.end(); it++) {
			it->second->init(gameData);
		}
		uninitiatedEntities.clear();
	}

	void EntityContainer::handleObjects(const SDL_Event& event, const GameData* gameData)
	{
#ifdef FPS_DBG
		TIME_FUNCTION;
#endif
		for (auto it = inputHandlers.begin(); it != inputHandlers.end(); it++) {
			if (isUninitiated(it->first)) {
				continue;
			}
			it->second->preHandle(gameData);
			it->second->handle(event);
			it->second->postHandle(gameData);
		}
	}

	void EntityContainer::renderObjects(const GameData* data) const
	{
#ifdef FPS_DBG
		TIME_FUNCTION;
#endif
		for (auto it1 = layeredObjects.begin(); it1 != layeredObjects.end(); it1++) {
			for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
				if (isUninitiated(it2->first)) {
					continue;
				}
				it2->second->preRender(data);
				if (isInRenderArea(it2->second, data)) {
					it2->second->render(data->getRenderData());
				}
				it2->second->postRender(data);
			}
		}
#ifdef COLLISION_DBG
				quadTree->render(data->getRenderData());
#endif
	}

	bool EntityContainer::isInRenderArea(Entity *e, const GameData* data) const
	{
		if (renderAreas.empty()) {
			return true;
		}

		for (const auto& it : renderAreas) {
			if (data->getCollisionDetector()->AABB(it.second->asEntityShape(),
						e->getEntityProperties().getColliderShape())) {
				return true;
			}
		}
	}

	void EntityContainer::moveObjects(const GameData* data)
	{
#ifdef FPS_DBG
		TIME_FUNCTION;
#endif
		float deltatime = dtMonitor->getDeltaTime();
		CollisionDetector *coldetector = data->getCollisionDetector();

		if (quadTree != nullptr) {
			delete quadTree;
		}
		Camera *camera = data->getRenderData()->getCamera();
		quadTree = new QuadTree(Square(0, 0, camera->getMapWidth(), camera->getMapHeight()));

		quadTree->clear();
		for (auto it = collidableObjects.begin(); it != collidableObjects.end(); ++it) {
			quadTree->insert(it->second);
		}

		for (auto& object : objects) {
			if (isUninitiated(object.first)) {
				continue;
			}
			object.second->preMove(data);

			EntityProperties& props = object.second->getEntityProperties();
			if (props.isMoving()) {
				if (props.isCollidable()) {
					coldetector->handlePossibleCollisionsFor(object.second, data);
				}
				props.move(deltatime);
			}

			object.second->postMove(data);
		}

		clearDeadObjects();
	}

	size_t EntityContainer::getObjectCount() const
	{
		return objects.size();
	}

	size_t EntityContainer::getObjectCountFor(Layer layer)
	{
		if (layeredObjects.find(layer) == layeredObjects.end()) {
			return 0;
		}

		return layeredObjects[layer].size();
	}

	size_t EntityContainer::getCollidablesCount() const
	{
		return collidableObjects.size();
	}

	void EntityContainer::clearDeadObjects()
	{
		std::vector<std::string> objectsToErase;
		for (auto it = objects.begin(); it != objects.end(); it++) {
			if (!it->second->isDead()) {
				continue;
			}
			std::string objId = it->first;
			collidableObjects.erase(objId);
			for (auto layerIt = layeredObjects.begin(); layerIt != layeredObjects.end(); layerIt++) {
				layerIt->second.erase(objId);
			}
			objectsToErase.push_back(objId);
			delete it->second;
		}

		for (auto it = objectsToErase.begin(); it != objectsToErase.end(); it++) {
			objects.erase(*it);
		}
	}

	void EntityContainer::iterateAllMovingObjects(EntityIter func) const
	{
		for (auto it = objects.begin(); it != objects.end(); it++) {
			if (it->second->getEntityProperties().isMoving()) {
				func(it->second);
			}
		}
	}

	void EntityContainer::iterateCollidablesFor(const Entity* source, EntityIter func)
	{
		std::vector<Entity*> collidables;
		if (!quadTree) {
			return;
		}
		quadTree->retrieve(&collidables, source);

		for (auto it = collidables.begin(); it != collidables.end(); ++it) {
			func(*it);
		}
	}

	Entity* EntityContainer::checkAllObjects(EntityProcessor func) const
	{
		for (auto it = objects.begin(); it != objects.end(); it++) {
			if (func(it->second)) {
				return it->second;
			}
		}
		return nullptr;
	}

	Entity* EntityContainer::checkAllCollidableObjects(EntityProcessor func) const
	{
		for (auto it = collidableObjects.begin(); it != collidableObjects.end(); it++) {
			if (func(it->second)) {
				return it->second;
			}
		}
		return nullptr;
	}

	Entity* EntityContainer::checkCollidablesFor(const Entity* source, EntityProcessor func)
	{
		std::vector<Entity*> collidables;
		quadTree->retrieve(&collidables, source);
		for (auto it = collidables.begin(); it != collidables.end(); ++it) {
			func(*it);
		}
		return nullptr;
	}

	void EntityContainer::iterateCollidablesIn(Layer layer, EntityIter func)
	{
		if (layeredObjects.find(layer) == layeredObjects.end()) {
			return;
		}

		for (auto it = layeredObjects[layer].begin(); it != layeredObjects[layer].end(); it++) {
			if (it->second->getEntityProperties().isCollidable()) {
				func(it->second);
			}
		}
	}

	void EntityContainer::addRenderArea(const std::string& key, MapArea *area)
	{
		renderAreas[key] = area;
	}

	MapArea* EntityContainer::getRenderArea(const std::string& key)
	{
		return renderAreas[key];
	}

	void EntityContainer::removeRenderArea(const std::string& key)
	{
		MapArea *area = renderAreas[key];
		if (area != nullptr) {
			delete area;
		}
	}
} // namespace flat2d
