#include "resource/Shader.h"

#include <fstream>

static std::vector<char> readFile(std::string file) {
    std::ifstream file_stream (file, std::ios::binary | std::ios::ate);
    if (!file_stream.is_open()) {
        throw std::runtime_error("Could not open file " + file);
    }
    const size_t file_size = (size_t)file_stream.tellg();
    std::vector<char> buffer(file_size);
    file_stream.seekg(0);
    file_stream.read(buffer.data(), file_size);
    file_stream.close();
    return buffer;
}
std::vector<VkVertexInputAttributeDescription> setupVerticesAttrib()
{
    std::vector<VkVertexInputAttributeDescription> attributes(2);
}
namespace FastGFX::Resource {
    Shader::Shader(Core::Renderer renderer)
    {
        this->m_renderer = renderer;
    }
    Shader Shader::useInternalShader(std::string shader_name)
    {
        return *this;
    }

    Shader Shader::useExternalVS(std::string glsl_file) {
        std::vector<char> data = readFile(glsl_file);

        VkShaderModuleCreateInfo shader_module_info = {};
        shader_module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_info.pNext = nullptr;
        shader_module_info.codeSize = data.size();
        shader_module_info.pCode = reinterpret_cast<uint32_t*>(data.data());

        if (vkCreateShaderModule(this->m_renderer.engineDevice, &shader_module_info, nullptr, &this->vertex) != VK_SUCCESS) {
            throw std::runtime_error("Could not create vertex shader module!");
        }

        VkPipelineShaderStageCreateInfo stage_info = {};
        stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        stage_info.module = this->vertex;
        stage_info.pName = "main";

        return *this;
    }
    Shader Shader::useExternalFS(std::string glsl_file) {
        std::vector<char> data = readFile(glsl_file);

        VkShaderModuleCreateInfo shader_module_info = {};
        shader_module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_info.pNext = nullptr;
        shader_module_info.codeSize = data.size();
        shader_module_info.pCode = reinterpret_cast<uint32_t*>(data.data());

        if (vkCreateShaderModule(this->m_renderer.engineDevice, &shader_module_info, nullptr, &this->frag) != VK_SUCCESS) {
            throw std::runtime_error("Could not create vertex shader module!");
        }

        VkPipelineShaderStageCreateInfo stage_info = {};
        stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        stage_info.module = this->frag;
        stage_info.pName = "main";
        return *this;
    }
    Shader Shader::Create(Core::Renderer renderer)
    {
        return *this;
    }

    void Shader::Bind() {
        vkCmdBindPipeline(this->m_renderer.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->renderPipelineObject);
    }
    void Shader::Destroy() {
        vkDestroyPipeline(this->m_renderer.engineDevice, this->renderPipelineObject, nullptr);
    }
}
