#include "../src/QuadTree.h"
#include "../src/Square.h"
#include "EntityImpl.h"
#include "catch.hpp"
#include <vector>

TEST_CASE("QuadTreeTest", "[animation]")
{
	flat2d::Square bounds(0, 0, 400, 400);
	flat2d::QuadTree* tree = new flat2d::QuadTree(bounds);

	SECTION("Test split one level", "[QuadTree]")
	{
		REQUIRE(tree->getDepth() == 0);

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

		REQUIRE(tree->getDepth() == 1);

		tree->clear();
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			delete *it;
		}

		REQUIRE(tree->getDepth() == 0);
	}

	SECTION("Test split two levels", "[QuadTree]")
	{
		REQUIRE(tree->getDepth() == 0);

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

		REQUIRE(tree->getDepth() == 2);

		tree->clear();
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			delete *it;
		}

		REQUIRE(tree->getDepth() == 0);
	}

	SECTION("Test retrieval", "[QuadTree]")
	{
		CHECK(tree->getDepth() == 0);

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

		CHECK(tree->getDepth() == 1);

		flat2d::Entity* collider = new EntityImpl(10, 10);
		std::vector<flat2d::Entity*> collidables;
		tree->retrieve(&collidables, collider);
		delete collider;

		REQUIRE(collidables.size() == 10);

		tree->clear();
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			delete *it;
		}

		CHECK(tree->getDepth() == 0);
	}

	delete tree;
}
