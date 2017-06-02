#include <vector>
#include "catch.hpp"
#include "EntityImpl.h"
#include "../src/QuadTree.h"
#include "../src/Square.h"

TEST_CASE( "QuadTreeTest", "[animation]" )
{
	flat2d::Square bounds(0, 0, 400, 400);
	flat2d::QuadTree *tree = new flat2d::QuadTree(bounds, nullptr);

	SECTION( "Test split one level", "[QuadTree]" )
	{
		REQUIRE( tree->getDepth() == 0 );

		std::vector<flat2d::Entity*> objects;
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(10, 10));
		}
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(210, 10));
		}
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(10, 210));
		}

		for (auto it = objects.begin(); it != objects.end(); ++it) {
			tree->insert(*it);
		}

		REQUIRE( tree->getDepth() == 1 );

		tree->clear();
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			delete *it;
		}

		REQUIRE( tree->getDepth() == 0 );
	}

	SECTION( "Test split two levels", "[QuadTree]" )
	{
		REQUIRE( tree->getDepth() == 0 );

		std::vector<flat2d::Entity*> objects;
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(10, 10));
		}
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(110, 10));
		}
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(10, 110));
		}

		for (auto it = objects.begin(); it != objects.end(); ++it) {
			tree->insert(*it);
		}

		REQUIRE( tree->getDepth() == 2 );

		tree->clear();
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			delete *it;
		}

		REQUIRE( tree->getDepth() == 0 );
	}

	SECTION( "Test retrieval", "[QuadTree]" )
	{
		CHECK( tree->getDepth() == 0 );

		std::vector<flat2d::Entity*> objects;
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(10, 10));
		}
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(210, 10));
		}
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(10, 210));
		}
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(10, 195));
		}

		for (auto it = objects.begin(); it != objects.end(); ++it) {
			tree->insert(*it);
		}

		CHECK( tree->getDepth() == 1 );

		flat2d::Entity *collider = new EntityImpl(10, 10);
		std::vector<flat2d::Entity*> collidables;
		tree->retrieve(&collidables, collider);
		delete collider;

		REQUIRE ( collidables.size() == 10 );

		tree->clear();
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			delete *it;
		}

		CHECK( tree->getDepth() == 0 );
	}

	SECTION( "Test moving objects", "[QuadTree]" )
	{
		CHECK( tree->getDepth() == 0 );

		// Create 20 Entity objects
		std::vector<flat2d::Entity*> objects;
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(10, 10));
		}
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(210, 10));
		}
		for (int i = 0; i < 5; i++) {
			objects.push_back(new EntityImpl(10, 210));
		}

		// Insert the objects
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			tree->insert(*it);
		}

		// Confirm count
		CHECK( tree->count() == objects.size() );

		// Move the first 5
		for (size_t i = 0; i < 5; ++i) {
			objects[i]->getEntityProperties().incrementYpos(15);
		}

		// Purge
		std::vector<flat2d::Entity*> purgedEntities;
		tree->purge(&purgedEntities);

		// Confirm 5 purged, 0 depth and 10 remaining
		CHECK( purgedEntities.size() == 5 );
		CHECK( tree->getDepth() == 0 );
		CHECK( tree->count() == 10 );

		tree->clear();
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			delete *it;
		}

		CHECK( tree->getDepth() == 0 );
	}

	delete tree;
}
