#pragma once

#include "kmr_device.hpp"

// std
#include <string>
#include <vector>

namespace kmr {
    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
      };

    class KmrPipeline {
    public:
        KmrPipeline(
            KmrDevice& device,
            const std::string& vertFilePath,
            const std::string& fragFilePath,
            const PipelineConfigInfo& configInfo);

        ~KmrPipeline();
        
        // Delete copy constructor and copy assignment operator
        KmrPipeline(const KmrPipeline&) = delete;
        void operator=(const KmrPipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string& filePath);
        void createGraphicsPipeline(
            const std::string& vertFilePath,
            const std::string& fragFilePath,
            const PipelineConfigInfo& configInfo);
        
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);


        KmrDevice& kmrDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
} // namespace kmr