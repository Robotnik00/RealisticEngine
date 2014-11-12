CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(SimpleDemo)

INCLUDE_DIRECTORIES(${SimpleDemo_SOURCE_DIR}/Include)

FILE(GLOB_RECURSE SOURCE_CODE ${SimpleDemo_SOURCE_DIR}/Source/*.cpp)

FILE(GLOB_RECURSE HEADER_CODE ${SimpleDemo_SOURCE_DIR}/Include/*.h)

FILE(GLOB_RECURSE SHADERS ${SimpleDemo_SOURCE_DIR}/Shaders/*.*)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR})

ADD_EXECUTABLE(SimpleDemo ${SOURCE_CODE} ${HEADER_CODE} ${SHADERS})

TARGET_LINK_LIBRARIES(SimpleDemo RealisticEngine)

SET(CMAKE_CXX_FLAGS "-std=c++11")

add_custom_command(TARGET SimpleDemo PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${SimpleDemo_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:SimpleDemo>)

add_custom_command(TARGET SimpleDemo PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${SimpleDemo_SOURCE_DIR}/Models $<TARGET_FILE_DIR:SimpleDemo>)
