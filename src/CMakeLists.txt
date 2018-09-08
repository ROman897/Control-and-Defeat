file(GLOB_RECURSE ALL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE ALL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE ALL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
# ADD SOURCE FILES HERE

add_executable(game ${ALL_SOURCES})

# ADD libraries which are imported through CMake(ExternalProjectAdd or otherwise) here
add_dependencies(game glfw)
target_link_libraries(game ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

# ADD includes of libraries here
target_include_directories(game
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
    PRIVATE ${SINGLE_HEADERS_DIR}
)
