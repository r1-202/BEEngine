add_library(BERender src/Camera.cpp src/Geometry.cpp src/Shader.cpp src/Texture.cpp)

target_include_directories(BERender PUBLIC ${CMAKE_SOURCE_DIR}/common/include 
                            PUBLIC include)

target_link_libraries(BERender PUBLIC glfw PUBLIC glad)