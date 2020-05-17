#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "Entity.h"
#include "Square.h"
#include <vector>

namespace flat2d {
	/**
	 * A QuadTree for organizing collidable Entity objects in the world. Used be
	 * the EntityContainer and CollisionDetector.
	 *
	 * @author Linus Probert <linus.probert@gmail.com>
	 */
	class QuadTree
	{
	  private:
		const unsigned int MAX_OBJECTS = 10;

		Square bounds;
		int depth;
		std::vector<Entity*> objects;
		std::vector<QuadTree*> nodes;

		void split();

		int getNodeIndexFor(const Entity* e) const;

	  public:
		/**
		 * Create a QuadTree with the specified bounds represented as a square
		 * @param b The nodes bounds
		 * @param d The depth (only used internally in the tree)
		 */
		explicit QuadTree(Square b, unsigned int d = 0)
		  : bounds(b)
		  , depth(d)
		{}

		/**
		 * Destroy the Tree. This doesn't delete contained Entity objects.
		 */
		~QuadTree() { clear(); }

		/**
		 * Insert an entity into the QuadTree
		 * @param e An entity pointer to insert
		 */
		void insert(Entity* e);

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
		void retrieve(std::vector<Entity*>* returnEntities,
		              const Entity*) const;
	};
} // namespace flat2d

#endif // QUADTREE_H_
