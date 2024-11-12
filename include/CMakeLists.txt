set(TRANSFORM_SRC 
	mathutil.inl
	matrix3f.inl
	matrix4f.inl
	vec3f.inl
	vec4f.inl
	quat.inl
	transform.inl
)

add_library(transform ${TRANSFORM_SRC} ${TRANSFORM_SRC})

target_include_directories(transform INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS transform DESTINATION lib)
install(FILES ${TRANSFORM_SRC} DESTINATION include)