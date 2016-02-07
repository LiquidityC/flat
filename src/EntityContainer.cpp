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


namespace flat2d
{
	EntityContainer::~EntityContainer()
	{
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
		layeredObjects[layer][objId] = object;
		registerObjectToSpatialPartitions(object);
		if (object->isInputHandler()) {
			inputHandlers[objId] = object;
		}
		if (object->getEntityProperties().isCollidable()) {
			collidableObjects[objId] = object;
		}
	}

	EntityShape EntityContainer::createBoundingBoxFor(const EntityProperties& props) const
	{
		EntityShape vShape = props.getVelocityColliderShape(dtMonitor->getDeltaTime());
		return {
			vShape.x - spatialPartitionExpansion,
			vShape.y - spatialPartitionExpansion,
			vShape.w + (2 * spatialPartitionExpansion),
			vShape.h + (2 * spatialPartitionExpansion)
		};
	}

	void EntityContainer::registerObjectToSpatialPartitions(Entity *o)
	{
		if (!o->getEntityProperties().isCollidable()) {
			return;
		}

		EntityProperties& props = o->getEntityProperties();
		EntityShape boundingBox = createBoundingBoxFor(props);

		if (static_cast<unsigned int>(boundingBox.w) > spatialPartitionDimension ||
				static_cast<unsigned int>(boundingBox.h) > spatialPartitionDimension)
		{
			std::cout << "Warning: Spatial partitions are to small in size"
				<< " " << boundingBox.w << "x" << boundingBox.h << ">"
				<< spatialPartitionDimension << std::endl;
		}

		addObjectToSpatialPartitionFor(o, boundingBox.x, boundingBox.y);
		addObjectToSpatialPartitionFor(o, boundingBox.x, boundingBox.y + boundingBox.h);
		addObjectToSpatialPartitionFor(o, boundingBox.x + boundingBox.w, boundingBox.y);
		addObjectToSpatialPartitionFor(o, boundingBox.x + boundingBox.w, boundingBox.y + boundingBox.h);

		props.setLocationChanged(false);
	}

	void EntityContainer::clearObjectFromCurrentPartitions(Entity *o)
	{
		EntityProperties::Areas& currentAreas = o->getEntityProperties().getCurrentAreas();
		for (auto it = currentAreas.begin(); it != currentAreas.end(); it++) {
			spatialPartitionMap[*it].erase(o->getStringId());
		}

		currentAreas.clear();
	}

	void EntityContainer::clearObjectFromUnattachedPartitions(Entity *o)
	{
		// TODO(Linus): This is bugging out. Fix later for optimization.
		EntityProperties::Areas& currentAreas = o->getEntityProperties().getCurrentAreas();
		EntityShape bounder = createBoundingBoxFor(o->getEntityProperties());

		std::vector<int> indexes;
		int index = 0;
		for (auto it = currentAreas.begin(); it != currentAreas.end(); it++) {
			if (!it->containsPoint(bounder.x, bounder.y)
					&& !it->containsPoint(bounder.x, bounder.y + bounder.h)
					&& !it->containsPoint(bounder.x + bounder.w, bounder.y)
					&& !it->containsPoint(bounder.x + bounder.w, bounder.y + bounder.h))
			{
				spatialPartitionMap[*it].erase(o->getStringId());
				indexes.push_back(index++);
			}
		}

		for (auto it = indexes.begin(); it != indexes.end(); it++) {
			currentAreas.erase(currentAreas.begin() + *it);
		}
	}

	void EntityContainer::addObjectToSpatialPartitionFor(Entity* o, int x, int y)
	{
		std::string objId = o->getStringId();

		unsigned int xcord = (x - (x % spatialPartitionDimension));
		unsigned int ycord = (y - (y % spatialPartitionDimension));

		// Find and make sure partition exists
		MapArea area(xcord, ycord, spatialPartitionDimension);
		if (spatialPartitionMap.find(area) == spatialPartitionMap.end()) {
			spatialPartitionMap[area] = ObjectList();
		}

		// The object is already in this partition
		if (spatialPartitionMap[area].find(objId) != spatialPartitionMap[area].end()) {
			return;
		}

		EntityProperties& objEntityProperties = o->getEntityProperties();
		objEntityProperties.getCurrentAreas().push_back(area);

		spatialPartitionMap[area][objId] = o;
	}

	void EntityContainer::setSpatialPartitionDimension(unsigned int i)
	{
		spatialPartitionDimension = i;
	}

	void EntityContainer::unregisterObject(Entity* object)
	{
		std::string objId = object->getStringId();
		objects.erase(objId);
		inputHandlers.erase(objId);
		if (object->getEntityProperties().isCollidable()) {
			collidableObjects.erase(objId);
		}

		clearObjectFromCurrentPartitions(object);

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

	void EntityContainer::unregisterAllObjects()
	{
		for(auto it = objects.begin(); it != objects.end(); it++) {
			delete it->second;
		}
		objects.clear();
		collidableObjects.clear();
		spatialPartitionMap.clear();
		inputHandlers.clear();
		reinitLayerMap();
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
			if (it->second->getEntityProperties().isCollidable()) {
				collidableObjects.erase(objId);
			}
			delete it->second;
		}

		layeredObjects[layer].clear();
	}

	void EntityContainer::handleObjects(const SDL_Event& event, const GameData* gameData)
	{
		for (auto it = inputHandlers.begin(); it != inputHandlers.end(); it++) {
			it->second->preHandle(gameData);
			it->second->handle(event);
			it->second->postHandle(gameData);
		}
	}

	void EntityContainer::renderObjects(const GameData* data) const
	{
		for (auto it1 = layeredObjects.begin(); it1 != layeredObjects.end(); it1++) {
			for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
				it2->second->preRender(data);
				it2->second->render(data->getRenderData());
				it2->second->postRender(data);
			}
		}
	}

	void EntityContainer::moveObjects(const GameData* data)
	{
		float deltatime = dtMonitor->getDeltaTime();
		CollisionDetector *coldetector = data->getCollisionDetector();

		for (auto it = objects.begin(); it != objects.end(); it++) {
			it->second->preMove(data);
			handlePossibleObjectMovement(it->second);

			EntityProperties& props = it->second->getEntityProperties();
			if (props.isMoving()) {
				if (props.isCollidable()) {
					coldetector->handlePossibleCollisionsFor(it->second, data);
				}
				props.move(deltatime);
				handlePossibleObjectMovement(it->second);
			}

			it->second->postMove(data);
			handlePossibleObjectMovement(it->second);
		}

		clearDeadObjects();
	}

	void EntityContainer::handlePossibleObjectMovement(Entity* entity)
	{
		// TODO(Linus): Maybe this should be replaced by the previous callback function that was
		// injected into the objects entity properties???
		// Multiple calls to this function seems wasteful although it filters nicely with the
		// hasLocationChanged flag.
		if (entity->getEntityProperties().hasLocationChanged()) {
			clearObjectFromCurrentPartitions(entity);
			registerObjectToSpatialPartitions(entity);
			entity->getEntityProperties().setLocationChanged(false);
		}
	}

	size_t EntityContainer::getObjectCount()
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
			clearObjectFromCurrentPartitions(it->second);
			objectsToErase.push_back(objId);
			delete it->second;
		}

		for (auto it = objectsToErase.begin(); it != objectsToErase.end(); it++) {
			objects.erase(*it);
		}
	}

	size_t EntityContainer::getSpatialPartitionCount() const
	{
		return spatialPartitionMap.size();
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
		const EntityProperties::Areas& currentAreas = source->getEntityProperties().getCurrentAreas();
		std::map<float, Entity*> sortedMap;
		EntityShape colliderShape = source->getEntityProperties().getColliderShape();
		float sx = colliderShape.x + (colliderShape.w / 2);
		float sy = colliderShape.y + (colliderShape.h / 2);

		// Itterate the objects and sort them according to distance
		for (auto areaIter = currentAreas.begin(); areaIter != currentAreas.end(); areaIter++) {
			for (auto objectIter = spatialPartitionMap[*areaIter].begin();
					objectIter != spatialPartitionMap[*areaIter].end();
					objectIter++)
			{
				if (!objectIter->second->getEntityProperties().isCollidable()) {
					continue;
				}
				if (*source == *objectIter->second) {
					continue;
				}

				const EntityShape& targetShape = objectIter->second->getEntityProperties().getColliderShape();
				float tx = targetShape.x + (targetShape.w/2);
				float ty = targetShape.y + (targetShape.h/2);

				float distance;
				if (sx == tx) {
					distance = abs(sy - ty);
				} else if (sy == ty) {
					distance = abs(sx - tx);
				} else {
					distance = sqrt(pow(sx - tx, 2) + pow(sy - ty, 2));
				}

				if (sortedMap.find(distance) != sortedMap.end()) {
					if (*sortedMap[distance] == *objectIter->second) {
						continue;
					}
				}

				while (sortedMap.find(distance) != sortedMap.end()) {
					distance += 0.00001f;
				}

				sortedMap[distance] = objectIter->second;
			}
		}

		// Itterate the sorted objects and call the cb function
		for (auto objectIter = sortedMap.begin(); objectIter != sortedMap.end(); objectIter++) {
			func(objectIter->second);
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
		const EntityProperties::Areas& currentAreas = source->getEntityProperties().getCurrentAreas();
		for (auto areaIter = currentAreas.begin(); areaIter != currentAreas.end(); areaIter++) {
			for (auto objectIter = spatialPartitionMap[*areaIter].begin();
					objectIter != spatialPartitionMap[*areaIter].end();
					objectIter++)
			{
				if (objectIter->second->getEntityProperties().isCollidable() && func(objectIter->second)) {
					return objectIter->second;
				}
			}
		}
		return nullptr;
	}
} // namespace flat2d
