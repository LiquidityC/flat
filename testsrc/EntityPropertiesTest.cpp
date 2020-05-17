#include "../src/EntityProperties.h"
#include "catch.hpp"
#include <iostream>

using namespace flat2d;

TEST_CASE("Test move", "[entityprops]")
{
	EntityProperties props(10, 10, 10);
	props.setXvel(10);
	props.setYvel(10);

	props.move(1);

	REQUIRE(props.getXpos() == 20);
	REQUIRE(props.getYpos() == 20);
	REQUIRE(props.hasLocationChanged());

	props.move(0.5);
	REQUIRE(props.getXpos() == 25);
	REQUIRE(props.getYpos() == 25);
	REQUIRE(props.hasLocationChanged());
}

TEST_CASE("Test increment move", "[entityprops]")
{
	EntityProperties props(10, 10, 10);
	REQUIRE(props.getXpos() == 10);
	REQUIRE(props.getYpos() == 10);
	REQUIRE(!props.hasLocationChanged());

	props.incrementXpos(10);
	REQUIRE(props.getXpos() == 20);
	REQUIRE(props.getYpos() == 10);
	REQUIRE(props.hasLocationChanged());

	props.setLocationChanged(false);
	REQUIRE(!props.hasLocationChanged());

	props.incrementYpos(10);
	REQUIRE(props.getXpos() == 20);
	REQUIRE(props.getYpos() == 20);
	REQUIRE(props.hasLocationChanged());

	props.incrementXpos(-10);
	REQUIRE(props.getXpos() == 10);
	REQUIRE(props.getYpos() == 20);
	REQUIRE(props.hasLocationChanged());

	props.setLocationChanged(false);
	REQUIRE(!props.hasLocationChanged());

	props.incrementYpos(-10);
	REQUIRE(props.getXpos() == 10);
	REQUIRE(props.getYpos() == 10);
	REQUIRE(props.hasLocationChanged());
}
