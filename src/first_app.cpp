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

    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<KmrModel> model = KmrModel::createModelFromFile(
            kmrDevice,
            "../models/flat_vase.obj");
        auto gameObj = KmrGameObject::createGameObject();
        gameObj.model = model;
        gameObj.transform.translation = {0.f, 0.5f, 2.5f};
        gameObj.transform.scale = glm::vec3{3.f, 1.f, 3.f};
        gameObjects.push_back(std::move(gameObj));
    }

} // namespace kmr
