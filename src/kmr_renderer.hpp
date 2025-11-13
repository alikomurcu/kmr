#pragma once

#include "kmr_window.hpp"
#include "kmr_device.hpp"
#include "kmr_swap_chain.hpp"
#include "kmr_model.hpp"

// std
#include <memory>
#include <vector>
#include <cassert>

namespace kmr
{
    class KmrRenderer
    {
    public:
        KmrRenderer(KmrWindow &window, KmrDevice &device);
        ~KmrRenderer();

        KmrRenderer(const KmrRenderer &) = delete;
        KmrRenderer &operator=(const KmrRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const
        {
            return kmrSwapChain->getRenderPass();
        }

        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        uint32_t getCurrentFrameIndex() const
        {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        KmrWindow &kmrWindow;
        KmrDevice &kmrDevice;
        std::unique_ptr<KmrSwapChain> kmrSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };
} // namespace kmr