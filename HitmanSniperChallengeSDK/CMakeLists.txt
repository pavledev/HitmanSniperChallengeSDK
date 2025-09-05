file(GLOB_RECURSE SRC_FILES
	CONFIGURE_DEPENDS
	src/*.cpp
	src/*.c
	src/*.hpp
	src/*.h,
)

file(GLOB_RECURSE HEADER_FILES
	CONFIGURE_DEPENDS
	include/*.h
)

add_library(HitmanSniperChallengeSDK SHARED
	${SRC_FILES}
	${HEADER_FILES}
 )

target_include_directories(HitmanSniperChallengeSDK PRIVATE
	${CMAKE_SOURCE_DIR}/External/mINI/src/mini
)

target_include_directories(HitmanSniperChallengeSDK PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/External/IconFontCppHeaders
)

target_link_libraries(HitmanSniperChallengeSDK PUBLIC
	Microsoft::DirectXTK
	imgui-docking-layout
	minhook::minhook
	winmm
	d3d11
	dxgi
	delayimp
)

target_link_options(HitmanSniperChallengeSDK PRIVATE
	/DELAYLOAD:d3d11.dll
)

target_compile_definitions(HitmanSniperChallengeSDK PRIVATE
	EXPORTS
)

install(TARGETS HitmanSniperChallengeSDK
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
