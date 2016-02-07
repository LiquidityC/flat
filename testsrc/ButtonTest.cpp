#include "catch.hpp"
#include "EntityImpl.h"
#include "../src/Button.h"

TEST_CASE ("ButtonTests", "[click]")
{
	bool clicked = false;
	flat2d::ui::Button button(10, 10, 30, 10, 
			[&]() {
				clicked = true;
			});

	REQUIRE( !clicked );

	button.triggerClick();

	REQUIRE( clicked );
}
