cmake_minimum_required(VERSION 2.8.11)
project (FUNNY_ENGINE)
# set (CMAKE_VERBOSE_MAKEFILE 1)
set(VULKEN_SDK $ENV{VK_SDK_PATH})
set(API_NAME "Vulkan" CACHE STRING "API name to use when building")
string(TOLOWER ${API_NAME} API_LOWERCASE)

if(EXISTS "${VULKEN_SDK}/x86_64/include")
    set(SDK_INCLUDE_PATH "${VULKEN_SDK}/x86_64/include")
    message("SDK_INCLUDE_PATH = ${SDK_INCLUDE_PATH}")
endif()

if(EXISTS "${VULKEN_SDK}/x86_64/lib")
    set(SDK_LIBRARY_PATH "${VULKEN_SDK}/x86_64/lib")
    message("SDK_LIBRARY_PATH = ${SDK_LIBRARY_PATH}")
endif()

# The MAJOR number of the version we're building, used in naming
# vulkan-<major>.dll (and other files).
set(MAJOR "1")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if(NOT WIN32)
    include(FindPkgConfig)
endif()

set (CMAKE_INSTALL_PREFIX "")

if(NOT WIN32)
    find_package(XCB REQUIRED)
    set (BUILDTGT_DIR build)
    set (BINDATA_DIR x86_64/bin)
    set (LIBSOURCE_DIR Lib)
else()
    # For Windows, since 32-bit and 64-bit items can co-exist, we build each in its own build directory.
    # 32-bit target data goes in build32, and 64-bit target data goes into build.  So, include/link the
    # appropriate data at build time.
    if (CMAKE_CL_64)
        set (BUILDTGT_DIR build)
        set (BINDATA_DIR Bin)
        set (LIBSOURCE_DIR Lib)
    else()
        set (BUILDTGT_DIR build32)
        set (BINDATA_DIR Bin32)
        set (LIBSOURCE_DIR Lib32)
    endif()
endif()

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers")
    set(COMMON_COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} -fno-strict-aliasing -fno-builtin-memcmp")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 ${COMMON_COMPILE_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_FLAGS} -std=c++11")
    if (UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    endif()
endif()

find_program(GLSLANG_VALIDATOR NAMES glslangValidator
             HINTS "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/install/bin"
                   "${PROJECT_SOURCE_DIR}/../${BINDATA_DIR}" )

find_path(GLSLANG_SPIRV_INCLUDE_DIR SPIRV/spirv.hpp HINTS "${VULKEN_SDK}/source/glslang/"
                                                          "${VULKEN_SDK}/glslang"
                                                          DOC "Path to SPIRV/spirv.hpp")
find_path(SPIRV_TOOLS_INCLUDE_DIR spirv-tools/libspirv.h HINTS "${VULKEN_SDK}/spirv-tools/include"
                                                               "${VULKEN_SDK}/source/spirv-tools/include"
                                                               "${VULKEN_SDK}/spirv-tools/external/include"
                                                               "${VULKEN_SDK}/source/spirv-tools/external/include"
                                                               DOC "Path to spirv-tools/libspirv.h")

if (WIN32)
    set (GLSLANG_SEARCH_PATH "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/glslang/Release"
                             "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/glslang/OSDependent/Windows/Release"
                             "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/hlsl/Release"
                             "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/OGLCompilersDLL/Release"
                             "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/SPIRV/Release" )
    set (SPIRV_TOOLS_SEARCH_PATH "${VULKEN_SDK}/spirv-tools/${BUILDTGT_DIR}/source/Release")
else()
    set (GLSLANG_SEARCH_PATH "${VULKEN_SDK}/glslang/build/install/lib" "${VULKEN_SDK}/x86_64/lib/glslang" )
    set (SPIRV_TOOLS_SEARCH_PATH "${VULKEN_SDK}/spirv-tools/build" "${VULKEN_SDK}/x86_64/lib/spirv-tools" )
endif()

find_library(GLSLANG_LIB NAMES glslang
             HINTS ${GLSLANG_SEARCH_PATH} )

find_library(OGLCompiler_LIB NAMES OGLCompiler
             HINTS ${GLSLANG_SEARCH_PATH} )

find_library(OSDependent_LIB NAMES OSDependent
             HINTS ${GLSLANG_SEARCH_PATH} )

find_library(HLSL_LIB NAMES HLSL
             HINTS ${GLSLANG_SEARCH_PATH} )

find_library(SPIRV_LIB NAMES SPIRV
             HINTS ${GLSLANG_SEARCH_PATH} )

find_library(SPIRV_TOOLS_LIB NAMES SPIRV-Tools
             HINTS ${SPIRV_TOOLS_SEARCH_PATH} )

# On Windows, we must pair Debug and Release appropriately
if (WIN32)
    set (GLSLANG_DEBUG_SEARCH_PATH "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/glslang/Debug"
                                   "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/glslang/OSDependent/Windows/Debug"
                                   "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/hlsl/Debug"
                                   "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/OGLCompilersDLL/Debug"
                                   "${VULKEN_SDK}/glslang/${BUILDTGT_DIR}/SPIRV/Debug")
    set (SPIRV_TOOLS_DEBUG_SEARCH_PATH "${VULKEN_SDK}/spirv-tools/${BUILDTGT_DIR}/source/Debug")

 #   add_library(glslang     STATIC IMPORTED)
 #   add_library(OGLCompiler STATIC IMPORTED)
 #   add_library(OSDependent STATIC IMPORTED)
 #   add_library(HLSL        STATIC IMPORTED)
 #   add_library(SPIRV       STATIC IMPORTED)
     add_library(Loader      STATIC IMPORTED)
 #   add_library(SPIRV-Tools STATIC IMPORTED)

    find_library(GLSLANG_DLIB NAMES glslangd
                 HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )
    find_library(OGLCompiler_DLIB NAMES OGLCompilerd
                 HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )
    find_library(OSDependent_DLIB NAMES OSDependentd
                 HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )
    find_library(HLSL_DLIB NAMES HLSLd
                 HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )
    find_library(SPIRV_DLIB NAMES SPIRVd
                 HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )
    find_library(SPIRV_TOOLS_DLIB NAMES SPIRV-Tools
                 HINTS ${SPIRV_TOOLS_DEBUG_SEARCH_PATH} )

#    set_target_properties(glslang PROPERTIES
#                         IMPORTED_LOCATION       "${GLSLANG_LIB}"
#                         IMPORTED_LOCATION_DEBUG "${GLSLANG_DLIB}")
#    set_target_properties(OGLCompiler PROPERTIES
#                         IMPORTED_LOCATION       "${OGLCompiler_LIB}"
#                         IMPORTED_LOCATION_DEBUG "${OGLCompiler_DLIB}")
#    set_target_properties(OSDependent PROPERTIES
#                         IMPORTED_LOCATION       "${OSDependent_LIB}"
#                         IMPORTED_LOCATION_DEBUG "${OSDependent_DLIB}")
#    set_target_properties(HLSL PROPERTIES
#                         IMPORTED_LOCATION       "${HLSL_LIB}"
#                         IMPORTED_LOCATION_DEBUG "${HLSL_DLIB}")
#    set_target_properties(SPIRV PROPERTIES
#                         IMPORTED_LOCATION       "${SPIRV_LIB}"
#                         IMPORTED_LOCATION_DEBUG "${SPIRV_DLIB}")
#    set_target_properties(SPIRV-Tools PROPERTIES
#                         IMPORTED_LOCATION       "${SPIRV_TOOLS_LIB}"
#                         IMPORTED_LOCATION_DEBUG "${SPIRV_TOOLS_DLIB}")

    set (GLSLANG_LIBRARIES glslang OGLCompiler OSDependent HLSL SPIRV)
    set (SPIRV_TOOLS_LIBRARIES SPIRV-Tools)
else ()
    set (GLSLANG_LIBRARIES ${GLSLANG_LIB} ${OGLCompiler_LIB} ${OSDependent_LIB} ${HLSL_LIB} ${SPIRV_LIB})
    set (SPIRV_TOOLS_LIBRARIES ${SPIRV_TOOLS_LIB})
endif()

# to run with source glslang libs/headers
if(UNIX)
    add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
    if (EXISTS "${VULKEN_SDK}/glslang")
         set (GLSLANGDIR "${VULKEN_SDK}/glslang")
    else()
         set (GLSLANGDIR "${VULKEN_SDK}/source/glslang")
    endif()
else()
    set (GLSLANGDIR "${VULKEN_SDK}/glslang")
endif()

get_filename_component(GLSLANG_PREFIX "${GLSLANGDIR}" ABSOLUTE)
if(NOT EXISTS ${GLSLANG_PREFIX})
    message(FATAL_ERROR "Necessary glslang components do not exist: " ${GLSLANG_PREFIX})
endif()

add_definitions(-DVULKAN_SAMPLES_BASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

if(WIN32)
    include_directories(${VULKEN_SDK}\\Include)
else()
    include_directories("/usr/include/vulkan" "${VULKEN_SDK}/x86_64/include/vulkan")
endif()

add_subdirectory(EngineProject)

