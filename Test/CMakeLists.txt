add_executable(CameraTest CameraTest.cpp)

target_include_directories(CameraTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)

target_link_libraries(CameraTest PUBLIC BERender)

add_executable(ShaderTest ShaderTest.cpp)

target_include_directories(ShaderTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)

target_link_libraries(ShaderTest PUBLIC BERender)

add_custom_command(
  TARGET ShaderTest POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/Resources
  ${CMAKE_CURRENT_BINARY_DIR}/Resources)

add_executable(TextureGeometryTest TextureGeometryTest.cpp)

target_include_directories(TextureGeometryTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)
  
target_link_libraries(TextureGeometryTest PUBLIC BERender)