#include "EntityImpl.h"
#include "catch.hpp"
#include "../src/EntityContainer.h"
#include "../src/EntityProperties.h"
#include "../src/GameData.h"
#include "../src/CollisionDetector.h"
#include "../src/DeltatimeMonitor.h"
#include "../src/Mixer.h"
#include "../src/MapArea.h"
#include "../src/Camera.h"
#include "../src/RenderData.h"

TEST_CASE( "Object container tests", "[objectcontainer]" )
{
	flat2d::DeltatimeMonitor *dtm = new flat2d::DeltatimeMonitor();
	flat2d::EntityContainer container(dtm);

	SECTION( "Test container destructor", "[objectcontainer]" )
	{
		flat2d::Entity* c = new EntityImpl(100, 100);

		REQUIRE( 0 == container.getObjectCount() );
		container.registerObject(c);
		REQUIRE( 1 == container.getObjectCount() );
		container.registerObject(c);
		REQUIRE( 1 == container.getObjectCount() );
		container.unregisterObject(c);
		REQUIRE( 0 == container.getObjectCount() );

		delete c;
	}


	SECTION( "Test container destructor", "[objectcontainer]" )
	{
		flat2d::Entity* c = new EntityImpl(100, 100);

		REQUIRE( 0 == container.getObjectCount() );

		container.registerObject(c);

		REQUIRE( 1 == container.getObjectCount() );

		container.registerObject(c);

		REQUIRE( 1 == container.getObjectCount() );
	}

	SECTION( "Test unregister", "[objectcontainer]" )
	{
		flat2d::Entity* c = new EntityImpl(100, 100);

		container.unregisterObject(c);

		delete c;
	}

	SECTION( "Test layers", "[objectcontainer]" )
	{
		flat2d::Entity* c1 = new EntityImpl(100, 100);
		flat2d::Entity* c2 = new EntityImpl(100, 100);
		flat2d::Entity* c3 = new EntityImpl(100, 100);

		REQUIRE( 0 == container.getObjectCountFor(0) );

		container.addLayer(0);
		container.addLayer(0);
		container.addLayer(1);

		container.registerObject(c1);

		REQUIRE( 1 == container.getObjectCount() );
		REQUIRE( 1 == container.getObjectCountFor(flat2d::EntityContainer::DEFAULT_LAYER) );

		container.registerObject(c2, 0);
		container.registerObject(c2, 1);

		REQUIRE( 2 == container.getObjectCount() );
		REQUIRE( 1 == container.getObjectCountFor(0) );
		REQUIRE( 0 == container.getObjectCountFor(1) );

		container.registerObject(c3, 1);

		REQUIRE( 1 == container.getObjectCountFor(1) );
		REQUIRE( 3 == container.getObjectCount() );

		container.unregisterObject(c2);

		REQUIRE( 0 == container.getObjectCountFor(0) );
		REQUIRE( 2 == container.getObjectCount() );

		delete c2;
	}

	SECTION( "Test clearing layers", "[objectcontainer]" )
	{
		flat2d::Entity* c1 = new EntityImpl(100, 100);
		flat2d::Entity* c2 = new EntityImpl(100, 100);
		flat2d::Entity* c3 = new EntityImpl(100, 100);
		flat2d::Entity* c4 = new EntityImpl(100, 100);
		flat2d::Entity* c5 = new EntityImpl(100, 100);
		flat2d::Entity* c6 = new EntityImpl(100, 100);

		unsigned int backLayer = 0;
		unsigned int frontLayer = 1;

		container.addLayer(backLayer);
		container.addLayer(frontLayer);

		container.registerObject(c1, frontLayer);
		container.registerObject(c2, frontLayer);
		container.registerObject(c3, frontLayer);
		container.registerObject(c4, backLayer);
		container.registerObject(c5, backLayer);
		container.registerObject(c6, backLayer);

		REQUIRE ( 6 == container.getObjectCount() );
		REQUIRE ( 3 == container.getObjectCountFor(frontLayer) );
		REQUIRE ( 3 == container.getObjectCountFor(backLayer) );

		container.unregisterAllObjectsFor(frontLayer);

		REQUIRE ( 3 == container.getObjectCount() );
		REQUIRE ( 0 == container.getObjectCountFor(frontLayer) );
		REQUIRE ( 3 == container.getObjectCountFor(backLayer) );
	}

	SECTION( "Test auto cleaning", "[objectcontainer]" )
	{
		flat2d::Entity* c1 = new EntityImpl(100, 100);
		EntityImpl* c2 = new EntityImpl(100, 100);

		flat2d::CollisionDetector detector(&container, dtm);
		flat2d::Mixer mixer;
		flat2d::Camera camera(100, 100);
		flat2d::RenderData renderData((SDL_Renderer*) nullptr, &camera);
		flat2d::GameData gameData(&container, &detector, &mixer, &renderData,
				(flat2d::DeltatimeMonitor*) nullptr);

		container.registerObject(c1);
		container.registerObject(c2);

		REQUIRE ( 2 == container.getObjectCount() );

		c2->setDead(true);
		container.moveObjects(&gameData);

		REQUIRE ( 1 == container.getObjectCount() );
	}

	SECTION( "Test RenderArea retrieval", "[objectcontainer]" )
	{
		flat2d::MapArea *area = new flat2d::MapArea(0, 0, 300);
		container.addRenderArea("key", area);
		REQUIRE( container.getRenderArea("key") != nullptr );
		REQUIRE( container.getRenderArea("nokey") == nullptr );
	}

	delete dtm;
}
