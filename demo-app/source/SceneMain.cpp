#include <test_scenes/SceneMain.h>
#include <resource/Shader.h>

std::vector<Vertex> data_vector = {
	{{glm::vec3(1.0f, 1.0f, 1.0f)}, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}},
	{{glm::vec3(1.0f, 1.0f, 1.0f)}, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}},
	{{glm::vec3(1.0f, 1.0f, 1.0f)}, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}}
};

VertexBuffer buffer{};
Shader shader;

void SceneMain::onSceneLoad(Renderer renderer) {
	buffer.Create(renderer, data_vector);
	shader = Shader(renderer)
		.useExternalVS("shaders/rgba_vert.spv")
		.useExternalFS("shaders/rgba_frag.spv")
		.Create(renderer);
}

void SceneMain::onSceneUpdate(Renderer renderer) {

}


void SceneMain::onSceneDraw(Renderer renderer) {
	renderer.clearBG(0.0f, 0.5f, 0.5f, 1.0f);
	shader.Bind();
}

void SceneMain::onSceneExit(Renderer renderer) {
	buffer.Destroy(renderer);
	shader.Destroy();
}




