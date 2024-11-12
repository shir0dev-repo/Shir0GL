file(GLOB TRANSFORM_SRC ${PROJECT_SOURCE_DIR}/transform/*.hpp)
add_library(transform SHARED ${TRANSFORM_SRC})

target_include_directories(transform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(transform PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS transform DESTINATION lib)
install(FILES ${TRANSFORM_SRC} DESTINATION include)