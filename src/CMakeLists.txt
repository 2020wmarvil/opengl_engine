add_library(source STATIC 
    VertexBuffer.cpp    VertexBuffer.h      VertexBufferLayout.h
    IndexBuffer.cpp     IndexBuffer.h
    VertexArray.cpp     VertexArray.h
    Shader.cpp          Shader.h
    Renderer.cpp        Renderer.h
    Texture.cpp         Texture.h
    Camera.cpp		Camera.h
    Cube.cpp		Cube.h
)

target_include_directories( source PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} vendor )

add_subdirectory(vendor)
