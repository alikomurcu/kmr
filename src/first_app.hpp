#pragma once

#include "kmr_device.hpp"
#include "kmr_game_object.hpp"
#include "kmr_window.hpp"
#include "kmr_renderer.hpp"

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

        KmrWindow kmrWindow{WIDTH, HEIGHT, "MERHABA VULKAN!"};
        KmrDevice kmrDevice{kmrWindow};
        KmrRenderer kmrRenderer{kmrWindow, kmrDevice};

        std::vector<KmrGameObject> gameObjects;
    };
} // namespace kmr