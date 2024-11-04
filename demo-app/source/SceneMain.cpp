#include <test_scenes/SceneMain.h>

#include "resource/Shader.h"

std::vector<Vertex> data_vector = {
	{{glm::vec3(1.0f, 1.0f, 1.0f)}, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}},
	{{glm::vec3(1.0f, 1.0f, 1.0f)}, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}},
	{{glm::vec3(1.0f, 1.0f, 1.0f)}, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}}
};
std::vector<uint16_t> indices = { 0, 1, 2};
VertexBuffer buffer{};
IndexBuffer indexBuffer{};
Shader shader{};

void SceneMain::onSceneLoad(Renderer renderer) {

}

void SceneMain::onSceneUpdate(Renderer renderer) {
	buffer = VertexBuffer(renderer, data_vector);
	indexBuffer = IndexBuffer(renderer, indices);
	shader = Shader(renderer).useExternalVS("rgba_vertex.spv").useExternalFS("rgba_fragment.spv").Create();
}


void SceneMain::onSceneDraw(Renderer renderer) {
	renderer.clearBG(0.0f, 0.0f, 0.0f, 1.0f);
	buffer.Bind();
	shader.Bind();
	indexBuffer.Bind();
}

void SceneMain::onSceneExit(Renderer renderer) {
	buffer.Destroy();
}




