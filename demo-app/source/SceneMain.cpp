#include <test_scenes/SceneMain.h>

#include "resource/Shader.h"

VertexBuffer buffer{};
IndexBuffer indexBuffer{};
Shader shader{};

ObjectManager objectManager;
Object objects[1];


void SceneMain::onSceneLoad(Renderer& renderer) {
	objects[0] = objectManager.CreatePrimitive(objectManager.Primitive_Hexagon);
}

void SceneMain::onSceneUpdate(Renderer& renderer) {

	buffer = VertexBuffer(renderer, objects[0].mesh.vertices);
	indexBuffer = IndexBuffer(renderer, objects[0].mesh.indices);
	shader = Shader(renderer).useExternalVS("shaders/rgba_vs.spv").useExternalFS("shaders/rgba_fs.spv").Create();
}


void SceneMain::onSceneDraw(Renderer& renderer) {
	renderer.clearBG(0.0f, 0.0f, 0.0f, 1.0f);
	buffer.Bind();
	shader.Bind();
	indexBuffer.Bind();
}

void SceneMain::onSceneExit(Renderer& renderer) {
	shader.Destroy();
	indexBuffer.Destroy();
	buffer.Destroy();
}




