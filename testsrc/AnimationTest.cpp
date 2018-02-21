#include "catch.hpp"
#include "../src/Animation.h"

TEST_CASE( "AnimationTest", "[animation]" )
{
	flat2d::Clips clips = {
		{ 0, 0, 5, 5 },
		{ 5, 0, 5, 5 }
	};

	flat2d::Animation animation(clips, 0);

	SECTION( "StartStopTest", "[animation]" )
	{
		REQUIRE( !animation.isRunning() );
		animation.start();
		REQUIRE( animation.isRunning() );
		animation.stop();
		REQUIRE( !animation.isRunning() );
	}

	SECTION( "RunTest", "[animation]" )
	{
		const SDL_Rect *clip = animation.run();
		REQUIRE(clip->x == 0);
		REQUIRE(clip->y == 0);
		REQUIRE(clip->w == 5);
		REQUIRE(clip->h == 5);

		animation.start();
		clip = animation.run();
		REQUIRE(clip->x == 5);
		REQUIRE(clip->y == 0);
		REQUIRE(clip->w == 5);
		REQUIRE(clip->h == 5);

		animation.stop();
		clip = animation.run();
		REQUIRE(clip->x == 0);
		REQUIRE(clip->y == 0);
		REQUIRE(clip->w == 5);
		REQUIRE(clip->h == 5);
	}
}
