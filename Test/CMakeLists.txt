add_executable(CameraTest CameraTest.cpp)

target_include_directories(CameraTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)

target_link_libraries(CameraTest PUBLIC BERender)

add_executable(ShaderTest ShaderTest.cpp)

target_include_directories(ShaderTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)

target_link_libraries(ShaderTest PUBLIC BERender)

add_executable(TextureGeometryTest TextureGeometryTest.cpp)

target_include_directories(TextureGeometryTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)
  
target_link_libraries(TextureGeometryTest PUBLIC BERender)

add_executable(LoadingTest LoadingTest.cpp)

target_include_directories(LoadingTest PUBLIC ${CMAKE_SOURCE_DIR}/common/include)
  
target_link_libraries(LoadingTest PUBLIC BERender)

add_executable(VKWindowTest VKWindowTest.cpp)

target_link_libraries(VKWindowTest VKBERender)

add_executable(VKDrawTriangle VKDrawTriangle.cpp)

target_link_libraries(VKDrawTriangle VKBERender)

file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_SOURCE_DIR}/Resources/*")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/Resources
  ${CMAKE_CURRENT_BINARY_DIR}/Resources
  DEPENDS ${RESOURCE_FILES}
  COMMENT "Copying Resources")

add_custom_target(copy_resources ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Resources
)