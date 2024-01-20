cmake_minimum_required(VERSION 3.27)

project(OpenGL)

# to add my handmade imgui cmake module
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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

# Loading libraries

# GLAD
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad)
list(APPEND ${PROJECT_NAME}_LIBRARIES glad)
get_target_property(glad_INCLUDE_DIRECTORIES glad INCLUDE_DIRECTORIES)
list(APPEND ${PROJECT_NAME}_INCLUDES ${glad_INCLUDE_DIRECTORIES})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)
list(APPEND ${PROJECT_NAME}_LIBRARIES glfw)

# glm
list(APPEND ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm)

# debugbreak
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/debugbreak)
list(APPEND ${PROJECT_NAME}_LIBRARIES debugbreak)

# Dear ImGui
include(imgui)
target_sources(${PROJECT_NAME} PRIVATE ${imgui_SRC})
list(APPEND ${PROJECT_NAME}_INCLUDES ${imgui_INCLUDES})

list(APPEND ${PROJECT_NAME}_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor
)
list(REMOVE_DUPLICATES ${PROJECT_NAME}_INCLUDES)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${${PROJECT_NAME}_INCLUDES}
)

list(REMOVE_DUPLICATES ${PROJECT_NAME}_LIBRARIES)
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)
