# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("SimpleOpenGL")


include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/libs CACHE PATH "Missing description." FORCE)
FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG master)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG master)
FetchContent_MakeAvailable(glad)
# Fetch PhysX
FetchContent_Declare(
  PhysX
  GIT_REPOSITORY https://github.com/NVIDIA-Omniverse/PhysX.git
  GIT_TAG main
)
FetchContent_MakeAvailable(PhysX)



add_executable(${PROJECT_NAME}
        # triangle.cpp
        # plane2.cpp
        # plane.cpp
        # text_render.cpp
        BasicGeometryMesh.cpp
        )


include_directories(
        includes
        libs/glfw-src/includes
        libs/glfw-src/deps

)

# Configure libraries CMake uses to link your target library.
target_link_libraries(${PROJECT_NAME}
        glfw
        glad
)