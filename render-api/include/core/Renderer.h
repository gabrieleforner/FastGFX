#pragma once
#define GLFW_INCLUDE_VULKAN
#include <VkBootstrap.h>
#include <GLFW/glfw3.h>

#include "resource/Buffer.h"


namespace FastGFX {
    namespace Resource {
        class VertexBuffer;
    }
}


namespace FastGFX::Core
{
    class Renderer
    {
    private:
        vkb::Instance instance;
        vkb::PhysicalDevice physicalDevice;
        vkb::Device device;
        vkb::Swapchain swapchain;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        std::vector<VkFramebuffer> framebuffers;
        VkCommandPool commandPool;

        uint32_t imageIndex;
        VkRenderPassBeginInfo renderPassInfo = {};
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;
        VkInstance engineInstance;
        VkDebugUtilsMessengerEXT engineDebugMessenger;
        VkSurfaceKHR engineSurface;
        VkSwapchainKHR swapChain;
        VkPhysicalDevice enginePhysicalDevice;
        VkDevice engineDevice;
        VkQueue enginePresentQueue;
        VkQueue engineGraphicsProcQueue;
        VkRenderPass engineRenderPass;
        VkClearValue clearColor = {};
        VkCommandBuffer commandBuffer;
        friend Resource::VertexBuffer;
    public:

        void create(GLFWwindow* window);
        void setFrame();
        void startDrawRecord();
        void endDrawRecord();
        void destroy();


        void clearBG(float r, float g, float b, float a);

    };
}