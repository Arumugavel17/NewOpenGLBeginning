cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

# Include directories
include_directories(include)
include_directories(include/glad)
include_directories(include/GLFW)
include_directories(include/glm)
include_directories(include/header)
include_directories(include/header/vendor)
include_directories(include/header/proprietary)
include_directories(include/header/vendor/assimp/include)
include_directories(include/header/vendor/assimp/build/include)

# Automatically gather all .cpp and .c source files in the implementation folder
file(GLOB SOURCES "src/*.cpp" "src/*.c")

# Create the executable
add_executable(OpenGLProject ${SOURCES} )

# Specify the path to the GLFW library
set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib")

# Link the GLFW and OpenGL libraries
target_link_libraries(OpenGLProject OpenGL32 ${GLFW_LIBRARIES})
 
# Link Assimp library
set(ASSIMP_LIBRARY_DIR "C:/Users/arumu/source/repos/OpenGLProject/OpenGLProject/include/header/vendor/assimp/build")
set(ASSIMP_LIBRARY "${ASSIMP_LIBRARY_DIR}/lib/Debug/assimp-vc143-mtd.lib")
target_link_libraries(OpenGLProject ${ASSIMP_LIBRARY})

# Ensure Assimp DLL is found at runtime 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/OpenGLProject)
add_custom_command(TARGET OpenGLProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${ASSIMP_LIBRARY_DIR}/bin/Debug/assimp-vc143-mtd.dll
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assimp-vc143-mtd.dll)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}") 
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/OpenGLProject/shaders") 
set(SHADER_DEST_DIR "${CMAKE_BINARY_DIR}/OpenGLProject/shaders") 
# Add a custom command to copy the folder after the target is built 
add_custom_command(TARGET OpenGLProject POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_SOURCE_DIR} ${SHADER_DEST_DIR})

set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/OpenGLProject/assets") 
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/OpenGLProject/assets") 
# Add a custom command to copy the folder after the target is built 
add_custom_command(TARGET OpenGLProject POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR})

# Set the C++ standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGLProject PROPERTY CXX_STANDARD 20) 
endif()

# TODO: Add tests and install targets if needed
