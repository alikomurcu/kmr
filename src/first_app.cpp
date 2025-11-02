#include "first_app.hpp"

// std
#include <stdexcept>
#include <array>

namespace kmr {

    FirstApp::FirstApp() {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() {
        vkDestroyPipelineLayout(kmrDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run() {
        while(!kmrWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(kmrDevice.device());
    }

    void FirstApp::loadModels() {
        std::vector<KmrModel::Vertex> vertices = {
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
        };
        kmrModel = std::make_unique<KmrModel>(kmrDevice, vertices);
    }

    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        
        if (vkCreatePipelineLayout(kmrDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void FirstApp::createPipeline() {
        auto pipelineConfig = KmrPipeline::defaultPipelineConfigInfo(kmrSwapChain.width(), kmrSwapChain.height());
        pipelineConfig.renderPass = kmrSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        kmrPipeline = std::make_unique<KmrPipeline>(kmrDevice, "shaders/triangle.vert.spv", "shaders/triangle.frag.spv", pipelineConfig);
    }

    void FirstApp::createCommandBuffers() {
        commandBuffers.resize(kmrSwapChain.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = kmrDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(kmrDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = kmrSwapChain.getRenderPass();
            renderPassInfo.framebuffer = kmrSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = kmrSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            kmrPipeline->bind(commandBuffers[i]);
            kmrModel->bind(commandBuffers[i]);
            kmrModel->draw(commandBuffers[i]);
            
            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to end recording command buffer!");
            }
        }
    }

    void FirstApp::drawFrame() {
        uint32_t imageIndex;
        auto result = kmrSwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        result = kmrSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to submit command buffers!");
        }
    }
} // namespace kmr