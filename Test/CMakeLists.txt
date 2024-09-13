add_executable(CameraTest CameraTest.cpp)

target_include_directories(CameraTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)

target_link_libraries(CameraTest PUBLIC glfw)

target_link_libraries(CameraTest PUBLIC BERender)