CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(RealisticEngine)


include(FindGit)

################
# Dependencies #
################
SET(GLM_SRC_DIR CACHE PATH "path to glm source code")
SET(SDL2_SRC_DIR CACHE PATH "path to SDL2 source code")
SET(ASSIMP3_SRC_DIR CACHE PATH "path to assimp3 source code")
SET(FREEIMAGE_SRC_DIR CACHE PATH "path to FreeImage")

SET(PHYSX_SDK_DIR CACHE PATH "path to physx sdk")



FILE(GLOB_RECURSE header_files1 ${PHYSX_SDK_DIR}/Include/*.h)
SET(HEADER_DIRECTORIES1 "")
FOREACH(file_path ${header_files1})
  GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
  SET(HEADER_DIRECTORIES1 ${HEADER_DIRECTORIES1} ${dir_path})
ENDFOREACH()
LIST(REMOVE_DUPLICATES HEADER_DIRECTORIES1)

FILE(GLOB_RECURSE header_files2 ${PHYSX_SOURCE_DIR}/Source/*.h)
SET(HEADER_DIRECTORIES2 "")
FOREACH(file_path ${header_files2})
  GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
  SET(HEADER_DIRECTORIES2 ${HEADER_DIRECTORIES2} ${dir_path})
ENDFOREACH()
LIST(REMOVE_DUPLICATES HEADER_DIRECTORIES2)



include_directories(${HEADER_DIRECTORIES1} ${HEADER_DIRECTORIES2})

SET(HEADERS ${HEADER_DIRECTORIES1} ${HEADER_DIRECTORIES2})

message("first list")

message("${HEADER_DIRECTORIES1}")

message("second list")

message("${HEADER_DIRECTORIES2}")


file(GLOB_RECURSE sources ${PHYSX_SDK_DIR}/Source/*.cpp)

file(GLOB physx_libs
#        ${PHYSX_SDK_DIR}/Lib/linux64/libPhysX3.a
#        ${PHYSX_SDK_DIR}/Lib/linux64/libSimulationController.a
#        ${PHYSX_SDK_DIR}/Lib/linux64/libLowLevel.a
        ${PHYSX_SDK_DIR}/Lib/linux64/libPhysXProfileSDK.a
        ${PHYSX_SDK_DIR}/Lib/linux64/libPhysX3Extensions.a
#        ${PHYSX_SDK_DIR}/Lib/linux64/libSceneQuery.a
#        ${PHYSX_SDK_DIR}/Lib/linux64/libPhysX3Common.a
#        ${PHYSX_SDK_DIR}/Lib/linux64/libPhysX3CharacterKinematic.a
        ${PHYSX_SDK_DIR}/Lib/linux64/libPhysX3Vehicle.a
#        ${PHYSX_SDK_DIR}/Lib/linux64/libPhysX3Cooking.a
#        ${PHYSX_SDK_DIR}/Lib/linux64/libPvdRuntime.a
        ${PHYSX_SDK_DIR}/Lib/linux64/libPxTask.a
#        ${PHYSX_SDK_DIR}/Lib/linux64/libLowLevelCloth.a
#	${PHYSX_SDK_DIR}/Lib/linux64/*.a		
   )



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

ADD_SUBDIRECTORY(${SDL2_SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/SDL2)
ADD_SUBDIRECTORY(${ASSIMP3_SRC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Assimp3)

INCLUDE_DIRECTORIES(${FREEIMAGE_SRC_DIR}/Source ${SDL2_SRC_DIR}/include ${ASSIMP3_SRC_DIR}/include ${GLM_SRC_DIR} ${RealisticEngine_SOURCE_DIR}/Include ${HEADER_DIRECTORIES1} ${HEADER_DIRECTORIES2})

LINK_DIRECTORIES(${FREEIMAGE_SRC_DIR}/Dist ${PHYSX_SDK_DIR}/Bin/linux64)

FILE(GLOB_RECURSE SOURCE_CODE ${RealisticEngine_SOURCE_DIR}/Source/*.cpp)

FILE(GLOB_RECURSE HEADER_CODE ${RealisticEngine_SOURCE_DIR}/Include/*.h)

ADD_LIBRARY(RealisticEngine SHARED ${SOURCE_CODE} ${HEADER_CODE})

TARGET_LINK_LIBRARIES(RealisticEngine SDL2 assimp GL FreeImage PhysX3CharacterKinematic_x64 PhysX3_x64 PhysX3Cooking_x64 ${physx_libs})

ADD_SUBDIRECTORY(Projects)


SET(CMAKE_CXX_FLAGS "-std=c++0x")
