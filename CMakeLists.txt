cmake_minimum_required (VERSION 3.2.0)

SET(CMAKE_COLOR_MAKEFILE ON)

project (flat CXX)
add_definitions(-std=c++14)

set(SDL2_BUILDING_LIBRARY "ON")

include(cmake/FindSDL2.cmake)
include(cmake/FindSDL2_image.cmake)
include(cmake/FindSDL2_ttf.cmake)
include(cmake/FindSDL2_mixer.cmake)
include(cmake/FindCCache.cmake)
include(cmake/Findcppcheck.cmake)
include(cmake/FindCCache.cmake)

include_directories(
	${PROJECT_BINARY_DIR}
	${SDL2_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_MIXER_INCLUDE_DIR}
	)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (CMAKE_BUILD_TYPE STREQUAL "travis")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 --coverage")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
	SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage")
endif()

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h) # Get all header files
set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/flat.h ${HEADERS}) # Add the generated header file

set(FLAT_SOURCES
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
	src/RuntimeAnalyzer.cpp
	src/QuadTree.cpp
	)

set(TEST_SOURCES
	testsrc/ButtonTest.cpp
	testsrc/CollisionDetectorTest.cpp
	testsrc/EntityContainerTest.cpp
	testsrc/EntityPropertiesTest.cpp
	testsrc/Flat2dTest.cpp
	testsrc/SquareTest.cpp
	testsrc/UIDTest.cpp
	testsrc/CameraTest.cpp
	testsrc/AlgorithmTest.cpp
	testsrc/AnimationTest.cpp
	testsrc/EntityTest.cpp
	testsrc/QuadTreeTest.cpp
	testsrc/DeltatimeMonitorTest.cpp)


add_executable(test_flat EXCLUDE_FROM_ALL ${FLAT_SOURCES} ${TEST_SOURCES})
target_link_libraries(test_flat
	${SDL2_LIBRARY}
	${SDL2MAIN_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_MIXER_LIBRARY}
	)

add_library(flat ${FLAT_SOURCES})
target_link_libraries(flat
	${SDL2_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_MIXER_LIBRARY}
	)

set_target_properties(flat PROPERTIES
	VERSION 1.0.0
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

# LINT:
if (CPPCHECK_FOUND)
	add_custom_target(lint
		COMMAND ${CPPCHECK_EXECUTABLE} --language=c++ --template=gcc --error-exitcode=1 --quiet --enable=warning,style,performance,portability,information,missingInclude src/
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		COMMENT "Run cppcheck"
		)
endif (CPPCHECK_FOUND)

add_custom_target(doc
	COMMENT "Creating documentation"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND doxygen
	)

enable_testing()
add_test(NAME catch COMMAND test_flat)
add_custom_target(check
	DEPENDS test_flat
	COMMAND test_flat
	)

install(TARGETS flat
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install (FILES ${HEADERS} DESTINATION include/flat)
