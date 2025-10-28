#pragma once

#include "kmr_pipeline.hpp"
#include "kmr_window.hpp"
#include "kmr_device.hpp"
#include "kmr_swap_chain.hpp"

// std
#include <memory>
#include <vector>

namespace kmr {
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;

        void run();

    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
    
        KmrWindow kmrWindow{ WIDTH, HEIGHT, "MERHABA VULKAN!" };
        KmrDevice kmrDevice{ kmrWindow };
        KmrSwapChain kmrSwapChain{ kmrDevice, kmrWindow.getExtent() };
        std::unique_ptr<KmrPipeline> kmrPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
    };
} // namespace kmr