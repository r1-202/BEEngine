add_library(VKBERender src/Camera.cpp)

target_include_directories(VKBERender PUBLIC ${CMAKE_SOURCE_DIR}/common/include 
                            PUBLIC include)


find_package(Vulkan REQUIRED)                            
target_link_libraries(VKBERender PUBLIC glfw PUBLIC stb_image Vulkan::Vulkan)