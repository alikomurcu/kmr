#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <string>

namespace kmr {
    class KmrWindow {

    public:
        KmrWindow(int w, int h, std::string name);
        ~KmrWindow();

		// Delete copy constructor and copy assignment operator
		KmrWindow(const KmrWindow&) = delete;
        KmrWindow& operator=(const KmrWindow&) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:
        void initWindow();
        
        const int width;
        const int height;

        std::string windowName;
        GLFWwindow* window;
    };
} // namespace kmr