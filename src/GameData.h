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
