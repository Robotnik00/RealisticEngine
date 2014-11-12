CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(RealisticEngine)

################
# Dependencies #
################
SET(GLM_SRC_DIR CACHE PATH "path to glm source code")
SET(SDL2_SRC_DIR CACHE PATH "path to SDL2 source code")
SET(ASSIMP3_SRC_DIR CACHE PATH "path to assimp3 source code")
SET(FREEIMAGE_SRC_DIR CACHE PATH "path to FreeImage")

#SET(PHYSX_SDK_DIR CACHE PATH "path to physx sdk")



# set the output destination
set(FREEIMAGE_LIBRARY ${FREEIMAGE_SRC_DIR}/Dist/libfreeimage.a)
# create a custom target called build_scintilla that is part of ALL
# and will run each time you type make
add_custom_target(build_FreeImage ALL
                   COMMAND ${CMAKE_MAKE_PROGRAM}
                   WORKING_DIRECTORY ${FREEIMAGE_SRC_DIR}
                   COMMENT "Original FreeImage makefile target")

# now create an imported static target
add_library(FreeImage SHARED IMPORTED)
# Import target "scintilla" for configuration ""
set_property(TARGET FreeImage APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(FreeImage PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${FREEIMAGE_SRC_DIR}/Dist/libfreeimage.a")

# now you can use scintilla as if it were a regular cmake built target in your project
add_dependencies(FreeImage build_FreeImage)






################

ADD_SUBDIRECTORY(${SDL2_SRC_DIR})
ADD_SUBDIRECTORY(${ASSIMP3_SRC_DIR})

INCLUDE_DIRECTORIES(${FREEIMAGE_SRC_DIR}/Source ${SDL2_SRC_DIR}/include ${ASSIMP3_SRC_DIR}/include ${GLM_SRC_DIR} ${RealisticEngine_SOURCE_DIR}/Include)

LINK_DIRECTORIES(${FREEIMAGE_SRC_DIR}/Dist)

FILE(GLOB_RECURSE SOURCE_CODE ${RealisticEngine_SOURCE_DIR}/Source/*.cpp)

FILE(GLOB_RECURSE HEADER_CODE ${RealisticEngine_SOURCE_DIR}/Include/*.h)

ADD_LIBRARY(RealisticEngine SHARED ${SOURCE_CODE} ${HEADER_CODE})

TARGET_LINK_LIBRARIES(RealisticEngine SDL2 assimp GL FreeImage)

ADD_SUBDIRECTORY(Projects)


SET(CMAKE_CXX_FLAGS "-std=c++0x")
