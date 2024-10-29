#ifndef SHADER_H
#define SHADER_H

#include <vulkan/vulkan.h>
#include <util/ConsoleLog.h>
#include "core/Renderer.h"

namespace FastGFX::Resource
{
    class Shader
    {
    private:
        Core::Renderer m_renderer;
        VkShaderModule vertex, frag = VK_NULL_HANDLE;
        VkPipelineLayout layout = VK_NULL_HANDLE;
        VkPipeline renderPipelineObject = VK_NULL_HANDLE;
    public:
        Shader(Core::Renderer renderer);
        Shader useInternalShader(std::string shader_name);
        Shader useExternalVS(std::string glsl_file);
        Shader useExternalFS(std::string glsl_file);
        Shader Create(Core::Renderer renderer);

        void Bind();
        void Destroy();
    };
}


#endif //SHADER_H
