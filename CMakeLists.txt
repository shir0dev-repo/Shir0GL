cmake_minimum_required(VERSION 3.6)

# create C++ project
project(SOGL VERSION 0.1 LANGUAGES CXX)

# set required C++ standards and OpenGL preference
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(OpenGL_GL_PREFERENCE GLVND)

### BUILD OPTIONS ###

option(BUILD_LIBRARIES
	"Should we build static libraries (where applicable)?"
	ON
)

set(LIBRARY_OUTPUT_DIRS "${CMAKE_BINARY_DIR}/lib")

option(BUILD_COPY_HEADERS
	"Should we copy the available headers into the binary directory?"
	ON
)

# grab package finders
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# glob header files (external as well)
file(GLOB_RECURSE HEADERS
	"${CMAKE_SOURCE_DIR}/common/transform/*.hpp"
	"${CMAKE_SOURCE_DIR}/common/rendering/include/*.hpp"

)

# glob source files
file(GLOB_RECURSE SOURCES
	#"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/common/rendering/*.cpp"
)

# add glfw dir
include_directories(${GLFW3_INCLUDE_DIRS})

#include glfw library
find_package(GLFW3 REQUIRED)

# add glew dir
include_directories(${GLEW_INCLUDE_DIRS})

# include glew library
find_package(GLEW REQUIRED)

# find rendering library
find_package(rendering)
add_subdirectory(common/rendering)
include_directories(${RENDERING_INCLUDE_DIRS})

# add transform headers

if(WIN32)
	file(GLOB_RECURSE SHADERS "${CMAKE_SOURCE_DIR}/assets/shaders*")
	file(GLOB_RECURSE ASSETS "${CMAKE_SOURCE_DIR}/assets/*")

	source_group("Shaders" FILES ${SHADERS})

	set_property(
		DIRECTORY ${CMAKE_SOURCE_DIR}
		PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
	)

	add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SHADERS})

	target_link_libraries(${PROJECT_NAME}
		${GLFW_LIBRARIES}
		${GLEW_LIBRARIES}
		${RENDER_LIBRARIES}
	)

	set(ASSET_OUTPUT_DIR "${CMAKE_BINARY_DIR}/assets")

	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSET_OUTPUT_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" ${ASSET_OUTPUT_DIR}
	)

else()
	add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

	target_link_libraries(${PROJECT_NAME}
		${GLFW_LIBRARIES}
		${GLEW_LIBRARIES}
	)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)