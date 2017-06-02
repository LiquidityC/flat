#ifndef QUADTREE_H_
#define	QUADTREE_H_

#include "Entity.h"
#include "Square.h"
#include "DeltatimeMonitor.h"
#include <vector>
#include <iostream>

namespace flat2d
{
	class RenderData;

	/**
	 * A QuadTree for organizing collidable Entity objects in the world. Used be the
	 * EntityContainer and CollisionDetector.
	 *
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class QuadTree
	{
		private:
			const unsigned int MAX_OBJECTS = 10;

			enum Position {
				CENTER,
				LEFT,
				RIGHT,
				TOP,
				BOTTOM,
				TOP_LEFT,
				TOP_RIGHT,
				BOTTOM_LEFT,
				BOTTOM_RIGHT,
				NO_POSITION
			};

			Square bounds;
			DeltatimeMonitor *deltatimeMonitor;
			int depth;
			std::vector<Entity*> objects;
			std::vector<QuadTree*> nodes;

			void split();

			/* Reverse a split */
			void unsplit();

			void gather(std::vector<Entity*> *collection);

			bool splitAvailable() const;

			typedef std::function<void (QuadTree *e)> DistributionCallback;
			void distribute(const Position&, DistributionCallback);

			Position getNodePositionFor(const Entity *e) const;
			void insertInto(Entity* e, Position p);

		public:
			/**
			 * Create a QuadTree with the specified bounds represented as a square
			 * @param b The nodes bounds
			 * @param d The depth (only used internally in the tree)
			 */
			explicit QuadTree(const Square& b, DeltatimeMonitor *dtm,  unsigned int d = 0) :
				bounds(b), deltatimeMonitor(dtm), depth(d) { }

			/**
			 * Destroy the Tree. This doesn't delete contained Entity objects.
			 */
			~QuadTree() {
				clear();
			}

			/**
			 * Insert an entity into the QuadTree
			 * @param e An entity pointer to insert
			 */
			void insert(Entity *e);

			/**
			 * Clear the QuadTree
			 */
			void clear();

			/**
			 * Returns the depth of the tree. Mostly intended for testing and
			 * other debugging information.
			 * @return The depth of the tree
			 */
			int getDepth() const;

			/**
			 * Return a list of possible collision Entity objects.
			 * @param returnEntities The prodced list
			 * @param entity The Entity to collide with
			 */
			void retrieve(std::vector<Entity*> *returnEntities, const Entity*);

			/**
			 * Render the nodes in the QuadTree, can be used to debug your game.
			 * @param rednerData RenderData object
			 */
			void render(const RenderData *renderData) const;

			/**
			 * Clear Entity objects with locationChanged == true from the tree
			 * and add the pointers to the provided list.
			 * @param purgedEntities a pointer to the list to populate
			 */
			void purge(std::vector<Entity*> *purgedEntities);

			/**
			 * Count the entities in the tree
			 * @return the Entity count
			 */
			unsigned int count() const;
	};
} // namespace flat2d

#endif // QUADTREE_H_
