#pragma once

#include "kmr_pipeline.hpp"
#include "kmr_window.hpp"
#include "kmr_game_object.hpp"
#include "kmr_device.hpp"
#include "kmr_swap_chain.hpp"
#include "kmr_model.hpp"

// std
#include <memory>
#include <vector>

namespace kmr
{
    class FirstApp
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;

        void run();

    private:
        void loadGameObjects();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int index);
        void renderGameObjects(VkCommandBuffer commandBuffer);

        KmrWindow kmrWindow{WIDTH, HEIGHT, "MERHABA VULKAN!"};
        KmrDevice kmrDevice{kmrWindow};
        std::unique_ptr<KmrSwapChain> kmrSwapChain;
        std::unique_ptr<KmrPipeline> kmrPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<KmrGameObject> gameObjects;
    };
} // namespace kmr