#ifndef BUTTON_H_
#define BUTTON_H_

#include "Entity.h"

namespace flat2d
{
	namespace ui
	{
		class Button : public flat2d::Entity
		{
			private:
				std::function<void ()> onClick;
				bool mouseIsOver = false;

			public:
				Button(int x, int y, unsigned int w, unsigned int h, std::function<void ()> click)
					: Entity(x, y, w, h), onClick(click) {
						entityProperties.setCollidable(false);
					}

				~Button() { }

				void setOnClick(std::function<void ()> click);
				void triggerClick();

				virtual void render(const RenderData* data) const;

				virtual void handle(const SDL_Event&);
		};
	}// namespace ui NOLINT(readability/namespace)
}// namespace flat2d

#endif // BUTTON_H_
#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>

namespace flat2d
{
	class Camera
	{
		private:
			int xpos, ypos, width, height, mapWidth, mapHeight;

		public:
			Camera(int w, int h) :
				xpos(0),
				ypos(0),
				width(w),
				height(h),
				mapWidth(1920),
				mapHeight(1080) { }

			int getXpos();
			int getYpos();
			int getWidth();
			int getHeight();

			void setMapDimensions(int, int);

			SDL_Rect getBox();

			void centerOn(int x, int y);

			bool isVisibleOnCamera(const SDL_Rect& box);

			int getScreenXposFor(int x) const;
			int getScreenYposFor(int y) const;
	};
} // namespace flat2d

#endif // CAMERA_H_
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
#ifndef COLLISIONPROPERTY_H_
#define COLLISIONPROPERTY_H_

namespace flat2d
{
	enum CollisionProperty {
		SOLID,
		BOUNCY,
		STICKY,
		ETHERAL
	};
}

#endif // COLLISIONPROPERTY_H_
#ifndef DATABASE_H_
#define DATABASE_H_

#include <sqlite3.h>
#include <string>
#include <vector>

/**
 * @brief Handle SQLite databases
 * @author Linus Probert <linus.probert@gmail.com>
 */
namespace flat2d
{
	namespace db
	{
		class Database {
			public:
				typedef std::vector<std::string> Row;
				typedef std::vector<Row> Result;

			public:
				/**
				 * @brief Constructor
				 * @param filename The database file
				 */
				explicit Database(const std::string& filename);

				/**
				 * @brief destructor
				 */
				~Database();

				/**
				 * @brief Open provided database file
				 * @param filename the path to the file
				 */
				bool open(const std::string& filename);

				/**
				 * @brief Run a query against the database
				 * @return the result of the query
				 */
				Result query(const std::string& query);

				/**
				 * @brief Close the database
				 */
				void close();

				/**
				 * @brief Get table count for given table
				 * @patam table_name The name of the table
				 */
				unsigned int get_table_count(const std::string& table_name);

			private:
				sqlite3* database;
		};
	} // namespace db NOLINT(readability/namespace)
} // namespace flat2d

#endif // DATABASE_H_
#ifndef DELTATIMEMONITOR_H_
#define DELTATIMEMONITOR_H_

namespace flat2d
{
	class DeltatimeMonitor
	{
		private:
			float deltaTime = 1.0;
			int currentTime = 0;
			int oldTime = 0;

		public:
			void updateDeltaTime();
			float getDeltaTime() const;
	};
} // namespace flat2d

#endif // DELTATIMEMONITOR_H_
#ifndef ENTITY_H_
#define ENTITY_H_

#include <SDL2/SDL.h>
#include <string>
#include <sstream>

#include "EntityProperties.h"
#include "UID.h"

namespace flat2d
{
	class Camera;
	class RenderData;
	class GameData;

	class Entity
	{
		private:
			size_t id;
			bool fixedPosition = false;
			bool inputHandler = false;

		protected:
			EntityProperties entityProperties;
			bool dead = false;
			SDL_Texture *texture = nullptr;
			SDL_Rect clip;

		public:
			Entity(int x, int y, int w, int h) :
				entityProperties(x, y, w, h),
				dead(false) {
					id = UID::generate();

					entityProperties.setColliderShape({ 0, 0, w, h });
					clip = { 0, 0, w, h };
				}

			virtual ~Entity() { }

			/* Operators */
			virtual bool operator==(const Entity& o) const {
				return id == o.id;
			}

			virtual bool operator!=(const Entity& o) const {
				return id != o.id;
			}

			virtual Entity& operator=(const Entity& o) {
				id = o.id;
				return *this;
			}

			virtual std::string getStringId() const {
				std::string number;
				std::stringstream ss;
				ss << id;
				ss >> number;
				return number;
			}

			virtual int getType() const {
				return -1;
			}

			/* Own methods */
			void setClip(SDL_Rect&);
			void setDead(bool isDead);
			bool isFixedPosition();
			void setFixedPosition(bool isFixed);
			bool isInputHandler();
			void setInputHandler(bool inputHandler);

			/* Implemented override methods */
			virtual bool isDead() const;
			virtual void render(const RenderData*) const;
			virtual const SDL_Texture* getTexture() const;
			virtual void setTexture(SDL_Texture* texture);
			virtual EntityProperties& getEntityProperties();
			virtual const EntityProperties& getEntityProperties() const;
			virtual bool onCollision(Entity *collider, const GameData*);

			/* Empty methods */
			virtual void init(const GameData*) { }
			virtual void preHandle(const GameData*) { }
			virtual void handle(const SDL_Event& event) { }
			virtual void postHandle(const GameData*) { }
			virtual void preRender(const GameData*) { }
			virtual void postRender(const GameData*) { }
			virtual void preMove(const GameData*) { }
			virtual void postMove(const GameData*) { }
	};
} // namespace flat2d

#endif // ENTITY_H_
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
#ifndef ENTITYPROPERTIES_H_
#define ENTITYPROPERTIES_H_

#include <SDL2/SDL.h>
#include <vector>
#include <functional>

#include "Square.h"
#include "MapArea.h"
#include "EntityShape.h"
#include "CollisionProperty.h"

namespace flat2d
{
	class EntityContainer;

	class EntityProperties : public Square
	{
		public:
			typedef std::vector<MapArea> Areas;

		private:
			float xvel = 0.0f;
			float yvel = 0.0f;

			bool collidable = false;
			bool locationChanged = false;

			CollisionProperty collisionProperty = CollisionProperty::SOLID;

			EntityShape colliderShape = { 0, 0, 0, 0 };
			Areas currentAreas;

			EntityShape getCustomVelocityColliderShape(float dx, float dy) const;

		public:
			EntityProperties(int x, int y, int dim) : Square(x, y, dim) { }
			EntityProperties(int x, int y, int w, int h) : Square(x, y, w, h) { }

			SDL_Rect getBoundingBox() const;

			EntityShape getColliderShape() const;
			void setColliderShape(EntityShape shape);

			EntityShape getVelocityColliderShape(float deltatime) const;
			EntityShape getXVelocityColliderShape(float deltatime) const;
			EntityShape getYVelocityColliderShape(float deltatime) const;

			void setCollidable(bool isCollidable);
			bool isCollidable() const;

			void incrementXpos(int x);
			void setXpos(int pos);
			int getXpos() const;
			void incrementYpos(int y);
			void setYpos(int pos);
			int getYpos() const;
			int getWidth() const;
			int getHeight() const;
			void setXvel(float xvel);
			float getXvel() const;
			void setYvel(float yvel);
			float getYvel() const;

			void setCollisionProperty(CollisionProperty);
			CollisionProperty getCollisionProperty() const;

			int getColliderLeftOffset() const;
			int getColliderRightOffset() const;
			int getColliderTopOffset() const;
			int getColliderBottomOffset() const;

			bool isMoving() const;

			void move(float deltatime);

			bool containsPoint(int, int) const;

			Areas& getCurrentAreas();
			const Areas& getCurrentAreas() const;

			void setLocationChanged(bool changed);
			bool hasLocationChanged() const;
	};
} // namespace flat2d

#endif // ENTITYPROPERTIES_H_
#ifndef ENTITYSHAPE_H_
#define ENTITYSHAPE_H_

namespace flat2d
{
	typedef struct {
		int x;
		int y;
		int w;
		int h;
	} EntityShape;
}


#endif // ENTITYSHAPE_H_
#ifndef FLATBUILDER_H_
#define FLATBUILDER_H_

#include <string>

namespace flat2d
{
	class EntityContainer;
	class CollisionDetector;
	class Window;
	class Camera;
	class GameData;
	class RenderData;
	class Mixer;
	class DeltatimeMonitor;
	class GameControllerContainer;
	class GameEngine;

	class FlatBuilder
	{
		private:
			Window *window = nullptr;
			RenderData *renderData = nullptr;
			GameData *gameData = nullptr;
			CollisionDetector *collisionDetector = nullptr;
			EntityContainer *entityContainer = nullptr;
			Camera *camera = nullptr;
			Mixer *mixer = nullptr;
			DeltatimeMonitor *deltatimeMonitor = nullptr;
			GameControllerContainer *controllerContainer = nullptr;
			GameEngine *gameEngine = nullptr;

		public:
			~FlatBuilder();

			bool initSDL(std::string, int, int);
			bool initContainers();

			GameData* getGameData() const;
			GameEngine* getGameEngine() const;
	};
} // namespace flat2d

#endif // FLATBUILDER_H_
#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <SDL2/SDL.h>

namespace flat2d
{
	class GameControllerContainer;

	class GameController
	{
		private:
			SDL_GameController *controller = nullptr;

		public:
			explicit GameController(int index);

			~GameController();

			SDL_GameController* getSDLController();
	};
} // namespace flat2d

#endif // GAMECONTROLLER_H_
#ifndef GAMECONTROLLERCONTAINER_H_
#define GAMECONTROLLERCONTAINER_H_

#include <vector>
#include "GameController.h"

namespace flat2d
{
	class GameControllerContainer
	{
		private:
			std::vector<GameController*> controllers;

			int loadControllers();

		public:
			GameControllerContainer();
			virtual ~GameControllerContainer();

			bool hasControllers();
			GameController* getController(int index);
	};
} // namespace flat2d

#endif // GAMECONTROLLERCONTAINER_H_
#ifndef GAMEDATA_H_
#define GAMEDATA_H_

namespace flat2d
{
	class EntityContainer;
	class CollisionDetector;
	class RenderData;
	class Mixer;
	class Camera;
	class DeltatimeMonitor;

	class GameData
	{
		private:
			EntityContainer *entityContainer;
			CollisionDetector *collisionDetector;
			Mixer *mixer;
			RenderData *renderData;
			DeltatimeMonitor *deltatimeMonitor;
			void *customGameData = nullptr;

		public:
			GameData(EntityContainer *obc, CollisionDetector *cd,
					Mixer *m, RenderData *rd, DeltatimeMonitor *dtm) :
				entityContainer(obc),
				collisionDetector(cd),
				mixer(m),
				renderData(rd),
				deltatimeMonitor(dtm) { }

			EntityContainer* getEntityContainer() const { return entityContainer; }
			CollisionDetector* getCollisionDetector() const { return collisionDetector; }
			Mixer* getMixer() const { return mixer; }

			void setCustomGameData(void *customGameData) { this->customGameData = customGameData; }
			void* getCustomGameData() const { return customGameData; }
			DeltatimeMonitor* getDeltatimeMonitor() const { return deltatimeMonitor; }
			RenderData* getRenderData() const { return renderData; }
	};
} // namespace flat2d


#endif // GAMEDATA_H_
#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

namespace flat2d
{
	class GameData;
	class RenderData;
	class EntityContainer;
	class VirtualGameStateController;

	class GameEngine
	{
		private:
			GameData *gameData;

			int screenWidth = 800;
			int screenHeight = 600;
			int screenFps = 60;
			int screenTicksPerFrame = 1000/60;

			GameEngine(const GameEngine&); // Don't implement
			void operator=(const GameEngine&); // Don't implement

		public:
			explicit GameEngine(GameData *data) : gameData(data) { }
			~GameEngine() { }

			void init(int screenWidth, int screenHeight, int fps);

			void run(VirtualGameStateController *gameStateController) const;
	};
} // namespace flat2d

#endif // GAMEENGINE_H_
#ifndef MAPAREA_H_
#define MAPAREA_H_

#include "Square.h"

namespace flat2d
{
	class MapArea : public Square
	{
		friend class EntityContainer;

		public:
			MapArea(int px, int py, int dim) : Square(px, py, dim) { }
			MapArea(int px, int py, int pw, int ph) : Square(px, py, pw, ph) { }

			SDL_Rect asSDLRect() const {
				return { x, y, w, h };
			}
	};
} // namespace flat2d

#endif // MAPAREA_H_
#ifndef MEDIAUTIL_H_
#define MEDIAUTIL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

namespace flat2d
{
	class MediaUtil
	{
		public:
			static SDL_Texture* loadTexture(std::string, SDL_Renderer*);

			static TTF_Font* loadFont(std::string, size_t size = 12);
			static SDL_Texture* createTextTexture(std::string, TTF_Font*, SDL_Color, SDL_Renderer*);
			static SDL_Texture* loadTextTexture(std::string, std::string, SDL_Renderer*);
	};
} // namespace flat2d

#endif // MEDIAUTIL_H_
#ifndef MIXER_H_
#define MIXER_H_

#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>

namespace flat2d
{
	class Mixer
	{
		private:
			std::map<int, Mix_Chunk*> effects;
			std::map<int, Mix_Music*> music;

		public:
			~Mixer();

			void clearAllSound();

			bool loadEffect(int, std::string);

			void playEffect(int effect);

			bool loadMusic(int, std::string);

			void playMusic(int music);

			bool playingMusic();

			void pauseMusic();

			bool pausedMusic();

			void unpauseMusic();

			void stopMusic();
	};
} // namespace flat2d

#endif // MIXER_H_
#ifndef RENDERDATA_H_
#define RENDERDATA_H_

#include <SDL2/SDL.h>

namespace flat2d
{
	class Camera;
	class EntityContainer;
	class CollisionDetector;

	class RenderData
	{
		private:
			SDL_Renderer *renderer;
			Camera *camera;
			void *customData = nullptr;

		public:
			RenderData(SDL_Renderer *ren, Camera *cam) :
				renderer(ren),
				camera(cam) { }

			SDL_Renderer* getRenderer() const { return renderer; }
			Camera* getCamera() const { return camera; }
			void* getCustomData() { return customData; }
	};
} // namespace flat2d

#endif // RENDERDATA_H_
#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#include <atomic>

namespace flat2d
{
	class SpinLock
	{
		private:
			std::atomic_flag lck = ATOMIC_FLAG_INIT;

		public:
			void lock()
			{
				while (lck.test_and_set(std::memory_order_acquire)) { }
			}

			void unlock()
			{
				lck.clear(std::memory_order_release);
			}
	};
} // namespace flat2d

#endif // SPINLOCK_H_
#ifndef SQUARE_H_
#define SQUARE_H_

namespace flat2d
{
	class Square
	{
		protected:
			int x, y, w, h;

		public:
			Square(int px, int py, int dim) : x(px), y(py), w(dim), h(dim) { }
			Square(int px, int py, int pw, int ph) : x(px), y(py), w(pw), h(ph) { }

			bool containsPoint(int, int) const;

			bool operator<(const Square&) const;
			bool operator==(const Square&) const;
			bool operator!=(const Square&) const;
	};
} // namespace flat2d
#endif // SQUARE_H_
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Square.h"

namespace flat2d
{
	class Texture : public Square
	{
		private:
			SDL_Texture *texture = nullptr;
			TTF_Font *font = nullptr;

			void freeTexture();
			void freeFont();

		public:
			Texture(int xpos, int ypos) : Square(xpos, ypos, 0, 0) { }
			virtual ~Texture();

			void render(SDL_Renderer *renderer) const;

			bool loadFromFile(std::string path, SDL_Renderer *renderer);
			bool loadFont(std::string path, size_t size);
			bool loadFromRenderedText(std::string text, SDL_Color color, SDL_Renderer *renderer);
	};
} // namespace flat2d

#endif // TEXTURE_H_
#ifndef TIMER_H_
#define TIMER_H_

namespace flat2d
{
	class Timer
	{
		public:
			Timer();

			void start();
			void stop();
			void pause();
			void unpause();

			Uint32 getTicks() const;

			bool isStarted() const;
			bool isPaused() const;

		private:
			Uint32 startTicks, pausedTicks;

			bool paused, started;
	};
} // namespace flat2d

#endif // TIMER_H_
#ifndef UID_H_
#define UID_H_

#include <cstddef>
#include "SpinLock.h"

namespace flat2d
{
	class UID
	{
		private:
			static size_t uid;
			static SpinLock lock;

		public:
			static size_t generate();
	};
} // namespace flat2d

#endif // UID_H_
#ifndef VIRTUALGAMESTATECONTROLLER_H_
#define	VIRTUALGAMESTATECONTROLLER_H_

namespace flat2d
{
	class GameData;

	class VirtualGameStateController
	{
		public:
			virtual ~VirtualGameStateController() { }
			virtual void handle(const SDL_Event& event) = 0;
			virtual bool gameStateCheck(GameData *gameData) = 0;
	};
} // namespace flat2d

#endif // VIRTUALGAMESTATECONTROLLER_H_
#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL2/SDL.h>
#include <cstdio>
#include <string>

namespace flat2d
{
	class Window
	{
		private:
			std::string title;
			unsigned int width, height;

			SDL_Window* window;
			SDL_Renderer* renderer;

		public:
			Window(std::string t, unsigned int w, unsigned int h) : title(t), width(w), height(h) { }

			~Window() {
				SDL_DestroyWindow(window);

				window = nullptr;
				renderer = nullptr;
			}

			bool init();

			SDL_Window* getWindow() { return window; }
			SDL_Renderer* getRenderer() { return renderer; }
	};
} // namespace flat2d

#endif // WINDOW_H_
