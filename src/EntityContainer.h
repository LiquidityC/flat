#ifndef ENTITYCONTAINER_H_
#define ENTITYCONTAINER_H_

#include <SDL2/SDL.h>
#include <iostream>
#include <map>
#include <string>
#include <functional>

#include "MapArea.h"
#include "EntityShape.h"

namespace flat2d
{
	// Forward declarations
	class Entity;
	class GameData;
	class RenderData;
	class DeltatimeMonitor;
	class EntityProperties;

	typedef int Layer;
	typedef std::map<std::string, Entity*> ObjectList;
	typedef std::map<Layer, ObjectList> LayerMap;
	typedef std::map<MapArea, ObjectList> SpatialPartitionMap;

	class EntityContainer
	{
		private:
			// TODO(Linus): Maybe make this editable in the future?
			const int spatialPartitionExpansion = 0;
			unsigned int spatialPartitionDimension = 100;

			DeltatimeMonitor *dtMonitor = nullptr;

			ObjectList objects;
			ObjectList collidableObjects;
			ObjectList inputHandlers;
			LayerMap layeredObjects;
			SpatialPartitionMap spatialPartitionMap;

			typedef std::function<bool (Entity*)> EntityProcessor;
			typedef std::function<void (Entity*)> EntityIter;

		private:
			EntityContainer(const EntityContainer&); // Don't implement
			void operator=(const EntityContainer&); // Don't implement

			void clearDeadObjects();
			void registerObjectToSpatialPartitions(Entity *entity);
			void addObjectToSpatialPartitionFor(Entity *entity, int x, int y);
			void clearObjectFromCurrentPartitions(Entity *entity);
			void clearObjectFromUnattachedPartitions(Entity *entity);
			EntityShape createBoundingBoxFor(const EntityProperties& props) const;

			void handlePossibleObjectMovement(Entity* entity);

			void reinitLayerMap();

		public:
			static const int DEFAULT_LAYER = -1;

			explicit EntityContainer(DeltatimeMonitor* dtm) : dtMonitor(dtm) {
				reinitLayerMap();
			}

			~EntityContainer();

			void addLayer(unsigned int);

			void registerObject(Entity*, Layer = DEFAULT_LAYER);
			void unregisterObject(Entity *entity);

			void unregisterAllObjects();
			void unregisterAllObjectsFor(Layer);

			size_t getObjectCount();
			size_t getObjectCountFor(Layer);

			size_t getSpatialPartitionCount() const;

			void handleObjects(const SDL_Event&, const GameData*);

			void moveObjects(const GameData*);

			void renderObjects(const GameData*) const;

			void setSpatialPartitionDimension(unsigned int);

			void iterateAllMovingObjects(EntityIter) const;
			void iterateCollidablesFor(const Entity*, EntityIter);
			Entity* checkAllCollidableObjects(EntityProcessor) const;
			Entity* checkAllObjects(EntityProcessor) const;
			Entity* checkCollidablesFor(const Entity*, EntityProcessor);
	};
} // namespace flat2d

#endif // ENTITYCONTAINER_H_
