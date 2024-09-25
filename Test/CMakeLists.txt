add_executable(CameraTest CameraTest.cpp)

target_include_directories(CameraTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)

target_link_libraries(CameraTest PUBLIC BERender)

add_executable(ShaderTest ShaderTest.cpp)

target_include_directories(ShaderTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)

target_link_libraries(ShaderTest PUBLIC BERender)