#include "kmr_window.hpp"

// std
#include <stdexcept>

namespace kmr {

    KmrWindow::KmrWindow(int w, int h, std::string name) : width(w), height(h), windowName(name) {
        initWindow();
    }

    KmrWindow::~KmrWindow() {
        glfwDestroyWindow(window);
    }


    void KmrWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window =  glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }

    void KmrWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }
} // namespace kmr