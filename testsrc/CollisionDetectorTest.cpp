#include "../src/CollisionDetector.h"
#include "../src/DeltatimeMonitor.h"
#include "../src/EntityContainer.h"
#include "../src/EntityProperties.h"
#include "../src/EntityShape.h"
#include "EntityImpl.h"
#include "catch.hpp"

TEST_CASE("CollisionDetectorTests", "[collision]")
{
	flat2d::DeltatimeMonitor* dtm;
	flat2d::EntityContainer* container;
	flat2d::CollisionDetector* detector;

	flat2d::Entity* c1;
	flat2d::Entity* c2;
	flat2d::Entity* c3;
	flat2d::Entity* c4;
	flat2d::Entity* c5;

	dtm = new flat2d::DeltatimeMonitor();
	container = new flat2d::EntityContainer(dtm);
	detector = new flat2d::CollisionDetector(container, dtm);

	c1 = new EntityImpl(100, 100);
	c2 = new EntityImpl(200, 100);
	c3 = new EntityImpl(200, 200);
	c4 = new EntityImpl(105, 105);
	c5 = new EntityImpl(115, 115);

	container->registerObject(c1);
	container->registerObject(c2);
	container->registerObject(c3);
	container->registerObject(c4);

	SECTION("Collision detection", "[collisions]")
	{
		REQUIRE(detector->AABB(c1->getEntityProperties().getColliderShape(),
		                       c4->getEntityProperties().getColliderShape()));
		REQUIRE(!detector->AABB(c1->getEntityProperties().getColliderShape(),
		                        c2->getEntityProperties().getColliderShape()));
	}

	SECTION("Multi collisions", "[hide][collisions]")
	{
		// TODO(Linus): Need some new tests here after rewrite
		//
		// flat2d::Entity* o = detector->checkForCollisions( c4 );
		// REQUIRE( o );
		// REQUIRE( *c1 == *o );

		// REQUIRE( !detector->checkForCollisions( c2 ) );
	}

	SECTION("Colliders", "[collisions]")
	{
		flat2d::EntityShape colliderBox = { 0, 0, 1, 1 };
		c1->getEntityProperties().setColliderShape(colliderBox);

		REQUIRE(!detector->AABB(c1->getEntityProperties().getColliderShape(),
		                        c4->getEntityProperties().getColliderShape()));
	}

	SECTION("Moving object collision", "[collisions]")
	{
		flat2d::EntityProperties& props = c1->getEntityProperties();
		props.setXvel(10);
		props.setYvel(0);

		float normaly, normalx;
		float result = detector->sweptAABB(&c1->getEntityProperties(),
		                                   &c5->getEntityProperties(),
		                                   &normaly,
		                                   &normalx);
		REQUIRE(result > 0.0f);
		REQUIRE(result < 1.0f);
	}

	delete detector;
	delete container;
	delete dtm;
}
