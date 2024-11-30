cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

# Include directories
include_directories(OpenGLProject/include)
include_directories(OpenGLProject/include/glad)
include_directories(OpenGLProject/include/imgui)
include_directories(OpenGLProject/include/proprietary)
include_directories(OpenGLProject/include/assimp/include)
include_directories(OpenGLProject/include/assimp/build/include)

set(VENDOR_SOURCES_LOCATION "OpenGLProject/src/vendor")

# Automatically gather all .cpp and .c source files in the implementation folder
file(GLOB PROPRITARY_SOURCES "OpenGLProject/src/propritary/*.cpp" "OpenGLProject/src/propritary/*.c")
file(GLOB VENDOR_SOURCES "${VENDOR_SOURCES_LOCATION}/imgui/*.cpp" "${VENDOR_SOURCES_LOCATION}/imgui/*.c")

# Create the executable
add_executable(OpenGLProject ${PROPRITARY_SOURCES} ${VENDOR_SOURCES})

# Specify the path to the GLFW library
set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/OpenGLProject/lib/glfw3.lib")

# Link the GLFW and OpenGL libraries
target_link_libraries(OpenGLProject PRIVATE OpenGL32 ${GLFW_LIBRARIES})

# Link Assimp library
set(ASSIMP_LIBRARY_DIR "D:/Visual Studios/NewOpenGLBeginning/OpenGLProject/include/assimp/build")
set(ASSIMP_LIBRARY "${ASSIMP_LIBRARY_DIR}/lib/Debug/assimp-vc143-mtd.lib")
target_link_libraries(OpenGLProject PRIVATE ${ASSIMP_LIBRARY})

# Post-build command to copy Assimp DLL
add_custom_command(TARGET OpenGLProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${ASSIMP_LIBRARY_DIR}/bin/Debug/assimp-vc143-mtd.dll
    ${CMAKE_BINARY_DIR}/assimp-vc143-mtd.dll)

# Shader and asset copying
set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/OpenGLProject/shaders")
set(SHADER_DEST_DIR "${CMAKE_BINARY_DIR}/shaders")

add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR}
    COMMENT "Copying shaders to the build directory..."
)

add_dependencies(OpenGLProject copy_shaders)

set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/OpenGLProject/assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/assets")

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
    COMMENT "Copying assets to the build directory..."
)

add_dependencies(OpenGLProject copy_assets)

# Set the C++ standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGLProject PROPERTY CXX_STANDARD 20)
endif()
