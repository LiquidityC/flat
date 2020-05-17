#ifndef GAMEDATA_H_
#define GAMEDATA_H_

namespace flat2d {
	class EntityContainer;
	class CollisionDetector;
	class RenderData;
	class Mixer;
	class Camera;
	class DeltatimeMonitor;

	/**
	 * The GameData object acts as a container for the game and it's state.
	 * From it you can reach the RenderData, EntityContainer, CollisionDetector
	 * Mixer, DeltatimeMonitor and your custom game data object.
	 */
	class GameData
	{
	  private:
		EntityContainer* entityContainer;
		CollisionDetector* collisionDetector;
		Mixer* mixer;
		RenderData* renderData;
		DeltatimeMonitor* deltatimeMonitor;
		void* customGameData = nullptr;

	  public:
		GameData(EntityContainer* obc,
		         CollisionDetector* cd,
		         Mixer* m,
		         RenderData* rd,
		         DeltatimeMonitor* dtm)
		  : entityContainer(obc)
		  , collisionDetector(cd)
		  , mixer(m)
		  , renderData(rd)
		  , deltatimeMonitor(dtm)
		{}

		/**
		 * Get the entity container object pointer
		 *
		 * @return A EntityContainer pointer
		 */
		EntityContainer* getEntityContainer() const { return entityContainer; }

		/**
		 * Get the CollisionDetector object pointer
		 *
		 * @return A CollisionDetector pointer
		 */
		CollisionDetector* getCollisionDetector() const
		{
			return collisionDetector;
		}

		/**
		 * Get the Mixer object pointer
		 *
		 * @return A Mixer pointer
		 */
		Mixer* getMixer() const { return mixer; }

		/**
		 * Add a void pointer to custom data you wan't stored with the GameData.
		 * This data will not be destroyed with the GameData object so you have
		 * to dispose of it yourself.
		 */
		void setCustomGameData(void* customGameData)
		{
			this->customGameData = customGameData;
		}

		/**
		 * Get your custom data object from the GameData object
		 *
		 * @return A void pointer to your custom object
		 */
		void* getCustomGameData() const { return customGameData; }

		/**
		 * Get the DeltatimeMonitor object pointer
		 *
		 * @return A DeltatimeMonitor pointer
		 */
		DeltatimeMonitor* getDeltatimeMonitor() const
		{
			return deltatimeMonitor;
		}

		/**
		 * Get the RenderData object pointer
		 *
		 * @return A RenderData pointer
		 */
		RenderData* getRenderData() const { return renderData; }
	};
} // namespace flat2d

#endif // GAMEDATA_H_
