#include "resource/Buffer.h"
#include "util/ConsoleLog.h"

uint32_t findMemType(uint32_t typeFilter, VkPhysicalDevice pDevice, VkMemoryPropertyFlags memoryFlags)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(pDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & memoryFlags) == memoryFlags) {
            return i;
        }
    }

    printError("This machine cannot allocate!");
    exit(-1);
}
VkDeviceSize device_offset[] = {0};
uint32_t size = 0;
namespace FastGFX::Resource
{
    VertexBuffer::VertexBuffer(const Core::Renderer& renderer, const std::vector<Vertex> &vertices) : renderer(&renderer)
    {
        VkResult result = {};
        VkBufferCreateInfo vertexBufferInfo = {};
        vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vertexBufferInfo.size = sizeof(Vertex) * vertices.size();
        vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vertexBufferInfo.pNext = nullptr;

        result = vkCreateBuffer(this->renderer->engineDevice, &vertexBufferInfo, nullptr, &this->vertexBufferObject);
        if (result != VK_SUCCESS) {
            printError("Failed to create vertex buffer!");
        }
        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(this->renderer->engineDevice, this->vertexBufferObject, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = findMemType(memoryRequirements.memoryTypeBits, this->renderer->enginePhysicalDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        result = vkAllocateMemory(this->renderer->engineDevice, &memoryAllocateInfo, nullptr, &this->vertexBufferMemory);
        if (result != VK_SUCCESS) {
            printError("Failed to allocate vertex buffer!");
        }
        result = vkBindBufferMemory(this->renderer->engineDevice, this->vertexBufferObject, this->vertexBufferMemory, 0);
        if (result != VK_SUCCESS) {
            printError("Failed to allocate vertex buffer!");
        }

        void* data;
        vkMapMemory(this->renderer->engineDevice, vertexBufferMemory, 0, memoryRequirements.size, 0, &data);
        memcpy(data, vertices.data(), (size_t) memoryRequirements.size);
        vkUnmapMemory(this->renderer->engineDevice, vertexBufferMemory);
    }
    void VertexBuffer::Bind()
    {
        vkCmdBindVertexBuffers(this->renderer->commandBuffer, 0, 1, &this->vertexBufferObject, device_offset);
    }
    void VertexBuffer::Destroy()
    {
        vkDestroyBuffer(this->renderer->engineDevice, this->vertexBufferObject, nullptr);
        vkFreeMemory(this->renderer->engineDevice, this->vertexBufferMemory, nullptr);
    }

    IndexBuffer::IndexBuffer(const Core::Renderer &renderer, const std::vector<uint16_t> &indices): renderer(&renderer) {
        size = indices.size();
        VkResult result = {};
        VkBufferCreateInfo vertexBufferInfo = {};
        vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vertexBufferInfo.size = sizeof(uint16_t) * indices.size();
        vertexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        vertexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        vertexBufferInfo.pNext = nullptr;

        result = vkCreateBuffer(this->renderer->engineDevice, &vertexBufferInfo, nullptr, &this->indexBufferObject);
        if (result != VK_SUCCESS) {
            printError("Failed to create vertex buffer!");
        }
        VkMemoryRequirements memoryRequirements;
        vkGetBufferMemoryRequirements(this->renderer->engineDevice, this->indexBufferObject, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo = {};
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = findMemType(memoryRequirements.memoryTypeBits,
                                                         this->renderer->enginePhysicalDevice,
                                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        result = vkAllocateMemory(this->renderer->engineDevice, &memoryAllocateInfo, nullptr, &this->indexBufferMemory);
        if (result != VK_SUCCESS) {
            printError("Failed to allocate vertex buffer!");
        }
        result = vkBindBufferMemory(this->renderer->engineDevice, this->indexBufferObject, this->indexBufferMemory, 0);
        if (result != VK_SUCCESS) {
            printError("Failed to allocate vertex buffer!");
        }

        void *data;
        vkMapMemory(this->renderer->engineDevice, this->indexBufferMemory, 0, memoryRequirements.size, 0, &data);
        memcpy(data, indices.data(), memoryRequirements.size);
        vkUnmapMemory(this->renderer->engineDevice, this->indexBufferMemory);
    }
    void IndexBuffer::Bind() {
        vkCmdBindIndexBuffer(this->renderer->commandBuffer, this->indexBufferObject, 0, VK_INDEX_TYPE_UINT16);
        //vkCmdDrawIndexed(this->renderer->commandBuffer, size, 1, 0, 0, 0);
    }
    void IndexBuffer::Destroy() {
        vkDestroyBuffer(this->renderer->engineDevice, this->indexBufferObject, nullptr);
        vkFreeMemory(this->renderer->engineDevice, this->indexBufferMemory, nullptr);
    }
}
