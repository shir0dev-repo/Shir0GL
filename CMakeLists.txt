cmake_minimum_required(VERSION 3.31)

project(SOGL VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_TRANSFORM "Use transform structures" ON)

configure_file(soglConfig.h.in soglConfig.h)

if(USE_TRANSFORM)
	add_subdirectory(transform)
	list(APPEND EXTRA_LIBS transform)
endif()

add_library(${PROJECT_NAME} SHARED "${PROJECT_SOURCE_DIR}/src/main.cpp")

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC
	"${PROJECT_BINARY_DIR}"
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/soglConfig.h" DESTINATION include)