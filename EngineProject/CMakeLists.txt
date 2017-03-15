aux_source_directory("./main" sources)

file(GLOB_RECURSE resources "./data")

set(definitions
    PRIVATE -DVK_PROTOTYPES
    PRIVATE -D_USE_MATH_DEFINES
)

if(SDK_INCLUDE_PATH)
    set(includes
        PRIVATE ${SDK_INCLUDE_PATH}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
else()
    set(includes
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
endif()

set(libraries PRIVATE ${CMAKE_THREAD_LIBS_INIT})

if(TARGET vulkan)
    list(APPEND definitions PRIVATE -DUNINSTALLED_LOADER="$<TARGET_FILE:vulkan>")
endif()

if(WIN32)
    list(APPEND definitions PRIVATE -DVK_USE_PLATFORM_WIN32_KHR)
    list(APPEND definitions PRIVATE -DWIN32_LEAN_AND_MEAN)

else()

endif()

if(WIN32)
    set (MOVE_CMD "move")
    set (VULKAN_LOADER_NAME "${API_LOWERCASE}-${MAJOR}")
else()
    set (MOVE_CMD "mv")
    set (PTHREAD "pthread")
    set (VULKAN_LOADER_NAME "${API_LOWERCASE}")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/layers")
    set (VULKAN_LOADER ${VULKAN_LOADER_NAME})
else()
    find_library(VULKAN_LOADER NAMES ${VULKAN_LOADER_NAME}
    HINTS "${VULKEN_SDK}/${LIBSOURCE_DIR}" "${VULKEN_SDK}/x86_64/lib" )
endif()

if(WIN32)
    add_executable(FunnyEngine WIN32 ${sources} ${resources})
    target_compile_definitions(FunnyEngine ${definitions})
    target_include_directories(FunnyEngine ${includes})
    target_link_libraries(FunnyEngine ${libraries} ${VULKAN_LOADER})
else()

endif()

add_custom_command(TARGET FunnyEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/EngineProject/data
        ${CMAKE_BINARY_DIR}/EngineProject/data)

