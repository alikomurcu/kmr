cmake_minimum_required(VERSION 3.25)
project(kmr LANGUAGES CXX)

# --- Build setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Vulkan SDK
find_package(Vulkan REQUIRED)

# --- glslc (from Vulkan SDK) ---
find_program(GLSLC_EXECUTABLE NAMES glslc HINTS $ENV{VULKAN_SDK}/Bin REQUIRED)

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
    src/kmr_model.cpp
)

# --- Shader compilation (GLSL -> SPIR-V) ---
file(GLOB SHADER_SOURCES "${CMAKE_SOURCE_DIR}/shaders/*.vert" "${CMAKE_SOURCE_DIR}/shaders/*.frag")
set(SPV_OUTPUTS)
foreach(shader ${SHADER_SOURCES})
    get_filename_component(fname "${shader}" NAME)
    set(out "${CMAKE_BINARY_DIR}/shaders/${fname}.spv")
    add_custom_command(
        OUTPUT "${out}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/shaders"
        COMMAND ${GLSLC_EXECUTABLE} "${shader}" -o "${out}"
        DEPENDS "${shader}"
        COMMENT "Compiling GLSL ${fname} -> SPIR-V"
        VERBATIM
    )
    list(APPEND SPV_OUTPUTS "${out}")
endforeach()
add_custom_target(compile_shaders ALL DEPENDS ${SPV_OUTPUTS})
add_dependencies(kmr compile_shaders)

# Copy compiled shaders next to the built executable after each build
add_custom_command(
    TARGET kmr POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:kmr>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_BINARY_DIR}/shaders" "$<TARGET_FILE_DIR:kmr>/shaders"
    COMMENT "Copying compiled shaders to the runtime output directory"
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