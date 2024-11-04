#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "core/Renderer.h"

namespace FastGFX {
    namespace Core {
        struct  Renderer;
    }
}

namespace  FastGFX::Resource
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 rgbaColor;
    };

    class VertexBuffer
    {
    private:
        VkBuffer vertexBufferObject = nullptr;
        VkDeviceMemory vertexBufferMemory = nullptr;
        const Core::Renderer* renderer;
    public:
        // Fluid Methods
        VertexBuffer(const Core::Renderer& renderer, const std::vector<Vertex> &vertices);
        explicit VertexBuffer() = default;
        void Bind();
        void Destroy();
    };

    class IndexBuffer
    {
    private:
        VkBuffer indexBufferObject = nullptr;
        VkDeviceMemory indexBufferMemory = nullptr;
        const Core::Renderer* renderer;
    public:
        // Fluid Methods

        IndexBuffer(const Core::Renderer& renderer, const std::vector<uint16_t> &indices);
        explicit IndexBuffer() = default;
        void Bind();
        void Destroy();
    };
}
