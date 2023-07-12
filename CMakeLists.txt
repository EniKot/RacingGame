cmake_minimum_required(VERSION 3.21)
project(RacingGame)

set(CMAKE_CXX_STANDARD 20)
set(INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include)
include_directories(RacingGame ${INCLUDE_DIRECTORIES})
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(RacingGame src/main.cpp
        include/glad.c
        include/imgui/imgui.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_demo.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/imgui_impl_glfw.cpp
        include/imgui/imgui_impl_opengl3.cpp

        src/stb_image.cpp
        src/Camera.h
        src/SubMesh.h
        src/Mesh.h
        src/SkyBox.h
        src/Texture.h
        src/Shader.h
        src/CubeTexture.h
        src/ShadowMap.h
        src/Entity.h
        src/Car.cpp
        src/Car.h
        src/Base.h
        src/DirectionalLight.cpp
        src/DirectionalLight.h
     )

target_link_libraries(RacingGame ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)
target_link_libraries(RacingGame ${PROJECT_SOURCE_DIR}/lib/libassimp-5.dll)
