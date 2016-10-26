cmake_minimum_required (VERSION 3.2.0)

SET(CMAKE_COLOR_MAKEFILE ON)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

project (flat CXX)

# Look for deps
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h) # Get all header files
set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/flat.h ${HEADERS}) # Add the generated header file

add_executable(test_flat EXCLUDE_FROM_ALL
	src/Button.cpp
	src/Camera.cpp
	src/CollisionDetector.cpp
	src/DeltatimeMonitor.cpp
	src/Entity.cpp
	src/EntityContainer.cpp
	src/EntityProperties.cpp
	src/FlatBuilder.cpp
	src/GameController.cpp
	src/GameControllerContainer.cpp
	src/GameEngine.cpp
	src/MediaUtil.cpp
	src/Mixer.cpp
	src/Square.cpp
	src/Texture.cpp
	src/Timer.cpp
	src/UID.cpp
	src/Window.cpp
	testsrc/ButtonTest.cpp
	testsrc/CollisionDetectorTest.cpp
	testsrc/EntityContainerTest.cpp
	testsrc/EntityPropertiesTest.cpp
	testsrc/Flat2dTest.cpp
	testsrc/SquareTest.cpp
	testsrc/UIDTest.cpp
	testsrc/CameraTest.cpp
	testsrc/AlgorithmTest.cpp
	src/RuntimeAnalyzer.cpp)

target_link_libraries(test_flat -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf)
target_compile_options(test_flat PRIVATE -std=c++11 -g -Wall)

add_library(flat
	src/Button.cpp
	src/Camera.cpp
	src/CollisionDetector.cpp
	src/DeltatimeMonitor.cpp
	src/Entity.cpp
	src/EntityContainer.cpp
	src/EntityProperties.cpp
	src/FlatBuilder.cpp
	src/GameController.cpp
	src/GameControllerContainer.cpp
	src/GameEngine.cpp
	src/MediaUtil.cpp
	src/Mixer.cpp
	src/Square.cpp
	src/Texture.cpp
	src/Timer.cpp
	src/UID.cpp
	src/Window.cpp
	src/Animation.cpp
	src/RuntimeAnalyzer.cpp)

target_link_libraries(flat -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf)
target_compile_options(flat PRIVATE -std=c++11 -Wall)
set_target_properties(flat PROPERTIES
	VERSION 0.0.0
	SOVERSION 0)

add_custom_command(TARGET flat
	POST_BUILD
	COMMENT "Build unified header"
	COMMAND ${CMAKE_COMMAND} -DDIR_ROOT=${CMAKE_CURRENT_SOURCE_DIR} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_header.cmake
	)

foreach(header ${HEADERS})
	add_custom_command(TARGET flat
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/include/flat
		COMMAND ${CMAKE_COMMAND} -E copy ${header} ${PROJECT_BINARY_DIR}/include/flat/
		)
endforeach()

add_custom_target(lint_flat
	COMMENT "Running cpp-lint"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND python2.7 lint/cpplint.py --linelength=120 --root=src --extensions=cpp,h --filter=-whitespace/tab,-whitespace/indent,-whitespace/parens,-whitespace/braces,-whitespace/comments,-runtime/indentation_namespace,-legal src/*
	)

add_custom_target(copy-doc ALL
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf ${PROJECT_BINARY_DIR}/.
	)

add_custom_target(doc
	COMMENT "Creating documentation"
	DEPENDS copy-doc
	COMMAND doxygen doxygen.conf
	)

add_custom_target(check_flat
	COMMENT "Running catch tests"
	DEPENDS test_flat
	COMMAND test_flat
	)

enable_testing()
add_test(NAME catch COMMAND test_flat)

install(TARGETS flat
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install (FILES ${HEADERS} DESTINATION include/flat)
