#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "kmr_camera.hpp"
#include "simple_render_system.hpp"
#include "kmr_model.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <stdexcept>
#include <chrono>
#include <array>
#include <cassert>

namespace kmr
{
    FirstApp::FirstApp()
    {
        loadGameObjects();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run()
    {
        SimpleRenderSystem simpleRenderSystem{kmrDevice, kmrRenderer.getSwapChainRenderPass()};
        KmrCamera camera{};
        // example camera setup
        camera.setViewTarget(
            glm::vec3{-1.f, -2.f, 2.f},
            glm::vec3{0.f, 0.f, 2.5f});

        auto viewerObject = KmrGameObject::createGameObject();
        KeyboardMovementController cameraController;

        auto curTime = std::chrono::high_resolution_clock::now();

        while (!kmrWindow.shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - curTime).count();
            curTime = newTime;

            // frameTime = glm::min(frameTime, 0.1f); // avoid large dt values

            cameraController.moveInPlaneXZ(kmrWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = kmrRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

            if (auto commandBuffer = kmrRenderer.beginFrame())
            {
                // make the render passes here, for example: shadow pass, post processing pass, etc.
                kmrRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

                kmrRenderer.endSwapChainRenderPass(commandBuffer);
                kmrRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(kmrDevice.device());
    }
    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<KmrModel> createCubeModel(KmrDevice &device, glm::vec3 offset)
    {
        KmrModel::Builder builder{};
        builder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},

            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},

            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},

            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},

        };

        for (auto &v : builder.vertices)
        {
            v.position += offset;
        }

        builder.indices = {0, 1, 2, 0, 3, 1, 4, 5, 6, 4, 7, 5, 8, 9, 10, 8, 11, 9,
                           12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
        return std::make_unique<KmrModel>(device, builder);
    }

    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<KmrModel> cubeModel = createCubeModel(kmrDevice, {0.f, 0.f, 0.f});
        auto cube = KmrGameObject::createGameObject();
        cube.model = cubeModel;
        cube.transform.translation = {0.f, 0.f, 2.5f};
        cube.transform.scale = {0.5f, 0.5f, 0.5f};
        gameObjects.push_back(std::move(cube));
    }

} // namespace kmr
