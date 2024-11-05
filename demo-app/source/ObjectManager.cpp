#include <ObjectManager.h>

namespace FastGFX::Core {
	Object ObjectManager::CreatePrimitive(PrimitiveType primitiveType) {
		Object obj{};

		obj.name = primitiveType.name;
		obj.mesh = primitiveType.mesh;

		return obj;
	}
}