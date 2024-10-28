cmake_minimum_required(VERSION 3.10)
project(render_api)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(vk-bootstrap)

set(CMAKE_CXX_STANDARD 23)
set(SOURCES include/core/AppModel.h
		include/core/Renderer.h
		include/util/Scene.h
		source/core/AppModel.cpp
		source/core/Renderer.cpp
		source/util/Scene.cpp
		source/util/ConsoleLog.cpp
		source/util/VkBootstrap.cpp
		source/resource/Buffer.cpp
		include/resource/Buffer.h
		source/resource/Shader.cpp
		include/resource/Shader.h)

if(WIN32)
	add_library(render_api STATIC ${SOURCES})
else()
	add_library(render_api SHARED ${SOURCES})
endif()

target_include_directories(render_api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(render_api PUBLIC glfw)
target_link_libraries(render_api PUBLIC Vulkan::Vulkan vk-bootstrap::vk-bootstrap vk-bootstrap::vk-bootstrap-compiler-warnings)
add_compile_options(-Wunused-parameter)

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})
file(GLOB SHADER_FILES "${SHADER_SOURCE_DIR}/*.vert" "${SHADER_SOURCE_DIR}/*.frag")

foreach(SHADER ${SHADER_FILES})
	get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
	set(SHADER_OUTPUT ${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv)

	add_custom_command(
			OUTPUT ${SHADER_OUTPUT}
			COMMAND glslc ${SHADER} -o ${SHADER_OUTPUT}
			DEPENDS ${SHADER}
			COMMENT "Compiling ${SHADER} to ${SHADER_OUTPUT}"
	)

	list(APPEND SPIRV_FILES ${SHADER_OUTPUT})
endforeach()

add_custom_target(compile_shaders ALL DEPENDS ${SPIRV_FILES})
target_sources(render_api PRIVATE ${SPIRV_FILES})
