add_library(stb_image stb_image.c)

target_include_directories(stb_image PUBLIC ${CMAKE_SOURCE_DIR}/common/include)