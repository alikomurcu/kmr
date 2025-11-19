#pragma once

#include "kmr_device.hpp"
#include "kmr_game_object.hpp"
#include "kmr_pipeline.hpp"
#include "kmr_model.hpp"
#include "kmr_camera.hpp"

// std
#include <memory>
#include <vector>

namespace kmr
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(KmrDevice &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;
        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<KmrGameObject> &gameObjects, const KmrCamera &camera);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        KmrDevice &kmrDevice;

        std::unique_ptr<KmrPipeline> kmrPipeline;
        VkPipelineLayout pipelineLayout;
    };
} // namespace kmr