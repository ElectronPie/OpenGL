cmake_minimum_required(VERSION 3.27)

project(OpenGL)

add_executable(${PROJECT_NAME}
    src/Application.cpp

    src/Renderer.h
    src/Renderer.cpp

    src/VertexBuffer.h
    src/VertexBuffer.cpp

    src/IndexBuffer.h
    src/IndexBuffer.cpp

    src/VertexArray.h
    src/VertexArray.cpp
    src/VertexBufferLayout.h

    src/Shader.h
    src/Shader.cpp

    src/Texture.h
    src/Texture.cpp

    src/vendor/stb_image/stb_image.h
    src/vendor/stb_image/stb_image.cpp

    src/tests/Test.h
    src/tests/Test.cpp
    src/tests/TestClearColor.h
    src/tests/TestClearColor.cpp
    src/tests/TestBlending.h
    src/tests/TestBlending.cpp
    src/tests/TestTexture2D.h
    src/tests/TestTexture2D.cpp
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/debugbreak)

file(GLOB imgui_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.cpp
)
target_sources(${PROJECT_NAME} PRIVATE
    ${imgui_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp
)

target_link_libraries(${PROJECT_NAME} glad glfw debugbreak)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)
