cmake_minimum_required (VERSION 2.8.0)

SET(CMAKE_COLOR_MAKEFILE ON)

project (flat CXX)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

set (HEADERS src/flat.h src/Button.h src/Camera.h src/CollisionDetector.h src/CollisionProperty.h src/Database.h
	src/DeltatimeMonitor.h src/Entity.h src/EntityContainer.h src/EntityProperties.h src/EntityShape.h
	src/FlatBuilder.h src/GameController.h src/GameControllerContainer.h src/GameData.h
	src/GameEngine.h src/MapArea.h src/MediaUtil.h src/Mixer.h src/RenderData.h src/SpinLock.h
	src/Square.h src/Texture.h src/Timer.h src/UID.h src/VirtualGameStateController.h src/Window.h)

add_executable(test_flat src/Button.cpp src/Camera.cpp src/CollisionDetector.cpp src/Database.cpp
	src/DeltatimeMonitor.cpp src/Entity.cpp src/EntityContainer.cpp src/EntityProperties.cpp
	src/FlatBuilder.cpp src/GameController.cpp src/GameControllerContainer.cpp src/GameEngine.cpp
	src/MediaUtil.cpp src/Mixer.cpp src/Square.cpp src/Texture.cpp src/Timer.cpp src/UID.cpp
	src/Window.cpp
	testsrc/ButtonTest.cpp testsrc/CollisionDetectorTest.cpp testsrc/EntityContainerTest.cpp
	testsrc/EntityPropertiesTest.cpp testsrc/Flat2dTest.cpp testsrc/SquareTest.cpp
	testsrc/UIDTest.cpp)

target_link_libraries(test_flat -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lsqlite3)
target_compile_options(test_flat PRIVATE -std=c++11 -g -Wall)

add_library(flat_static STATIC  src/Button.cpp src/Camera.cpp src/CollisionDetector.cpp
	src/Database.cpp src/DeltatimeMonitor.cpp src/Entity.cpp src/EntityContainer.cpp src/EntityProperties.cpp
	src/FlatBuilder.cpp src/GameController.cpp src/GameControllerContainer.cpp src/GameEngine.cpp
	src/MediaUtil.cpp src/Mixer.cpp src/Square.cpp src/Texture.cpp src/Timer.cpp src/UID.cpp src/Window.cpp)

target_link_libraries(flat_static -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lsqlite3)
target_compile_options(flat_static PRIVATE -std=c++11 -g -Wall)
set_target_properties(flat_static PROPERTIES OUTPUT_NAME flat)

add_library(flat SHARED  src/Button.cpp src/Camera.cpp src/CollisionDetector.cpp
	src/Database.cpp src/DeltatimeMonitor.cpp src/Entity.cpp src/EntityContainer.cpp src/EntityProperties.cpp
	src/FlatBuilder.cpp src/GameController.cpp src/GameControllerContainer.cpp src/GameEngine.cpp
	src/MediaUtil.cpp src/Mixer.cpp src/Square.cpp src/Texture.cpp src/Timer.cpp src/UID.cpp src/Window.cpp)

target_link_libraries(flat -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lsqlite3)
target_compile_options(flat PRIVATE -std=c++11 -g -Wall)
set_target_properties(flat PROPERTIES
	VERSION 0.0.0
	SOVERSION 0)

add_custom_command(TARGET flat flat_static
	COMMENT "Build unified header"
	COMMAND ${CMAKE_COMMAND} -DDIR_ROOT=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_header.cmake
	)

add_custom_target(lint
	COMMENT "Running cpp-lint"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND python2.7 lint/cpplint.py --linelength=120 --root=src --extensions=cpp,h --filter=-whitespace/tab,-whitespace/indent,-whitespace/parens,-whitespace/braces,-whitespace/comments,-runtime/indentation_namespace,-legal src/*
	)

enable_testing()
add_test(NAME catch COMMAND test_flat)

install(TARGETS test_flat flat flat_static
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install (FILES ${HEADERS} DESTINATION include/flat)
