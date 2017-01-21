#include "catch.hpp"
#include "../src/DeltatimeMonitor.h"

TEST_CASE( "DeltatimeMonitorTest", "[deltatime]" )
{
	flat2d::DeltatimeMonitor dtMonitor;

	SECTION( "InitTest", "[deltatime]" )
	{
		REQUIRE( dtMonitor.getDeltaTime() == 1 );
	}
}
