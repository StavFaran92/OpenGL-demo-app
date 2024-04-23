target_include_directories(Engine PUBLIC 	
	assimp/include
	entt
	glew/include
	GLM
	imgui
	SDL2/include
	spdlog/include
	stb_image
	
	$ENV{PHYSX}/physx/include
	$ENV{PHYSX}/pxshared/include
)

target_link_directories(Engine PUBLIC 
	assimp/lib
	glew/lib/Release/x64
	SDL2/lib
	spdlog/lib
	
	$ENV{PHYSX}/physx/bin/win.x86_64.vc142.mt/checked
)

target_link_libraries(Engine PUBLIC 
	PhysX_64
	PhysXCommon_64
	PhysXExtensions_static_64
	PhysXCooking_64
	PhysXCharacterKinematic_static_64
	PhysXPvdSDK_static_64
	PhysXVehicle_static_64
	PhysXFoundation_64
	SDL2
	SDL2main
	opengl32
	glew32
	assimp-vc142-mt
	spdlog
)
	