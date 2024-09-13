add_executable(CameraTest CameraTest.cpp)

target_link_libraries(CameraTest PUBLIC glfw)

target_link_libraries(CameraTest PUBLIC BERender)