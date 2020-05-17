#include "../src/DeltatimeMonitor.h"
#include "catch.hpp"

TEST_CASE("DeltatimeMonitorTest", "[deltatime]")
{
	flat2d::DeltatimeMonitor dtMonitor;

	SECTION("InitTest", "[deltatime]")
	{
		REQUIRE(dtMonitor.getDeltaTime() == 1);
	}
}
