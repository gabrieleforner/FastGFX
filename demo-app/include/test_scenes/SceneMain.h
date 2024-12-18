#include <util/Scene.h>
#include <resource/Buffer.h>
#include <ObjectManager.h>

using namespace FastGFX::Util;
using namespace FastGFX::Resource;

class SceneMain : public Scene
{
public:
	void onSceneLoad(Renderer& renderer);
	void onSceneUpdate(Renderer& renderer);
	void onSceneDraw(Renderer& renderer);
	void onSceneExit(Renderer& renderer);
};