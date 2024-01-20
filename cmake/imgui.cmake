set(imgui_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imstb_truetype.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib.cpp
)

set(imgui_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends
    # ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp
)
