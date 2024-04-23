file(GLOB IMGUI_SOURCES *.cpp *.h)
add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC 
	../SDL2/include/SDL2
)