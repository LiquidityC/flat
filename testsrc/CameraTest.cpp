#include <SDL2/SDL.h>

#include "catch.hpp"
#include "../src/Camera.h"

TEST_CASE( "CameraTests", "[camera]" )
{
	flat2d::Camera *camera = new flat2d::Camera(200, 100);
	camera->setMapDimensions(1000, 100);

	SECTION( "Map dimensions", "[camera][map]" )
	{
		camera->setMapDimensions(1000, 100);
		REQUIRE( camera->getMapWidth() == 1000 );
		REQUIRE( camera->getMapHeight() == 100 );
		camera->setMapDimensions(2000, 200);
		REQUIRE( camera->getMapWidth() == 2000 );
		REQUIRE( camera->getMapHeight() == 200 );
	}

	SECTION( "Camera dimensions", "[camera]" )
	{
		REQUIRE( camera->getHeight() == 100 );
		REQUIRE( camera->getWidth() == 200 );
	}

	SECTION( "Is visible on camera", "[camera]" )
	{
		SDL_Rect box = { 0, 0, 50, 50 };
		REQUIRE( camera->isVisibleOnCamera(box) );
		box.x = -25;
		box.y = -25;
		REQUIRE( camera->isVisibleOnCamera(box) );
		box.x = 201;
		box.y = 101;
		REQUIRE( !camera->isVisibleOnCamera(box) );
		box.x = 199;
		box.y = 99;
		REQUIRE( camera->isVisibleOnCamera(box) );
	}

	SECTION( "Center camera", "[camera]" )
	{
		camera->setMapDimensions(1000, 1000);
		camera->centerOn(500, 500);
		REQUIRE(camera->getXpos() == 400);
		REQUIRE(camera->getYpos() == 450);

		camera->centerOn(1000, 1000);
		REQUIRE(camera->getXpos() == 800);
		REQUIRE(camera->getYpos() == 900);

		camera->centerOn(10, 10);
		REQUIRE(camera->getXpos() == 0);
		REQUIRE(camera->getYpos() == 0);
	}
}
