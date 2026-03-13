<<<<<<< HEAD
cmake_minimum_required(VERSION 4.1)
project(toyEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(toyEngine main.cpp
        include/Canvas.h
        include/Geometry.h
        include/Renderer.h
        src/Canvas.cpp
        src/Renderer.cpp
        include/Model.h
        include/Texture.h
        include/stb_image.h
        src/stb_image_impl.cpp
        include/Material.h
        include/RenderData.h
        src/RenderData.cpp
)
=======
cmake_minimum_required(VERSION 4.0)
project(myRenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(myRenderer main.cpp)
>>>>>>> c3bc670 (trash)
