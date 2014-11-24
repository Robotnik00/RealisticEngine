CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(RealisticWater)


INCLUDE_DIRECTORIES(${RealisticWater_SOURCE_DIR}/Include)


FILE(GLOB_RECURSE SHADERS ${RealisticWater_SOURCE_DIR}/Shaders/*.*)

FILE(GLOB_RECURSE SOURCE ${RealisticWater_SOURCE_DIR}/Source/*.cpp)

FILE(GLOB_RECURSE HEADER ${RealisticWater_SOURCE_DIR}/Include/*.h)


ADD_EXECUTABLE(RealisticWater ${SOURCE} ${HEADER} ${SHADERS})

TARGET_LINK_LIBRARIES(RealisticWater RealisticEngine)

SET(CMAKE_CXX_FLAGS "-std=c++11")

add_custom_command(TARGET RealisticWater PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${RealisticWater_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:RealisticWater>)

add_custom_command(TARGET RealisticWater PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${RealisticWater_SOURCE_DIR}/Models $<TARGET_FILE_DIR:RealisticWater>)
