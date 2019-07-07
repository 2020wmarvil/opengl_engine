add_library(SRC_CODE STATIC 
    VertexBuffer.h
    VertexBuffer.cpp
    IndexBuffer.h
    IndexBuffer.cpp
    VertexArray.h
    VertexArray.cpp
    VertexBufferLayout.h
    Shader.h
    Shader.cpp
    Renderer.h
    Renderer.cpp
)

target_include_directories(SRC_CODE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
