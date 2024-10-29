#include "resource/Shader.h"
#include "resource/Buffer.h"

#include <fstream>

VkPipelineShaderStageCreateInfo stages[2];
struct EngineShader
{
    std::string name;
    std::string vsFile;
    std::string fsFile;
};
std::vector<EngineShader> engineShaders = {
    {"rgba_default", "shaders/rgba_vert.spv", "shaders/rgba_frag.spv"}
};

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

    // TODO: Map Attributes
    //  * attrib[0] -> FORMAT: vec3 (XYZ Position)
    //  * attrib[1] -> FORMAT: vec4 (RGBA Color)

    attributes[0].binding = 0;
    attributes[0].location = 0;
    attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributes[0].offset = offsetof(FastGFX::Resource::Vertex, position);

    attributes[1].binding = 0;
    attributes[1].location = 0;
    attributes[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributes[1].offset = offsetof(FastGFX::Resource::Vertex, rgbaColor);

    return attributes;
}
namespace FastGFX::Resource {
    Shader::Shader(Core::Renderer renderer)
    {
        this->m_renderer = renderer;
    }
    Shader Shader::useInternalShader(std::string shader_name)
    {
        for (auto& shader : engineShaders)
        {
            if (shader.name == shader_name)
            {


                break;
            }
        }
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
        stages[0] = stage_info;
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

        stages[1] = stage_info;

        return *this;
    }
    Shader Shader::Create(Core::Renderer renderer)
    {
        VkVertexInputBindingDescription bind_desc{};
        bind_desc.binding = 0;
        bind_desc.stride = sizeof(Vertex);
        bind_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        std::vector<VkDynamicState> states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.pNext = 0;
        dynamicState.dynamicStateCount = states.size();
        dynamicState.pDynamicStates = states.data();
        dynamicState.flags = 0;

        VkPipelineVertexInputStateCreateInfo inputSetting{};
        inputSetting.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        inputSetting.pNext = 0;

        inputSetting.vertexAttributeDescriptionCount = 1;
        inputSetting.vertexBindingDescriptionCount = 1;
        inputSetting.pVertexAttributeDescriptions = setupVerticesAttrib().data();
        inputSetting.pVertexBindingDescriptions = &bind_desc;

        VkPipelineInputAssemblyStateCreateInfo iaConfiguration{};
        iaConfiguration.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        iaConfiguration.pNext = 0;
        iaConfiguration.primitiveRestartEnable = VK_FALSE;
        iaConfiguration.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

        VkViewport pipelineRenderArea{};
        pipelineRenderArea.width = this->m_renderer.swapchain.extent.width;
        pipelineRenderArea.height = this->m_renderer.swapchain.extent.height;
        pipelineRenderArea.x = 0;
        pipelineRenderArea.y = 0;
        pipelineRenderArea.minDepth = 0.0f;
        pipelineRenderArea.maxDepth = 1.0f;

        VkRect2D scissorConfig{};
        scissorConfig.extent = this->m_renderer.swapchain.extent;
        scissorConfig.offset = { 0, 0 };

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pNext = 0;
        viewportState.scissorCount = 1;
        viewportState.viewportCount = 1;
        viewportState.pScissors = &scissorConfig;
        viewportState.pViewports = &pipelineRenderArea;

        VkPipelineRasterizationStateCreateInfo raster{};
        raster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        raster.pNext = 0;
        raster.depthClampEnable = VK_TRUE;
        raster.lineWidth = 1.0f;
        raster.cullMode = VK_CULL_MODE_BACK_BIT;
        raster.frontFace = VK_FRONT_FACE_CLOCKWISE;
        raster.depthBiasEnable = VK_FALSE;
        raster.depthBiasConstantFactor = 0.0f; // Optional
        raster.depthBiasClamp = 0.0f; // Optional
        raster.depthBiasSlopeFactor = 0.0f;
        raster.rasterizerDiscardEnable = VK_FALSE;  // Disables passing vertices to the rasterizing stage

        VkPipelineMultisampleStateCreateInfo msaa{};
        msaa.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        msaa.pNext = 0;
        msaa.rasterizationSamples = VK_SAMPLE_COUNT_4_BIT;
        msaa.sampleShadingEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState blending{};
        blending.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        blending.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &blending;

        VkPipelineLayout pipelineLayout{};
        VkPipelineLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutInfo.pNext = 0;

        if (vkCreatePipelineLayout(this->m_renderer.engineDevice, &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            printError("Failed to set up render pipeline layout!");
            exit(-1);
        }

        VkShaderModule mods[] = {this->vertex, this->frag};

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = stages;
        pipelineInfo.pVertexInputState = &inputSetting;
        pipelineInfo.pInputAssemblyState = &iaConfiguration;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &raster;
        pipelineInfo.pMultisampleState = &msaa;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = this->m_renderer.engineRenderPass;
        pipelineInfo.subpass = 0;

        if (vkCreateGraphicsPipelines(this->m_renderer.engineDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->renderPipelineObject) != VK_SUCCESS) {
            printError("Failed to create a valid render pipeline!");
            exit(-1);
        }

        return *this;
    }

    void Shader::Bind() {
        vkCmdBindPipeline(this->m_renderer.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->renderPipelineObject);
    }
    void Shader::Destroy() {
        vkDestroyPipelineLayout(this->m_renderer.engineDevice, this->layout, nullptr);
        vkDestroyPipeline(this->m_renderer.engineDevice, this->renderPipelineObject, nullptr);
    }
}
