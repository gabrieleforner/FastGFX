#include <glm/glm.hpp>
#include "resource/Buffer.h"
#include <vector>

using namespace FastGFX::Resource;

namespace FastGFX::Core {

	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	};

	struct PrimitiveType {
		const char *name;
		Mesh mesh;
	};

	struct Object {
		const char* name;
		Mesh mesh;
	};

	static class ObjectManager {
	public:
		Object CreatePrimitive(PrimitiveType primitiveType);

		PrimitiveType Primitive_Square{
			"Square",
			Mesh{
				//Vertices
				{
					{{glm::vec3(-0.5f, -0.5f, 0.0f)}, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}},
					{{glm::vec3(0.5f, -0.5f, 0.0f)}, {glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}},
					{{glm::vec3(0.5f, 0.5f, 0.0f)}, {glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}},
					{{glm::vec3(-0.5f, 0.5f, 0.0f)}, {glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)}}
				},

				//Indices
				{ 0, 1, 3, 1, 2, 3}
			}
		};

		PrimitiveType Primitive_Triangle{
			"Triangle",
			Mesh{
				//Vertices
				{
					{{glm::vec3(0.0f, -0.5f, 0.0f)}, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}},
					{{glm::vec3(0.5f, 0.5f, 0.0f)}, {glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}},
					{{glm::vec3(-0.5f, 0.5f, 0.0f)}, {glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}}
				},

			//Indices
			{ 0, 1, 2}
			}	
		};

		PrimitiveType Primitive_Hexagon{
			"Hexagon",
			Mesh{
				//Vertices
				{
					{{glm::vec3(-0.5f, 0.0f, 0.0f)}, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}},//left
					{{glm::vec3(-0.25f, -0.5f, 0.0f)}, {glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}},//top left
					{{glm::vec3(0.25f, -0.5f, 0.0f)}, {glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}},//top right
					{{glm::vec3(0.5f, 0.0f, 0.0f) }, {glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}},//right
					{{glm::vec3(-0.25f, 0.5f, 0.0f)}, {glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}},//bottomleft
					{{glm::vec3(0.25f, 0.5f, 0.0f)}, {glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}},//bottomright
					{{glm::vec3(0.0f, 0.0f, 0.0f) }, {glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)} }//center
				},

				//Indices
				{ 
					0,1,6,
					1,2,6,
					6,2,3,
					6,3,4,
					5,6,4,
					0,6,5

				}
			}
		};
	};
}