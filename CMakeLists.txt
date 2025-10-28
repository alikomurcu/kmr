cmake_minimum_required(VERSION 3.25)
project(kmr LANGUAGES CXX)

# --- Build setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Vulkan SDK
find_package(Vulkan REQUIRED)

# ---- GLFW (precompiled, local) ---------------------------------------------
set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/external/glfw")

# Static .lib
add_library(glfw3 STATIC IMPORTED GLOBAL)
set_target_properties(glfw3 PROPERTIES
    IMPORTED_LOCATION "${GLFW_ROOT}/lib-vc2022/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${GLFW_ROOT}/include"
)

# On Windows, GLFW needs these system libs when linking statically
if (MSVC)
    set(GLFW_WIN_SYS_LIBS user32 gdi32 shell32 ole32 advapi32 winmm)
endif()

# ---- Executable -------------------------------------------------------------
add_executable(kmr
    src/main.cpp
    src/first_app.cpp
    src/kmr_window.cpp
    src/kmr_pipeline.cpp
    src/kmr_device.cpp
    src/kmr_swap_chain.cpp
)

# --- Runtime assets (shaders) ---
set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shaders")

# Copy shaders next to the built executable after each build
add_custom_command(
    TARGET kmr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:kmr>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${SHADER_DIR}" "$<TARGET_FILE_DIR:kmr>/shaders"
    COMMENT "Copying shaders to the runtime output directory"
)

# In Visual Studio, run the app with the exe folder as working directory
if (MSVC)
    set_property(TARGET kmr PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:kmr>")
endif()

# (Optional) install rule so `cmake --install` bundles shaders with the exe
install(TARGETS kmr RUNTIME DESTINATION .)
install(DIRECTORY "${SHADER_DIR}" DESTINATION .)


# Make the headers visible to kmr
target_include_directories(kmr PRIVATE
    "${CMAKE_SOURCE_DIR}/external/glm"
    "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(kmr PRIVATE
    Vulkan::Vulkan
    glfw3
    ${GLFW_WIN_SYS_LIBS}
)