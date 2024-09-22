add_library(BERender src/Camera.cpp )

target_include_directories(BERender PUBLIC ${CMAKE_SOURCE_DIR}/common/include 
                            PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(BERender PUBLIC glfw)