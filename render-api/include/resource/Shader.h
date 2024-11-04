#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace FastGFX::Core {
    struct Renderer;
}

namespace FastGFX {
    namespace Resource {

        class Shader
        {
        public:
            Shader(const Core::Renderer& renderer);
            explicit Shader() = default;

            Shader useInternalShader(std::string shader_name);
            Shader useExternalVS(std::string glsl_file);
            Shader useExternalFS(std::string glsl_file);

            Shader Create();
            void Bind();
            void Destroy();

        private:
            const Core::Renderer* m_renderer;

            VkShaderModule vertex = VK_NULL_HANDLE;
            VkShaderModule frag = VK_NULL_HANDLE;

            VkPipeline renderPipelineObject = VK_NULL_HANDLE;
            VkPipelineLayout layout = VK_NULL_HANDLE;
        };

    }
}
#endif //SHADER_H
