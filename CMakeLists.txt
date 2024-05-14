cmake_minimum_required(VERSION 3.20)
project(SGE)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Engine)
add_subdirectory(EditorApp)
add_subdirectory(PhysxSample)
add_subdirectory(NormalMapSample)
add_subdirectory(Sandbox)
add_subdirectory(ShadowSample)

