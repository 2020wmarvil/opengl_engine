find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(Main main.cpp)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS} )
target_link_libraries(Main glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} SRC_CODE SDB_IMAGE )